#pragma once
#include <memory>
#include <vector>
#include "vertex.cpp"
#include "polygon.cpp"
#include "settings.hpp"
#include "customFloat.cpp"

namespace State{
    
    class IState {
    public:
        virtual std::vector<std::shared_ptr<IState>> find_next_states() const = 0;
        virtual bool finalized() const = 0;
        virtual void print() const = 0;
        virtual ~IState() = default;
        virtual bool equals(std::shared_ptr<IState>, Polygon::Transformations) const = 0;
    };

    template <typename Poly>
    class State: public IState {
    public:
        State(const Polygon::BarePoly& starting_frame, 
            const Polygon::Pool<Polygon::BarePoly>& poly_pool, 
            const CalcSettings& settings);

        State(const State& state);

        bool all_polys_match(const std::vector<Poly>& polyrow1, 
                             const std::vector<Poly>& polyrow2, 
                             std::function<Poly(const Poly&)> f) const;

        bool equals_derived(const State& other, Polygon::Transformations transf) const;
        bool equals(std::shared_ptr<IState> other, Polygon::Transformations transf) const override;
        bool equals_under_transform(const State& other, std::function<Poly(const Poly&)> f) const;
        std::vector<std::pair<unsigned int, const Polygon::RestrictedPoly<Poly>*>> get_remaining_poly_pool() const;
        std::vector<std::shared_ptr<IState>> find_next_states() const override;
        void insert_polygon(unsigned int node_index, unsigned int poly_index, Poly&& poly);

        bool is_valid() const;
        bool finalized() const override;
        void print() const override;

    private:
        Poly _frame;
        const std::shared_ptr<Polygon::Pool<Poly>> _poly_pool;
        std::vector<std::vector<Poly>> _current_polys;
        std::function<bool(const Poly&, const Poly&)> _overlapper;
        std::function<unsigned int(const Poly&)> _selector;
        unsigned int _size;
    };
    

    template <typename Poly>
    bool State<Poly>::all_polys_match(const std::vector<Poly>& polyrow1, 
                            const std::vector<Poly>& polyrow2, 
                            std::function<Poly(const Poly&)> f) const {

        for(const auto& poly1: polyrow1){
            bool found_match = false;
            Poly transformed_poly = f(poly1);
            for(const auto& poly2: polyrow2){
                if(transformed_poly == poly2){
                    found_match = true;
                    break;
                }
            }
            if(!found_match)
                return false;
        }
        return true;
    }

    template <typename Poly>
    State<Poly>::State(const Polygon::BarePoly& starting_frame, 
        const Polygon::Pool<Polygon::BarePoly>& poly_pool, 
        const CalcSettings& settings):
            _frame{starting_frame}, 
            _poly_pool{std::make_shared<Polygon::Pool<Poly>>(poly_pool.convert<Poly>())}, 
            _current_polys(_poly_pool->pool.size()){
                _overlapper = Polygon::overlapper_factory<Poly>(settings.overlapper);
                _selector = Polygon::selector_factory<Poly>(settings.node_selector);
            };

    template <typename Poly>
    State<Poly>::State(const State& state):
        _frame{state._frame},
        _poly_pool{state._poly_pool}, 
        _current_polys{state._current_polys},
        _overlapper{state._overlapper}, 
        _selector{state._selector},
        _size{state._size}{};


    template <typename Poly>
    bool State<Poly>::equals_derived(const State& other, Polygon::Transformations transf) const {
            

        switch(transf){
            case Polygon::Transformations::RotFlip:
                return equals_under_transform(other, Polygon::identity<Poly>) or
                        equals_under_transform(other, Polygon::rotate_90<Poly>) or
                        equals_under_transform(other, Polygon::rotate_270<Poly>) or
                        equals_under_transform(other, Polygon::rotate_180<Poly>) or
                        equals_under_transform(other, Polygon::flip_x<Poly>) or
                        equals_under_transform(other, Polygon::flip_y<Poly>) or
                        equals_under_transform(other, Polygon::flip_D<Poly>) or 
                        equals_under_transform(other, Polygon::flip_AD<Poly>);

            case Polygon::Transformations::Id:
                return equals_under_transform(other, Polygon::identity<Poly>);
    
            default:
                assert(false); // SIMAO: implementar outros casos
        }

    }


    template <typename Poly>
    bool State<Poly>::equals(std::shared_ptr<IState> other, Polygon::Transformations transf) const {
        if(typeid(*other) != typeid(*this))
            return false;
        
        std::shared_ptr<State> derivedPtr = std::static_pointer_cast<State>(other);
        return equals_derived(*derivedPtr, transf);
    }


    template <typename Poly>
    bool State<Poly>::equals_under_transform(const State& other, std::function<Poly(const Poly&)> f) const {
        if(other._size != _size)
            return false;

        if(_frame.is_valid() and other.is_valid() and !(other._frame == _frame))
            return false;

        for(unsigned int i=0; i<_current_polys.size(); i++){
            const auto& polyrow = _current_polys.at(i);
            const auto& other_polyrow = other._current_polys.at(i);
            if(!all_polys_match(polyrow, other_polyrow, f)){
                return false;
            }
        }
        return true;
    }

    template <typename Poly>
    std::vector<std::pair<unsigned int, const Polygon::RestrictedPoly<Poly>*>> State<Poly>::get_remaining_poly_pool() const {
        std::vector<std::pair<unsigned int, const Polygon::RestrictedPoly<Poly>*>> remaining;

        for(unsigned int i=0; i<_poly_pool->pool.size(); i++){
            if(_current_polys.at(i).size() < _poly_pool->pool.at(i).get_amount()){
                remaining.push_back({i, &_poly_pool->pool.at(i)});
            }
        }
        return remaining;
    }

    template <typename Poly>
    void State<Poly>::insert_polygon(unsigned int node_index, unsigned int poly_index, Poly&& poly){

        _current_polys.at(poly_index).push_back(poly);
        _size++;

        auto insertion_vertex = _frame.vertex_from_index(node_index);
        auto getter = [](unsigned int){return 0;}; // SIMAO: permitir seleccionar getter

        typename Poly::VertexType* head = poly.get_head();
        _frame.merge(insertion_vertex, poly, head);
        _frame.prune_LL({insertion_vertex, head}, getter);
        
    }

    template <typename Poly>
    std::vector<std::shared_ptr<IState>> State<Poly>::find_next_states() const {
        
        if(!is_valid() or finalized())
            return {};

        unsigned int node_index = _selector(_frame);
        const auto& insertion_vertex = *_frame.vertex_from_index(node_index);
        
        std::vector<std::shared_ptr<IState>> next_states;
        for(auto [poly_index, restricted_poly]: get_remaining_poly_pool()){
            for(const auto& transformed_poly: restricted_poly->get_variations()){

                if(angles_compatible(transformed_poly.get_head()->angle_opening, insertion_vertex.angle_opening)){
                    // SIMAO: if this next line becomes a memory bottleneck, I should implement thread-registers
                    auto new_poly = transformed_poly.copy_into(insertion_vertex);

                    const auto& restriction = restricted_poly->get_restriction();
                    if(restriction.is_valid() and !_overlapper(new_poly, restriction))
                        continue;

                    if(!_overlapper(new_poly, _frame)){
                        std::shared_ptr<State> new_state = std::make_shared<State>(*this);
                        new_state->insert_polygon(node_index, poly_index, std::move(new_poly));
                        next_states.push_back(new_state);
                    }

                }
            }
        }
        
        return next_states;
    }

    template <typename Poly>
    bool State<Poly>::is_valid() const {
        return _frame.is_valid(); // SIMAO: verificar   
    }

    template <typename Poly>
    bool State<Poly>::finalized() const {
        return _size == _poly_pool->size;
    }
    
    template <typename Poly>
    void State<Poly>::print() const {
        std::cout << "---- Printing state. Frame:\n";
        _frame.print();
        std::cout << "current polygons:\n";
        for(const auto& polyrow: _current_polys){
            for(const auto& poly: polyrow)
                poly.print();
        }
    }


    using namespace Polygon;
    using StateGenerator = std::function<std::shared_ptr<IState>(const BarePoly&, const Pool<BarePoly>&, const CalcSettings&)>;

    StateGenerator factory(const std::string& name){
        if(name == "State"){
            return [](const BarePoly& starting_frame, 
                    const Pool<BarePoly>& poly_pool, 
                    const CalcSettings& settings){

                    using Poly = LLPoly<Float<double>>;
                return std::make_shared<State<Poly>>(starting_frame, poly_pool, settings);
            };
        } else {
            return [](const BarePoly& starting_frame, 
                    const Pool<BarePoly>& poly_pool, 
                    const CalcSettings& settings){

                    using Poly = LLPoly<Float<double>>;
                return std::make_shared<State<Poly>>(starting_frame, poly_pool, settings);
            };
        }
    };

} // State