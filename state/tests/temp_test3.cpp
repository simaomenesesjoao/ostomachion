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


template <typename T>
std::vector<T> find_uniques_brute(const std::vector<T>& container){

    std::vector<T> uniques;

    for(unsigned i=0; i<container.size(); i++){
        auto& state1 = container.at(i);
        bool unique = true;

        for(unsigned j=i+1; j<container.size(); j++){
            auto& state2 = container.at(j);

            if(state2 == state1){
                unique = false;
                break;
            }
        }

        if(unique){
            uniques.push_back(state1);
        }
    }
    return uniques;
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    
    
    using Num = Float<double>;
    using Poly = Polygon<Num>;
    // using Group = ;/
    using Inner = Ostomini<Poly, SymmetryGroup::Id>;
    using Stat = State<Num, Inner, SelectLeftest, GetLast>;

    std::vector<std::tuple<unsigned int, unsigned int, bool>> indices;
    indices = {};
    // indices = {{4,0,0}, {3,0,0}, {0,0,0}, {5,0,0}, {1,2,0}, {2,0,0}, {6,0,0}};

    Stat first_state;
    bool allow_reflection = true;
    std::ofstream stream("/dev/null");
    std::vector<Stat> next_states{first_state.apply_iterations(indices, stream)};

    std::cout << "distinct: " << first_state.used_polys->get_num_distinct_polys() << "\n";

    std::cout << next_states.at(0) << "\n";
    Hash<Stat> container;
    container.insert(next_states);

    while(true){
        auto state = container.get_next();
        if(!state)
            break;
        auto next = state->find_next_states(allow_reflection, stream);
        // std::cout << next.size();
        container.insert(next);

    }

    std::cout << "final size:" << container.final_container.size() << "\n";
    std::vector<Stat> uniques = find_uniques_brute(container.final_container);
    std::cout << "uniques size:" << uniques.size() << "\n";

    // for(const auto& s1: container.final_container){
    //     for(const auto& s2: container.final_container){
    //         if(s1 == s2 and s1.get_hash() != s2.get_hash()){
    //             std::cout << "\n---------s1 hash---------:\n";
    //             s1.used_polys->calculate_hash();
    //             std::cout << "\n---------s2 hash---------:\n";
    //             s2.used_polys->calculate_hash();
    //             std::cout << s1 << "\n" << s1.get_hash() << "\n";
    //             std::cout << s2 << "\n" << s2.get_hash() << "\n";
    //             return 1;
    //         }
    //     }
    // }
    Graphics graphics;
    for(const auto& state: uniques){
        // std::cout << state.used_polys->get_size() << " ";
        // std::cout << state << "\n";
        // graphics.draw_state(state.used_polys->as_vector(), state.current_polygon->as_vector());
    }
    
    
}