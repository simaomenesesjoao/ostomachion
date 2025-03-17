#ifndef STATE_C
#define STATE_C

#include <set>
#include "polygon.cpp"
#include "ostomachion.cpp"
#include "grid.cpp"
#include <memory>


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

        long h = 0;
        for(unsigned i=0; i<polygons<Num>::num_polygons; i++){
            auto& poly = state_.at(i);
            for(auto& point: poly){
                double x = (double)point.get_x();
                double y = (double)point.get_y();
                h += (long)(x*94833373);
                h = h%100003;
                h += (long)(y*94373);
                h = h%100003;
            }
        }
        hash_ = h;
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

template <typename Num>
struct SelectObtusest{
    static unsigned int select(const Polygon<Num>& polygon){
        return polygon.get_obtusest_node();
    }
};

template <typename Num>
struct SelectLeftest{
    static unsigned int select(const Polygon<Num>& polygon){
        return polygon.get_leftest_node();       
    }
};

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

template <typename Num, typename Inner, template <typename> class Selector, typename Getter>
class State{

    using Poly = Polygon<Num>;
    using Nod = Node<Num>;
    // using Ang = Angle<Num>;
    using Poi = Point<Num>;
    std::size_t hash_;

public:
    // bool allow_reflection;
    std::unique_ptr<Poly> current_polygon;
    std::unique_ptr<Grid<Num>> grid;
    std::unique_ptr<Inner> used_polys;
    std::vector<unsigned> history;
    // std::size_t state_hash;


    State(State const& other):
        hash_{0},
        current_polygon(std::make_unique<Poly>(*other.current_polygon)),
        grid{std::make_unique<Grid<Num>>(*other.grid)},
        used_polys(std::make_unique<Inner>(*other.used_polys)), 
        history{other.history}{
            calculate_hash();
        }

    // State(Poly&& _poly, std::unique_ptr<Inner>&& _used_polys):
    //     current_polygon{std::make_unique<Poly>(_poly)}, 
    //     used_polys{std::move(_used_polys)}{}

        
    State(const Poly& _poly, const Inner& _used_polys, const std::vector<unsigned>& hist):
        hash_{0},
        current_polygon{std::make_unique<Poly>(_poly)}, 
        used_polys{std::make_unique<Inner>(_used_polys)},
        history{hist}{
            calculate_hash();
        }

    State():hash_{0},
            current_polygon(std::make_unique<Poly>(polygons<Num>::frame)), 
            used_polys(std::make_unique<Inner>()),
            history{{}}{
                calculate_hash();
            }

    State(State&& other): hash_{0},
                          current_polygon{std::move(other.current_polygon)},
                          grid{std::move(other.grid)},
                          used_polys{std::move(other.used_polys)},
                          history{std::move(other.history)}{
                            calculate_hash();
                          }

    State& operator=(State&& other){
        if(this == &other)
            return *this;

        current_polygon = std::move(other.current_polygon);
        grid = std::move(other.grid);
        used_polys = std::move(other.used_polys);
        history = std::move(other.history);
        calculate_hash();

        return *this;
    }

     State& operator=(const State& other){
        if(this == &other)
            return *this;

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
        return used_polys->size();
    }



    std::vector<State> find_next_states(bool allow_reflection, std::ostream& stream = std::cout) const{
        // Find the node with smallest internal angle
        
        std::vector<State> next_states;
        if(current_polygon->head == nullptr)
            return next_states;
        
        //unsigned obtusest_node_index = current_polygon->get_obtusest_index();
        unsigned int node_index = Selector<Num>::select(*current_polygon);
        Nod& obtusest_node = current_polygon->ll_node_from_index(node_index)->data;

        
        // stream << "------------- state ---------------\n" << *this << "\n";

        // Find which polygons haven't been used yet
        for(unsigned i = 0; i < polygons<Num>::num_polygons; i++){

            if(used_polys->is_set(i)) 
                continue;

            for(auto reflected: std::vector<bool>{true, false}){

                Poly poly = polygons<Num>::polyset.at(i);
                // stream << "considering poly " << poly << " with size " << poly.size_ll << "\n";

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
                            // stream << "#################### Found new non-overlap\n";
                            // stream << "still considering poly " << poly << " with size " << poly.size_ll << "\n";

                            // Posso ter um merge que não canibalize o outro polígono - rvalue refs
                            Poly new_frame{*current_polygon}; 
                            Poly new_poly{poly};

                            LL_Node<Nod> *node_frame = new_frame.ll_node_from_index(node_index);
                            LL_Node<Nod> *node_poly = new_poly.ll_node_from_index(j);

                            // stream << "before merge\n";
                            // stream << "node frame: " << node_frame->data << "\n";
                            // stream << "poly: " << new_poly << "\n";
                            // stream << "node poly: " << node_poly->data << "\n";
                            // stream << "new frame: " << new_frame << "\n";
                            new_frame.merge(node_frame, new_poly, node_poly);
                            // stream << "before prune\n" << new_frame << "\n";
                            // stream << "update nodes: " << node_frame->data << " " << node_poly->data << "\n";
                            new_frame.prune_LL({node_frame, node_poly}, Getter(), stream);
                            // stream << "after prune\n" << new_frame << "\n";
                            
                            
                            // std::unique_ptr<Inner> new_used_polys = std::make_unique<Inner>(*used_polys);
                            // new_used_polys->set_poly(i, poly);
                            
                            // State new_state{std::move(new_frame), std::move(new_used_polys)};
                            // next_states.push_back(std::move(new_state));
                            // next_states.push_back({std::move(new_frame), std::move(new_used_polys)});
                            
                            Inner new_used_polys = *used_polys;
                            new_used_polys.set_poly(i, poly);
                            std::vector<unsigned> new_history = history;
                            new_history.push_back(next_states.size());

                            State new_state(new_frame, new_used_polys, new_history);
                            // stream << "next state: " << new_state << "\n";
                            next_states.push_back(new_state);

                        }
                    }   
                }
            }
        }                           
        // stream << "produced " << next_states.size() << " next states\n";
        // stream << "--------------------- end -----------------\n";
        return next_states;
    }


    std::optional<State> iterate(
            unsigned int poly_index, unsigned int num_rotations, 
            bool reflected, std::ostream& stream = std::cout) const{
        
        if(current_polygon->head == nullptr)
            return std::nullopt;
        
        unsigned int node_index = Selector<Num>::select(*current_polygon);
        Nod& obtusest_node = current_polygon->ll_node_from_index(node_index)->data;

        if(used_polys->is_set(poly_index))
            return std::nullopt;

        Poly poly = polygons<Num>::polyset.at(poly_index);

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
                new_used_polys.set_poly(poly_index, poly);

                State new_state(new_frame, new_used_polys, {});
                return new_state;

            }
        }   
    
    
        return std::nullopt;
    }


    State apply_iterations
        (std::vector<std::tuple<unsigned int, unsigned int, bool>> indices, 
        std::ostream& stream){
        
        State state;

        for(auto& index: indices){
            unsigned int poly_index = std::get<0>(index);
            unsigned int num_rotations = std::get<1>(index);
            bool reflect = std::get<2>(index);
            std::optional<State> new_state = state.iterate(poly_index, num_rotations, reflect, stream);
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