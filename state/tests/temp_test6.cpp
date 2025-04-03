#include <thread>
#include <gmpxx.h>
#include "polygon.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "polyset.cpp"
#include "state.cpp"
#include "group.cpp"
#include "ArchiveQueue.cpp"
// #include "../../solver/src/graphics.cpp"


template <typename T, typename G>
std::vector<T> find_uniques_brute(const std::vector<T>& container){

    std::vector<T> uniques;

    for(unsigned i=0; i<container.size(); i++){
        auto& state1 = container.at(i);
        bool unique = true;

        for(unsigned j=i+1; j<container.size(); j++){
            auto& state2 = container.at(j);

            if(state2.used_polys->equals_under_symmetry(*state1.used_polys, G::transformations)){
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
        template <typename> class PresetPolys, 
        template <typename> class Group, 
        template <typename> class Selector,
        template <typename> class Container>
Analytics runner(const Indices& indices){
    using Poly = Polygon<Num>;
    using Inner = PolySet<Poly, Group, PresetPolys>;
    using St = State<Num, Inner, Selector, GetLast>;

    St first_state;
    bool allow_reflection = true;
    std::ofstream stream("/dev/null");

    std::vector<St> next_states{first_state.apply_iterations(indices, stream)};
    Container<St> container;
    container.analytics.info += std::format("{:>3} {:>6} {:>10}", 
        Group<Poly>::name, Container<St>::name, Selector<Num>::name);

    container.insert(next_states);
    container.analytics.clock_start();

    while(true){
        auto state = container.get_next();
        if(!state)
            break;

        auto next = state->find_next_states(allow_reflection, stream);
        container.insert(next);
    }
    container.analytics.clock_end();
    auto uniques = find_uniques_brute<St, SymmetryGroup::D4<Poly>>(container.final_container);
    container.analytics.total_final = uniques.size();

    return container.analytics;
}

template <typename Num, 
    template <typename> class PresetPolys, 
    template <typename> class Group,
    template <typename> class Selector>
std::vector<Analytics> helper1(const Indices& indices){
    auto run1 = runner<Num, PresetPolys, Group, Selector, Stack>(indices);
    auto run2 = runner<Num, PresetPolys, Group, Selector, Hash>(indices);
    auto run3 = runner<Num, PresetPolys, Group, Selector, HashLevel>(indices);
    return {run1, run2, run3};
}


template <typename Num, 
    template <typename> class PresetPolys, 
    template <typename> class Group>
std::vector<std::vector<Analytics>> helper2(const Indices& indices){
    auto run1 = helper1<Num, PresetPolys, Group, SelectLeftest>(indices);
    auto run2 = helper1<Num, PresetPolys, Group, SelectObtusest>(indices);
    auto run3 = helper1<Num, PresetPolys, Group, SelectFarthest>(indices);
    return {run1, run2, run3};
}


template <typename Num, 
    template <typename> class PresetPolys>
std::vector<std::vector<std::vector<Analytics>>> helper3(const Indices& indices){
    auto run1 = helper2<Num, PresetPolys, SymmetryGroup::Id>(indices);
    auto run2 = helper2<Num, PresetPolys, SymmetryGroup::D4>(indices);
    auto run3 = helper2<Num, PresetPolys, SymmetryGroup::C4>(indices);
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
    // using Num = Number<mpz_class, 2, 5, 13, 17>;
    std::vector<std::tuple<unsigned int, unsigned int, bool>> indices;
    indices = {};

    auto analytics_matrix = helper3<Num, Rectangles>(indices);
    for(const auto& v2: analytics_matrix){
        for(const auto& v1: v2){
            for(const auto& v0: v1){
                v0.print_single_line();
            }
        }
    }
    
    // Graphics graphics;
    // for(const auto& state: uniques){
    //     std::cout << state.used_polys->get_size() << " ";
    //     std::cout << state << "\n";
    //     graphics.draw_state(state.used_polys->as_vector(), state.current_polygon->as_vector());
    // }
    
    
}