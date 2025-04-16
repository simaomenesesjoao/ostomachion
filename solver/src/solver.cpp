#include <thread>
#include <vector>
#include <gmpxx.h>
#include <fstream>

#include "points_vectors.cpp"
#include "polygon.cpp"
//#include "customFloat.cpp"
#include "state.cpp"
//#include "group.cpp"
#include "container.hpp"
#include "settings.hpp"

std::vector<std::shared_ptr<State>> find_next_states(
    const State& state, void* overlaps, void* node_selector){
    
    if(!state.is_valid() or state.is_complete())
        return {};
    
    auto frame = state.get_frame(); // weak pointer
    unsigned int node_index = node_selector(frame);
    auto& insertion_node = node_from_index(node_index)->data;

    std::vector<std::shared_ptr<State>> next_states;
    for(const auto& restricted_poly: state.get_remaining_poly_pool()){

        for(const auto& transformed_poly: restricted_poly.variations){
            if(angles_compatible(transformed_poly.head.angle_opening, insertion_node.angle_opening)){
                auto new_poly = transformed_poly.move_into(insertion_node);

                if(overlaps(new_poly, restricted_poly.prison) and !overlaps(new_poly, frame)){
                    std::shared_ptr<State> new_state = std::make_shared<State>(state);
                    new_state->insert_polygon(node_index, new_poly);
                    next_states.push_back(new_state);
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
                auto next_states = find_next_states(**state, settings.overlapper, settings.node_selector);
                container->insert(next_states);
            }
            std::cout << "thread finishing\n";
        });
    }

    for(auto& thread: threads)
        thread.join();

    return {container->get_data(), container->get_analytics()};
}