#include <thread>
#include <gmpxx.h>
#include "polygon.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "polyset.cpp"
#include "state.cpp"
#include "group.cpp"
#include "ArchiveQueue.cpp"
#include "../../solver/src/graphics.cpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){

    using Num = Float<double>;
    using Poly = Polygon<Num>;
    using Inner = Ostomini<Poly, SymmetryGroup::Id>;
    using St = State<Num, Inner, SelectLeftest, GetLast>;
    std::vector<std::tuple<unsigned int, unsigned int, bool>> indices;
    indices = {};
    
    St first_state;
    bool allow_reflection = true;
    std::ofstream stream("/dev/null");

    std::vector<St> next_states{first_state.apply_iterations(indices, stream)};
    Stack<St> container;
    container.insert(next_states);

    while(true){
        auto state = container.get_next();
        if(!state)
            break;

        auto next = state->find_next_states(allow_reflection, stream);
        container.insert(next);
        std::cout << state->size() << "\n";
    }


    container.analytics.print();
}