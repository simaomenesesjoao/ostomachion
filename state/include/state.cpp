#ifndef STATE_C
#define STATE_C

#include <set>
#include "polygon.cpp"
#include "ostomachion.cpp"
#include "grid.cpp"
#include <memory>

// using Poi = Point<Num>;


// bool are_polys_same(const std::vector<Poi>& this_poly,
//                     const std::vector<Poi>& that_poly){

//     unsigned N = this_poly.size();
//     if(that_poly.size() != N)
//         return false;
//     if(N == 0)
//         return true;

//     // Check if any node is identical to the first
//     unsigned index_origin_this = 0;
//     unsigned index_origin_that;
//     auto& point = this_poly.at(index_origin_this);
//     bool found = false;
    
//     for(unsigned j=0; j<N; j++){
//         auto& other_point = that_poly.at(j);
//         if(other_point == point){
//             found = true;
//             index_origin_that = j;
//             break;
//         }
//     }

//     // std::cout << "indices: " << index_origin_this << " " << index_origin_that << "\n";

//     if(not found){
//         return false;
//     }

//     // Check in the direct order
//     bool direct_order = true;
//     for(unsigned j=0; j<N; j++){
//         auto& this_point = this_poly.at((j+index_origin_this)%N);
//         auto& that_point = that_poly.at((j+index_origin_that)%N);
//         if(this_point != that_point){
//             direct_order = false;
//             break;
//         }
//     }

//     if(direct_order)
//         return true;

//     // Check in the reverse order 
//     bool reverse_order = true;
//     for(unsigned j=0; j<N; j++){
//         auto& this_point = this_poly.at((j+index_origin_this)%N);
//         auto& that_point = that_poly.at((N-j+index_origin_that)%N);
//         if(this_point != that_point){
//             reverse_order = false;
//             break;
//         }
//     }

//     if(reverse_order)
//         return true;

//     return false;

// }

    // bool operator==(State const& other_state) const {
    //     // return true;
    //     // Some polygons are identical, even though they have different ids
    //     // poly6 == poly9, poly7 == poly8

    //     auto a_list = std::vector<unsigned>{5, 6}; 
    //     auto b_list = std::vector<unsigned>{8, 7};

    //     for(unsigned i=0; i<polygons<Num>::num_polygons; i++){

    //         if(std::count(a_list.begin(), a_list.end(), i) > 0 or 
    //            std::count(b_list.begin(), b_list.end(), i) > 0)
    //             continue;

    //         auto& this_poly = used_polys->at(i);
    //         auto& that_poly = other_state->used_polys.at(i);
    //         if(not are_polys_same(this_poly, that_poly))
    //             return false;
            
    //     }


    //     for(unsigned k=0; k<2; k++){
    //         unsigned a = a_list.at(k);
    //         unsigned b = b_list.at(k);
    //         bool b1 = are_polys_same(used_polys->at(a), other_state.used_polys->at(a));
    //         bool b2 = are_polys_same(used_polys->at(b), other_state.used_polys->at(b));
    //         bool b3 = are_polys_same(used_polys->at(b), other_state.used_polys->at(a));
    //         bool b4 = are_polys_same(used_polys->at(a), other_state.used_polys->at(b));

    //         if(!(b1 and b2) and !(b3 and b4))
    //             return false;
    //     }


            
    //     return true;
    // }






template <typename Num, bool enable_hash = true, bool enable_comparison = true>
class InnerState{
    using Poi = Point<Num>;
    using Nod = Node<Num>;
public:
    InnerState():
        state_{std::vector<std::vector<Poi>>(polygons<Num>::num_polygons)}, 
        size_{0}{
            calculate_hash();
        }

    bool is_set(unsigned i){
        return state_.at(i).size() != 0;
    }

    void set_poly(unsigned i, const Polygon<Num> & polygon){

        LL_Node<Nod> *poly_node = polygon.head;
        for(unsigned k = 0; k < polygon.size_ll; k++){
            state_.at(i).push_back(poly_node->data.position);
            poly_node = poly_node->next;
        }
        calculate_hash();
        size_++;
    }

    void calculate_hash(){
        if(!enable_hash){
            hash_ = 0;
            return;
        }

        // long h = 0;
        // for(unsigned i=0; i<polygons<Num>::num_polygons; i++){
        //     auto& poly = state_.at(i);
        //     for(auto& point: poly){
        //         double x = (double)point.get_x();
        //         double y = (double)point.get_y();
        //         h += (long)(x*94833373);
        //         h = h%100003;
        //         h += (long)(y*94373);
        //         h = h%100003;
        //     }
        // }
        // hash_ = h;
        // std::cout << hash_ << "\n";
    }

    std::size_t get_hash() const {
        return hash_;
    }
    
    unsigned size() const {
        return size_;
    }

    unsigned max_size() const {
        return state_.size();
    }

    std::vector<Poi> at(unsigned i) const{
        return state_.at(i);
    }



    template <typename T>
    bool are_polys_same(const T& other) const {
        if(size_ != other.size())
            return false;

        for(unsigned i=0; i<max_size(); i++){
            if(i == 5 or i == 6 or i == 7 or i == 8){
                continue;
            }

            if(at(i) != other.at(i))
                return false;
            
        }

        auto a_list = std::vector<unsigned>{5, 6}; 
        auto b_list = std::vector<unsigned>{8, 7};

        for(unsigned k=0; k<2; k++){
            unsigned a = a_list.at(k);
            unsigned b = b_list.at(k);
            bool b1 = (at(a) == other.at(a));
            bool b2 = (at(b) == other.at(b));
            bool b3 = (at(b) == other.at(a));
            bool b4 = (at(a) == other.at(b));

            if(!(b1 and b2) and !(b3 and b4))
                return false;
        }


        return true;
    }

    bool operator==(const InnerState& other) const {
        if(!enable_comparison)
            return false;
        return are_polys_same(other);
    }
    
    std::vector<std::vector<std::vector<double>>> used_polys_as_vector() const {
        std::vector<std::vector<std::vector<double>>> vector;
        for(auto& poly: state_){
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

private:
    std::vector<std::vector<Poi>> state_;
    unsigned size_;
    std::size_t hash_;
};


template <typename Num, bool a, bool b>
std::ostream& operator<<(std::ostream& stream, const InnerState<Num, a, b> & used_polys){

    for(unsigned i=0; i<used_polys.max_size(); i++){
        stream << "[";
        for(auto point: used_polys.at(i)){
            double x = (double)point.get_x();
            double y = (double)point.get_y();
            stream << "(" << x << "," << y << ") ";
        }
        stream << " ]";
    }

    stream << std::endl;
    return stream;    
}

template <typename Num, typename Inner>
class State{
    using Poly = Polygon<Num>;
    using Nod = Node<Num>;
    // using Ang = Angle<Num>;
    using Poi = Point<Num>;

public:
    // bool allow_reflection;
    std::unique_ptr<Poly> current_polygon;
    std::unique_ptr<Grid<Num>> grid;
    std::unique_ptr<Inner> used_polys;
    // std::size_t state_hash;


    State(State const& other):
        current_polygon(std::make_unique<Poly>(*other.current_polygon)),
        grid{std::make_unique<Grid<Num>>(*other.grid)},
        used_polys(std::make_unique<Inner>(*other.used_polys)){}

    State(Poly&& _poly, std::unique_ptr<Inner>&& _used_polys):
        current_polygon{std::make_unique<Poly>(_poly)}, 
        used_polys{std::move(_used_polys)}{}

    State():current_polygon(std::make_unique<Poly>(polygons<Num>::frame)), 
            used_polys(std::make_unique<Inner>()){}

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

        current_polygon = std::make_unique<Poly>(*(other.current_polygon));
        grid = std::make_unique<Grid<Num>>(*(other.grid));
        used_polys = std::make_unique<Inner>(*(other.used_polys));

        return *this;
    }



    std::vector<State> find_next_states(bool allow_reflection) const{
        // Find the node with smallest internal angle
        std::vector<State> next_states;
        if(current_polygon->head == nullptr)
            return next_states;
        
        unsigned obtusest_node_index = current_polygon->get_obtusest_index();
        Nod& obtusest_node = current_polygon->ll_node_from_index(obtusest_node_index)->data;

        // Find which polygons haven't been used yet
        for(unsigned i = 0; i < polygons<Num>::num_polygons; i++){

            if(used_polys->is_set(i)) 
                continue;

            for(auto reflected: std::vector<bool>{true, false}){

                Poly poly = Poly(polygons<Num>::polyset.at(i));

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

                        if(not current_polygon->overlaps(poly)){

                            // Posso ter um merge que não canibalize o outro polígono - rvalue refs
                            Poly new_frame{*current_polygon}; 
                            Poly new_poly{poly};

                            LL_Node<Nod> *node_frame = new_frame.ll_node_from_index(obtusest_node_index);
                            LL_Node<Nod> *node_poly = new_poly.ll_node_from_index(j);

                            new_frame.merge(node_frame, new_poly, node_poly);
                            new_frame.prune_LL({node_frame, node_poly});
                            
                            std::unique_ptr<Inner> new_used_polys = std::make_unique<Inner>(*used_polys);
                            new_used_polys->set_poly(i, poly);
                            
                            State new_state{std::move(new_frame), std::move(new_used_polys)};
                            next_states.push_back(std::move(new_state));
                            // next_states.push_back({std::move(new_frame), std::move(new_used_polys)});
                        }
                    }   
                }
            }
        }
        return next_states;
    }

    State iterate(std::vector<unsigned> indices, bool allow_reflection){
        std::vector<State> next_states = find_next_states(allow_reflection);
        State state;

        for(auto& index: indices){
            
            if(next_states.size() == 0){
                std::cout << "empty\n";
                break;
            }
            if(index >= next_states.size()){
                std::cout << "out of range\n";
                break;
            }


            state = std::move(next_states.at(index));
            next_states = state.find_next_states(allow_reflection);
        }
        return state;
    }


};

template <typename Num, typename Inner>
std::ostream& operator<<(std::ostream& stream, State<Num, Inner> const& state){
    

    // Print frame
    stream << "Frame: {"; 
    LL_Node<Node<Num>> *current = state.current_polygon->head;
    for(unsigned i=0; i< state.current_polygon->size_ll; i++){
        double x = (double)current->data.position.get_x();
        double y = (double)current->data.position.get_y();
        stream << "(" << x << "," << y << ") ";
        current = current->next;
    }
    stream << "}";
    
    
    // Print used polys
    stream << " Used polys:";
    const Inner& inner = *state.used_polys;
    stream << inner << "\n";
    
    return stream;
}


#endif // STATE_C