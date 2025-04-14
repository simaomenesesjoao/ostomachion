#include <memory>
#include "polygon.cpp"
#include "settings.hpp"

class State{
public:
    State(const Polygon::BarePoly& starting_frame, 
        std::shared_ptr<std::vector<Polygon::RestrictedPoly>> poly_pool, 
        const CalcSettings& settings):
            _frame{std::move(Polygon::factory(settings.poly_type)(starting_frame))}, 
            _poly_pool{poly_pool}, _current_polys{}{

            };
    // State(const State&){};
    // State(State&&){};
    void insert(){};
    bool is_valid() const {
        return _frame->is_valid();
    };
    bool is_complete() const {
        return false;
    };
    bool finalized(){
        return false;
    };

private:
    std::unique_ptr<Polygon::IPoly> _frame;
    std::shared_ptr<std::vector<Polygon::RestrictedPoly>> _poly_pool;
    std::vector<std::unique_ptr<Polygon::IPoly>> _current_polys;
};