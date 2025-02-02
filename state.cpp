#ifndef STATE_C
#define STATE_C

#include "polygon.cpp"
#include <set>
#include "ostomachion.cpp"

template <int... Ints>
class Tracker{


                        // auto hash = next_state.get_hash();
};

template <int... Ints>
class State{
    using Poly = Polygon<Ints...>;
    using Nod = Node<Ints...>;
    using Num = Number<Ints...>;
    using Ang = Angle<Ints...>;
    using Poi = Point<Ints...>;
    // Characterizes an ostomachion state by the current polygon 
    // (through the positions of the Nodes) and unused polygons
public:
    
    Poly current_polygon;
    std::vector<std::vector<Poi>> used_polys;
    
    // Tracker<Ints...> tracker;

    // State(Tracker& tracker): current_polygon{polygons::frame}, tracker{tracker}{
    // }

    State(State const& other):
        current_polygon(other.current_polygon), 
        used_polys(other.used_polys){}

    State(Poly const& _poly, std::vector<std::vector<Poi>> const& _used_polys):
        current_polygon{_poly}, used_polys{_used_polys}{}

    State():current_polygon(polygons::frame){
        for(unsigned i=0; i<polygons::num_polygons; i++){
            used_polys.push_back(std::vector<Poi>());
        }
    }
    // ~State(){

    // }

    long get_hash(){
        // Hashes the state. This cannot depend on the order of the 
        // nodes in the polygon


        long hash{};

        LL_Node<Nod> *current{current_polygon.head};
        for(int i=0; i < current_polygon.size_ll; i++){
            auto& [x, y] = current->data.position;
            current = current->next;

            // Unfold x, y into their components and make hash from it

            hash += x*256 + y;
        }

        return hash;

    }


    static bool equals(State const& state1, State const& state2){
        return false;

    }

    

    
    std::vector<State> find_next_states(){
        // Find the node with smallest internal angle
        std::vector<State> next_states;
        
        unsigned obtusest_node_index = current_polygon.get_obtusest_index();
        Nod& obtusest_node = current_polygon.ll_node_from_index(obtusest_node_index)->data;

        // Find which polygons haven't been used yet
        for(unsigned i = 0; i < polygons::num_polygons; i++){
            

            if(used_polys.at(i).size() != 0) 
                continue;

            Poly poly = Poly(polygons::polyset.at(i));
            LL_Node<Nod> *current = poly.head;
            for(unsigned j = 0; j < poly.size_ll; j++){
                Nod& proposed_node = current->data;
                current = current->next;

                if(angles_compatible(proposed_node.angle_opening, obtusest_node.angle_opening)){
                    poly.translate(obtusest_node.position - proposed_node.position);
                    poly.rotate(obtusest_node.angle_start - proposed_node.angle_end, 
                                obtusest_node.position);
                    if(true or not current_polygon.overlaps(poly)){

                        // Posso ter um merge que não canibalize o outro polígono - rvalue refs
                        Poly new_frame{current_polygon}; // move this into the next state
                        Poly new_poly{poly};

                        LL_Node<Nod> *node_frame = new_frame.ll_node_from_index(obtusest_node_index);
                        LL_Node<Nod> *node_poly = new_poly.ll_node_from_index(j);

                        new_frame.merge(node_frame, new_poly, node_poly);
                        new_frame.prune_LL({node_frame, node_poly});
                        std::vector<std::vector<Poi>> new_used_polys{used_polys};

                        LL_Node<Nod> *poly_node = poly.head;
                        for(unsigned k = 0; k < poly.size_ll; k++){
                            new_used_polys.at(i).push_back(poly_node->data.position);
                            poly_node = poly_node->next;
                        }
                                               
                        // State next_state(new_frame, new_used_polys);
                        next_states.push_back({new_frame, new_used_polys});
                    }
                }   
            }
        }
        return next_states;
    }

};

template <int... Ints>
std::ostream& operator<<(std::ostream& stream, State<Ints...> const& state){
    

    // Print frame
    stream << "Frame: {"; 
    LL_Node<Node<Ints...>> *current = state.current_polygon.head;
    for(unsigned i=0; i< state.current_polygon.size_ll; i++){
        double x = (double)current->data.position.get_x();
        double y = (double)current->data.position.get_y();
        stream << "(" << x << "," << y << ") ";
        current = current->next;
    }
    stream << "}";
    // print_poly(state.current_polygon);
    
    
    // Print used polys
    stream << " Used polys:";
    for(unsigned i=0; i<state.used_polys.size(); i++){
        stream << "[";
        for(auto& point: state.used_polys.at(i)){
            double x = (double)point.get_x();
            double y = (double)point.get_y();
            stream << "(" << x << "," << y << ") ";
        }
        stream << " ]";
    }
    stream << std::endl;
    return stream;
}


#endif // STATE_C