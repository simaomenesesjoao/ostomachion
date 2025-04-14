#ifndef STATE_C
#define STATE_C

#include <set>
#include "polygon.cpp"
#include "ostomachion.cpp"
#include "grid.cpp"
#include <memory>
#include <optional>

template <typename Num>
struct SelectObtusest{
    static std::string name;
    static unsigned int select(const Polygon<Num>& polygon){
        return polygon.get_obtusest_node();
    }
};
template <typename Num> std::string SelectObtusest<Num>::name = "Obtusest";

template <typename Num>
struct SelectFarthest{
    static std::string name;
    static unsigned int select(const Polygon<Num>& polygon){
        return polygon.get_farthest_node(0,0);
    }
};
template <typename Num> std::string SelectFarthest<Num>::name = "Farthest";

template <typename Num>
struct SelectLeftest{
    static std::string name;
    static unsigned int select(const Polygon<Num>& polygon){
        return polygon.get_leftest_node();       
    }
};
template <typename Num> std::string SelectLeftest<Num>::name = "Leftest";

struct GetFirst{
    static unsigned int get([[maybe_unused]] int max){
        return 0;
    }
};


struct GetLast{
    static unsigned int get(int max){
        return max-1;
    }
};


template <typename Poly>
class OverlapChecker{
public:
    virtual bool operator()(const Poly& poly, const Poly& frame, const std::vector<std::vector<Poly>>& poly_vec) const = 0;
};


template <typename Poly>
class OverlapConvex: public OverlapChecker<Poly>{
private:
    bool overlaps(const Poly& poly, const Poly& frame) const {
        return frame.overlaps(poly);
    }
    bool overlaps(const Poly& poly, const std::vector<std::vector<Poly>>& poly_vec) const {
        // Also needs to take into acount the boundaries!
        
        for(const auto& polyrow: poly_vec){
            for(const Poly& other_poly: polyrow){
                if(poly.convex_overlaps(other_poly)){
                    return true;
                }
            }
        }
        return false;
    }
    bool in_bounds(const Poly& poly) const {
        LL_Node<typename Poly::Nod> *current = poly.head;
        int min = -6;
        int max = 6;
        for(unsigned int i=0; i<poly.size_ll; i++){
            const auto& P = current->data.position;
            const auto& x = P.get_x();
            const auto& y = P.get_y();
            if(x < min or x > max or y < min or y > max)
                return false;
            current = current->next;
        }

        return true;
    }

public:
    bool operator()(const Poly& poly, [[maybe_unused]] const Poly& frame, 
        [[maybe_unused]] const std::vector<std::vector<Poly>>& poly_vec) const override {
        // bool r1 = overlaps(poly, frame);
        // Poly fr{{{-6,6}, {-6,6}, {6,6}, {6,-6}}};

        // bool r2 = ;


        // for(const auto& polyrow: poly_vec){
        //     for(const Poly& other_poly: polyrow){
        //         if(!r1 and poly.convex_overlaps(other_poly)){
        //             std::cout << "oops\n";
        //             poly.convex_overlaps(other_poly);
        //         }
        //     }
        // }

        // bool r4 = 
        // if(r1 != r4){
        //     std::cout << "oh no\n";
        //     return r1;
        // }
        return not in_bounds(poly) or overlaps(poly, poly_vec);


    }
};

template <typename Poly>
class OverlapChain: public OverlapChecker<Poly>{

    bool operator()(const Poly& poly, const Poly& frame, const std::vector<std::vector<Poly>>& poly_vec) const override {
        return frame.overlaps(poly);
    }
};

template <typename Num, typename Inner, template <typename> class Selector, typename Getter>
class State{
    using Poly = Polygon<Num>;
    using Nod = Node<Num>;
    using Poi = Point<Num>;
    std::size_t hash_;

public:
    using In = Inner;
    
    std::unique_ptr<Poly> current_polygon;
    std::unique_ptr<Grid<Num>> grid;
    std::unique_ptr<Inner> used_polys;
    std::vector<unsigned> history;
    std::shared_ptr<OverlapChecker<Poly>> overlaps;

    State(State const& other):
        hash_{other.get_hash()},
        current_polygon(std::make_unique<Poly>(*other.current_polygon)),
        grid{std::make_unique<Grid<Num>>(*other.grid)},
        used_polys(std::make_unique<Inner>(*other.used_polys)), 
        history{other.history},
        overlaps{other.overlaps}{}

        
    State(const Poly& _poly, const Inner& _used_polys, std::shared_ptr<OverlapChecker<Poly>> _overlaps):
        hash_{0},
        current_polygon{std::make_unique<Poly>(_poly)}, 
        used_polys{std::make_unique<Inner>(_used_polys)},
        overlaps{_overlaps}{
            calculate_hash();
        }

    State(std::shared_ptr<OverlapChecker<Poly>> _overlaps):
            hash_{0},
            current_polygon(std::make_unique<Poly>(Inner::S::frame)), 
            used_polys(std::make_unique<Inner>()),
            overlaps{_overlaps}{
                calculate_hash();
            }

    State(State&& other): hash_{other.get_hash()},
                          current_polygon{std::move(other.current_polygon)},
                          grid{std::move(other.grid)},
                          used_polys{std::move(other.used_polys)},
                          overlaps{other.overlaps}{}

    State& operator=(State&& other){
        if(this == &other)
            return *this;

        overlaps = other.overlaps;
        hash_ = other.get_hash();
        current_polygon = std::move(other.current_polygon);
        grid = std::move(other.grid);
        used_polys = std::move(other.used_polys);
        history = std::move(other.history);

        return *this;
    }

     State& operator=(const State& other){
        if(this == &other)
            return *this;

        overlaps = other.overlaps;
        current_polygon = std::make_unique<Poly>(*(other.current_polygon));
        grid = std::make_unique<Grid<Num>>(*(other.grid));
        used_polys = std::make_unique<Inner>(*(other.used_polys));
        history = other.history;
        calculate_hash();

        return *this;
    }


    std::size_t get_hash() const {
        return hash_;
    }

    void calculate_hash() {
        std::size_t h1 = used_polys->get_hash();
        std::size_t h2 = current_polygon->get_hash();
        hash_ = (h1 + 19*h2)%991482311;
    }

    bool operator==(const State& other) const {
        return *used_polys == *(other.used_polys) and *current_polygon == *(other.current_polygon);
    }

    unsigned size() const {
        return used_polys->get_size();
    }


    std::vector<std::shared_ptr<State>> find_next_states(bool allow_reflection, std::ostream& stream = std::cout) const{
        
        
        std::vector<std::shared_ptr<State>> next_states;
        if(current_polygon->head == nullptr)
            return next_states;
        
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


    std::optional<std::shared_ptr<State>> iterate(
            unsigned int poly_index, unsigned int num_rotations, 
            bool reflected, std::ostream& stream = std::cout) const{    
        
        if(current_polygon->head == nullptr)
            return std::nullopt;
        
        unsigned int node_index = SelectLeftest<Num>::select(*current_polygon);
        Nod& obtusest_node = current_polygon->ll_node_from_index(node_index)->data;

        if(!used_polys->is_type_available(poly_index))
            return std::nullopt;

        Poly poly = used_polys->get_prepoly_at(poly_index);

        if(num_rotations >= poly.size_ll) 
            return std::nullopt;

        if(reflected)
            poly.flip_x();

        LL_Node<Nod> *current = poly.head;
        for(unsigned j = 0; j < num_rotations; j++)
            current = current->next;
            
        Nod& proposed_node = current->data;
        
        if(angles_compatible(proposed_node.angle_opening, obtusest_node.angle_opening)){
            poly.translate(obtusest_node.position - proposed_node.position);
            poly.rotate(obtusest_node.angle_start - proposed_node.angle_end, 
                        obtusest_node.position);


            if(not current_polygon->overlaps(poly)){
                
                Poly new_frame{*current_polygon}; 
                Poly new_poly{poly};

                LL_Node<Nod> *node_frame = new_frame.ll_node_from_index(node_index);
                LL_Node<Nod> *node_poly = new_poly.ll_node_from_index(num_rotations);

                new_frame.merge(node_frame, new_poly, node_poly);
                new_frame.prune_LL({node_frame, node_poly}, Getter(), stream);

                Inner new_used_polys = *used_polys;
                new_used_polys.insert(poly_index, poly);

                return std::make_shared<State>(new_frame, new_used_polys, overlaps);

            }
        }   
    
    
        return std::nullopt;
    }


    std::shared_ptr<State> apply_iterations
        (std::vector<std::tuple<unsigned int, unsigned int, bool>> indices, 
        std::ostream& stream){
        
        std::shared_ptr<State> state = std::make_shared<State>(overlaps);

        for(auto& index: indices){
            unsigned int poly_index = std::get<0>(index);
            unsigned int num_rotations = std::get<1>(index);
            bool reflect = std::get<2>(index);
            std::optional<std::shared_ptr<State>> new_state = state->iterate(poly_index, num_rotations, reflect, stream);
            assert(new_state);
            state = *new_state;
        }
        return state;
    }


};

template <typename Num, typename Inner, template <typename> class Select, typename Get>
std::ostream& operator<<(std::ostream& stream, State<Num, Inner, Select, Get> const& state){
    

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
    stream << inner << " ";

    stream << "history: ";
    for(auto& h: state.history)
        stream << h << " ";
    stream << "\n";

    return stream;
}


#endif // STATE_C