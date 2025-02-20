#ifndef STATE_C
#define STATE_C

#include <set>
#include "polygon.cpp"
#include "ostomachion.cpp"
#include "grid.cpp"

template <int... Ints>
class Tracker{


                        // auto hash = next_state.get_hash();
};

template <typename Num>
class State{
    using Poly = Polygon<Num>;
    using Nod = Node<Num>;
    using Ang = Angle<Num>;
    using Poi = Point<Num>;
public:
    
    Poly current_polygon;
    Grid<Num> grid;
    std::vector<std::vector<Poi>> used_polys;

    State(State const& other):
        current_polygon(other.current_polygon), 
        used_polys(other.used_polys){}

    State(Poly const& _poly, std::vector<std::vector<Poi>> const& _used_polys):
        current_polygon{_poly}, used_polys{_used_polys}{}

    // Constructor
    State():current_polygon(polygons<Num>::frame){
        // grid{2, 3, {-1,1}, {-1,1}, {13,1}, {13,1}}
        for(unsigned i=0; i<polygons<Num>::num_polygons; i++){
            used_polys.push_back(std::vector<Poi>());
        }

        LL_Node<Node<Num>>* current = current_polygon.head;
        for(unsigned i=0; i<current_polygon.size_ll; i++){
            // grid.rasterize_edge(current);
            current = current->next;
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

    std::vector<State> find_next_states() const{
        // Find the node with smallest internal angle
        std::vector<State> next_states;
        if(current_polygon.head == nullptr)
            return next_states;
        
        unsigned obtusest_node_index = current_polygon.get_obtusest_index();
        Nod& obtusest_node = current_polygon.ll_node_from_index(obtusest_node_index)->data;
        //std::cout << current_polygon << "\n";

        // Find which polygons haven't been used yet
        for(unsigned i = 0; i < polygons<Num>::num_polygons; i++){

            
            if(used_polys.at(i).size() != 0) 
                continue;

            Poly poly = Poly(polygons<Num>::polyset.at(i));
            LL_Node<Nod> *current = poly.head;
            for(unsigned j = 0; j < poly.size_ll; j++){
                Nod& proposed_node = current->data;
                current = current->next;
                
                if(angles_compatible(proposed_node.angle_opening, obtusest_node.angle_opening)){
                    poly.translate(obtusest_node.position - proposed_node.position);
                    poly.rotate(obtusest_node.angle_start - proposed_node.angle_end, 
                                obtusest_node.position);
                    if(not current_polygon.overlaps(poly)){
                    // if(true or not current_polygon.overlaps(poly)){

                        // Posso ter um merge que não canibalize o outro polígono - rvalue refs
                        Poly new_frame{current_polygon}; // move this into the next state
                        Poly new_poly{poly};

                        LL_Node<Nod> *node_frame = new_frame.ll_node_from_index(obtusest_node_index);
                        LL_Node<Nod> *node_poly = new_poly.ll_node_from_index(j);

                        new_frame.merge(node_frame, new_poly, node_poly);
                        // std::cout << "New frame after merge " << new_frame << "\n";
                        new_frame.prune_LL({node_frame, node_poly});
                        // std::cout << "New frame after prune " << new_frame << "\n";
                        std::vector<std::vector<Poi>> new_used_polys{used_polys};

                        LL_Node<Nod> *poly_node = poly.head;
                        for(unsigned k = 0; k < poly.size_ll; k++){
                            new_used_polys.at(i).push_back(poly_node->data.position);
                            poly_node = poly_node->next;
                        }
                        
                        next_states.push_back({new_frame, new_used_polys});
                    }
                }   
            }
        }
        return next_states;
    }

};

template <typename Num>
std::ostream& operator<<(std::ostream& stream, State<Num> const& state){
    

    // Print frame
    stream << "Frame: {"; 
    LL_Node<Node<Num>> *current = state.current_polygon.head;
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