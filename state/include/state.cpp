#ifndef STATE_C
#define STATE_C

#include <set>
#include "polygon.cpp"
#include "ostomachion.cpp"
#include "grid.cpp"
#include <memory>

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
    // std::size_t state_hash;


    State(State const& other):
        current_polygon(other.current_polygon),
        grid{other.grid},
        used_polys(other.used_polys){}

    State(Poly const& _poly, std::vector<std::vector<Poi>> const& _used_polys):
        current_polygon{_poly}, used_polys{_used_polys}{}

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

    State(State&& other):current_polygon{std::move(other.current_polygon)},
                         grid{std::move(other.grid)},
                         used_polys{std::move(other.used_polys)}{}

    State& operator=(State&& other){
        if(this == &other)
            return *this;

        current_polygon = std::move(other.current_polygon);
        grid = std::move(other.grid);
        used_polys = std::move(other.used_polys);

        return *this;
    }

     State& operator=(const State& other){
        if(this == &other)
            return *this;

        current_polygon = other.current_polygon;
        grid = other.grid;
        used_polys = other.used_polys;

        return *this;
    }



    // ~State(){

    // }

    // void set_hash(){
        
    //     // state_hash = 0;
    //     // return;

    //     long hash = 0;
    //     for(unsigned i=0; i<polygons<Num>::num_polygons; i++){
    //         auto& poly = used_polys.at(i);
    //         for(auto& point: poly){
    //             double x = (double)point.get_x();
    //             double y = (double)point.get_y();
    //             //std::cout << x << " " << y << "\n";
    //             hash += (long)(x*100000000) + (long)(y*10000);
    //             if(i==6 or i==7)
    //                 hash *= 6+7;
    //             else if(i==5 or i==8)
    //                 hash *= 5+8;
    //             else
    //                 hash *= (i+1);

    //             hash = hash%100000;
    //             // std::cout << hash <<;
    //         }


    //     }
    //     state_hash = hash;
    // }
    
    // struct HashFunction {
    //     size_t operator()(const State& state) const {
    //         return state.state_hash;
    //     }
    // };

    bool are_polys_same(const std::vector<Poi>& this_poly,
                        const std::vector<Poi>& that_poly) const {
        // std::cout << "checking equality!\n";

        unsigned N = this_poly.size();
        if(that_poly.size() != N)
            return false;
        if(N == 0)
            return true;

        // Check if any node is identical to the first
        unsigned index_origin_this = 0;
        unsigned index_origin_that;
        auto& point = this_poly.at(index_origin_this);
        bool found = false;
        
        for(unsigned j=0; j<N; j++){
            auto& other_point = that_poly.at(j);
            if(other_point == point){
                found = true;
                index_origin_that = j;
                break;
            }
        }

        // std::cout << "indices: " << index_origin_this << " " << index_origin_that << "\n";

        if(not found){
            return false;
        }

        // Check in the direct order
        bool direct_order = true;
        for(unsigned j=0; j<N; j++){
            auto& this_point = this_poly.at((j+index_origin_this)%N);
            auto& that_point = that_poly.at((j+index_origin_that)%N);
            if(this_point != that_point){
                direct_order = false;
                break;
            }
        }

        if(direct_order)
            return true;

        // Check in the reverse order 
        bool reverse_order = true;
        for(unsigned j=0; j<N; j++){
            auto& this_point = this_poly.at((j+index_origin_this)%N);
            auto& that_point = that_poly.at((N-j+index_origin_that)%N);
            if(this_point != that_point){
                reverse_order = false;
                break;
            }
        }

        if(reverse_order)
            return true;

        return false;

    }

    bool operator==(State const& other_state) const {
        // Some polygons are identical, even though they have different ids
        // poly6 == poly9, poly7 == poly8

        auto a_list = std::vector<unsigned>{5, 6}; 
        auto b_list = std::vector<unsigned>{8, 7};

        for(unsigned i=0; i<polygons<Num>::num_polygons; i++){

            if(std::count(a_list.begin(), a_list.end(), i) > 0 or 
               std::count(b_list.begin(), b_list.end(), i) > 0)
                continue;

            auto& this_poly = used_polys.at(i);
            auto& that_poly = other_state.used_polys.at(i);
            if(not are_polys_same(this_poly, that_poly))
                return false;
            
        }


        for(unsigned k=0; k<2; k++){
            unsigned a = a_list.at(k);
            unsigned b = b_list.at(k);
            bool b1 = are_polys_same(used_polys.at(a), other_state.used_polys.at(a));
            bool b2 = are_polys_same(used_polys.at(b), other_state.used_polys.at(b));
            bool b3 = are_polys_same(used_polys.at(b), other_state.used_polys.at(a));
            bool b4 = are_polys_same(used_polys.at(a), other_state.used_polys.at(b));

            if(!(b1 and b2) and !(b3 and b4))
                return false;
        }


            
        return true;
    }

    std::vector<State> find_next_states() const{
        // Find the node with smallest internal angle
        std::vector<State> next_states;
        if(current_polygon.head == nullptr)
            return next_states;
        
        unsigned obtusest_node_index = current_polygon.get_obtusest_index();
        Nod& obtusest_node = current_polygon.ll_node_from_index(obtusest_node_index)->data;
        // std::cout << "Current poly:\n";
        // current_polygon.print();
        // std::cout << "\n";

        // Find which polygons haven't been used yet
        for(unsigned i = 0; i < polygons<Num>::num_polygons; i++){

            // std::cout << "Testing poly " << i << "\n";
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
                    
                    // std::cout << "\nPoly " << i << " Node " << j << "\n";
                    // poly.print(); // ############################333


                    if(not current_polygon.overlaps(poly)){
                        // std::cout << "was accepted\n";
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
                        
                        State new_state{new_frame, new_used_polys};
                        // new_state.set_hash();
                        next_states.push_back(std::move(new_state));
                    }
                }   
            }
        }
        return next_states;
    }

    std::vector<std::vector<std::vector<double>>> used_polys_as_vector() const {
        std::vector<std::vector<std::vector<double>>> vector;
        for(auto& poly: used_polys){
            std::vector<std::vector<double>> pol;
            for(auto& point: poly){
                double x = (double)point.get_x();
                double y = (double)point.get_y();
                std::vector<double> P{x,y};
                pol.push_back(P);
            }
            vector.push_back(pol);

        }
        return vector;
    }


    std::vector<std::vector<double>> frame_as_vector() const {
        std::vector<std::vector<double>> vector;
        LL_Node<Nod>* current{current_polygon.head}; 
        for(unsigned i=0; i<current_polygon.size_ll; i++){
            double x = (double)current->data.position.get_x();
            double y = (double)current->data.position.get_y();
            std::vector<double> point{x,y};
            vector.push_back(point);
            current = current->next;
        }
        
        return vector;
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