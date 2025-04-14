#include <thread>
#include <gmpxx.h>
#include <fstream>

#include "polygon.cpp"
//#include "customFloat.cpp"
#include "state.cpp"
//#include "group.cpp"
#include "container.hpp"
#include "settings.hpp"

std::vector<std::shared_ptr<State>> find_next_states(
    const State& state, const CalcSettings& settings, int thread_register){

    std::vector<std::shared_ptr<State>> next_states;
    if(!state.is_valid() or state.is_complete())
        return {};
    
    unsigned int node_index = Selector<Num>::select(*current_polygon);
    Nod& obtusest_node = current_polygon->ll_node_from_index(node_index)->data;

    // Find which polygons haven't been used yet
    for(unsigned int i: used_polys->find_available_types()){

        for(auto reflected: std::vector<bool>{true, false}){

            Poly poly = used_polys->get_prepoly_at(i);

            if(not allow_reflection and reflected){
                continue;
            }
                
            if(reflected){
                poly.flip_x();
            } 

            LL_Node<Nod> *current = poly.head;
            for(unsigned j = 0; j < poly.size_ll; j++){
                Nod& proposed_node = current->data;
                current = current->next;
                
                if(angles_compatible(proposed_node.angle_opening, obtusest_node.angle_opening)){
                    poly.translate(obtusest_node.position - proposed_node.position);
                    poly.rotate(obtusest_node.angle_start - proposed_node.angle_end, 
                                obtusest_node.position);
                    
                    // if(not current_polygon->overlaps(poly))){
                    if(not overlaps->operator()(poly, *current_polygon, used_polys->get_all_polys())){

                        if(i==0 and !poly.convex_overlaps(used_polys->get_irreducible_section())){                            
                            continue;
                        }
                        // Posso ter um merge que não canibalize o outro polígono - rvalue refs
                        Poly new_frame{*current_polygon}; 
                        Poly new_poly{poly};

                        LL_Node<Nod> *node_frame = new_frame.ll_node_from_index(node_index);
                        LL_Node<Nod> *node_poly = new_poly.ll_node_from_index(j);

                        new_frame.merge(node_frame, new_poly, node_poly);
                        new_frame.prune_LL({node_frame, node_poly}, Getter(), stream);
                        
                        Inner new_used_polys = *used_polys;
                        new_used_polys.insert(i, poly);
                        
                        next_states.push_back(std::make_shared<State>(new_frame, new_used_polys, overlaps));

                    }
                }   
            }
        }
    }

    return next_states;
    
}

std::pair<std::vector<std::shared_ptr<State>>, Analytics> get_combinations(
    const Input::Input& input, 
    const CalcSettings& settings, 
    const AnalyticsSettings& analytics_settings){

    auto PolyPoolPtr = std::make_shared<std::vector<Polygon::RestrictedPoly>>(input.polygon_pool);
    auto first_state = std::make_shared<State>(input.frame, PolyPoolPtr, settings);
    // first_state.iterate(input.initializer);

    auto container = Container::factory<std::shared_ptr<State>>(settings.container_type);
    container->insert({first_state});

    std::vector<std::thread> threads;
    for(int i=0; i<settings.num_threads; i++){
        threads.emplace_back([&container, &settings](){
            while(true){
                auto state = container->pop();
                if(!state)
                    break;
                auto next_states = find_next_states(**state, settings, 1);
                container->insert(next_states);
            }
            std::cout << "thread finishing\n";
        });
    }

    for(auto& thread: threads)
        thread.join();

    return {container->get_data(), container->get_analytics()};
}