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

using Indices = std::vector<std::tuple<unsigned int, unsigned int, bool>>;

template <typename Num, 
        template <typename, template <typename> class> class Set,
        template <typename> class Group, 
        template <typename> class Selector,
        template <typename> class Container>
Analytics runner(const Indices& indices){
    std::cout << "runner\n";
    using Poly = Polygon<Num>;
    using Inner = Set<Poly, Group>;
    using St = State<Num, Inner, Selector, GetLast>;

    St first_state;
    bool allow_reflection = true;
    std::ofstream stream("/dev/null");

    std::vector<St> next_states{first_state.apply_iterations(indices, stream)};
    Container<St> container;
    container.insert(next_states);

    while(true){
        auto state = container.get_next();
        if(!state)
            break;

        auto next = state->find_next_states(allow_reflection, stream);
        container.insert(next);
    }

    return container.analytics;
}


template <typename Num, 
    template <typename, template <typename> class> class Set, 
    template <typename> class Group, 
    template <typename> class Selector>
std::vector<Analytics> helper1(const Indices& indices){
    auto run1 = runner<Num, Set, Group, Selector, Hash>(indices);
    auto run2 = runner<Num, Set, Group, Selector, Stack>(indices);
    return {run1, run2};
}


template <typename Num, 
    template <typename, template <typename> class> class Set, 
    template <typename> class Group>
std::vector<std::vector<Analytics>> helper2(const Indices& indices){
    auto run1 = helper1<Num, Set, Group, SelectLeftest>(indices);
    auto run2 = helper1<Num, Set, Group, SelectObtusest>(indices);
    auto run3 = helper1<Num, Set, Group, SelectFarthest>(indices);
    return {run1, run2, run3};
}


template <typename Num, template <typename, template <typename> class> class Set>
std::vector<std::vector<std::vector<Analytics>>> helper3(const Indices& indices){
    auto run1 = helper2<Num, Set, SymmetryGroup::Id>(indices);
    auto run2 = helper2<Num, Set, SymmetryGroup::D4>(indices);
    auto run3 = helper2<Num, Set, SymmetryGroup::C4>(indices);
    return {run1, run2, run3};
}



int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){

    // {
    //     using Num = Float<double>;
    //     using Poly = Polygon<Num>;
    //     using Set = Ostomini<Poly, SymmetryGroup::D4>;
    //     using St = State<Num, Set, SelectLeftest, GetFirst>;
    //     St state;

    // }

    using Num = Float<double>;
    std::vector<std::tuple<unsigned int, unsigned int, bool>> indices;
    indices = {};

    auto analytics_matrix = helper3<Num, Ostomini>(indices);
    
    // Graphics graphics;
    // for(const auto& state: uniques){
    //     std::cout << state.used_polys->get_size() << " ";
    //     std::cout << state << "\n";
    //     graphics.draw_state(state.used_polys->as_vector(), state.current_polygon->as_vector());
    // }
    
    
}