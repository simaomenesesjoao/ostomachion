#include <thread>
#include <gmpxx.h>
#include "polygon.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "polyset.cpp"
#include "state.cpp"
#include "group.cpp"
#include "ArchiveQueue.cpp"


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

template <typename Container>
void run(Container& container, bool allow_reflection){
    std::ofstream stream("/dev/null");
    
    while(true){
        auto maybe_state = container.get_next();
        if(!maybe_state)
            break;

        auto next = maybe_state->find_next_states(allow_reflection, stream);
        container.insert(next);
    }
}

template <typename Num, 
        template <typename> class PresetPolys, 
        template <typename> class Group, 
        template <typename> class Selector,
        template <typename> class Container>
Analytics runner(const Indices& indices, int num_threads){
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
        
    std::vector<std::thread> threads;
    for(int i=0; i<num_threads; i++){
        threads.push_back(std::thread(run<Container<St>>, std::ref(container), allow_reflection));
    }
    
    for(auto& t: threads)
        t.join();

    container.analytics.clock_end();
    auto uniques = find_uniques_brute<St, SymmetryGroup::D4<Poly>>(container.get_solutions());
    container.analytics.total_final = uniques.size();

    return container.analytics;
}

int main([[maybe_unused]] int argc, char** argv){

    using Num = Float<double>;
    int num_threads = atoi(argv[1]);
    Indices indices = {};

    auto run1 = runner<Num, Ostomid, SymmetryGroup::Id, SelectLeftest, Stack>(indices, num_threads);
    run1.print();
}