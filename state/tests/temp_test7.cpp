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

            if(state2->used_polys->equals_under_symmetry(*state1->used_polys, G::transformations)){
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

        auto next_states = (*maybe_state)->find_next_states(allow_reflection, stream);            
        container.insert(next_states);
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
    using Cont = Container<std::shared_ptr<St>>;

    St first_state(std::make_shared<OverlapConvex<Poly>>());
    bool allow_reflection = true;
    std::ofstream stream("/dev/null");

    std::vector<std::shared_ptr<St>> next_states{first_state.apply_iterations(indices, stream)};

    Cont container;
    container.analytics.info += std::format("{:>3} {:>6} {:>10}", 
        Group<Poly>::name, Container<St>::name, Selector<Num>::name);

    container.insert(next_states);
    container.analytics.clock_start();
        
    std::vector<std::thread> threads;
    for(int i=0; i<num_threads; i++){
        threads.push_back(std::thread(run<Cont>, std::ref(container), allow_reflection));
    }
    
    for(auto& t: threads)
        t.join();

    container.analytics.clock_end();
    auto uniques = find_uniques_brute<std::shared_ptr<St>, SymmetryGroup::D4<Poly>>(container.get_solutions());
    container.analytics.total_final = uniques.size();

    return container.analytics;
}



















// template <typename Num>
// bool are_separate_convex_vector(
//     const std::vector<Node<Num>>& poly1, const std::vector<Node<Num>>& poly2) {
//     // Only strictly valid for convex polygons

//     for(unsigned int i=0; i<poly1.size(); i++){
//         const Node<Num>& current = poly1.at(i);
//         const Node<Num>& next = (i==poly1.size()-1) ? poly1.at(0): poly1.at(i+1);

//         const Point<Num>& C = current.position;
//         const Point<Num>& N = next.position;

//         Num dx = N.get_x() - C.get_x();
//         Num dy = N.get_y() - C.get_y();
        
//         bool separated = true;
//         for(unsigned int j=0; j<poly2.size(); j++){

//             Point<Num> P = poly2.at(j).position - C;

//             // (dy, -dx) is (dx, dy) rotated 90º clockwise, 
//             // so it faces away from the polygon
//             Num proj = P.get_x()*dy - P.get_y()*dx;
//             if(proj < 0){
//                 separated = false;
//                 break;
//             }

//         }
        
//         if(separated)
//             return true;

//     }
//     return false;
// }


using PolyV = std::vector<std::pair<double, double>>;
bool are_separate_convex_vector(const PolyV& poly1, const PolyV& poly2) {
    // Only strictly valid for convex polygons


    // int count = 0;
    unsigned int S = poly1.size();
    for(unsigned int i=0; i<S; i++){
        const auto& C = poly1[i];
        const auto& N = poly1[(i+1)%S];

        double dx = N.first - C.first;
        double dy = N.second - C.second;
        
        bool separated = true;
        for(const auto& P: poly2){

            // (dy, -dx) is (dx, dy) rotated 90º clockwise, 
            // so it faces away from the polygon
            double proj = (P.first - C.first)*dy - (P.second - C.second)*dx;
            // count++;
            if(proj < 0){
                separated = false;
                break;
            }

        }
        
        if(separated)
            return true;

    }
    // std::cout << count << "\n";
    return false;
}

bool convex_overlaps_vector(const PolyV& poly1, const PolyV& poly2) {
    return !(are_separate_convex_vector(poly1, poly2) or are_separate_convex_vector(poly2, poly1));
}


int main([[maybe_unused]] int argc, char** argv){

// {

//     using Num = Float<double>;
//     using Poly = Polygon<Num>;
//     const Poly& poly1 = Ostomid<Poly>::pre_polys.at(0).first;
//     const Poly& poly2 = Ostomid<Poly>::pre_polys.at(1).first;
//     // const Poly poly2{{{}, {}, {}}};
//     std::cout << "overlap? " << poly1.convex_overlaps(poly2) << "\n";

// }

    // using Num = Float<double>;
    
    // // SIMAO temporary test
    // // int num_threads = 1;
    // int num_threads = atoi(argv[1]);
    // Indices indices = {};

    // auto run1 = runner<Num, Ostomachion, SymmetryGroup::Id, SelectLeftest, Stack>(indices, num_threads);
    // run1.print();

    {
        using Num = Float<double>;
        using Poly = Polygon<Num>;
        using Inner = PolySet<Poly, SymmetryGroup::Id, Ostomachion>;
        using St = State<Num, Inner, SelectLeftest, GetLast>;

        // const Poly& poly1 = Ostomid<Poly>::pre_polys.at(0).first;
        const Poly& poly2 = Ostomachion<Poly>::pre_polys.at(1).first;

        std::ofstream stream("/dev/null");
        Indices indices{{{0,0,0}, {3,0,0}, {4,0,0}, {5,0,0}, {2,0,0}}};

        St state(std::make_shared<OverlapConvex<Poly>>());
        state = *state.apply_iterations(indices, stream);
        std::cout << state << "\n";
        const auto& polys = state.used_polys->get_all_polys();







        auto s1 = std::chrono::high_resolution_clock::now();
        int num_overlaps = 0;
        unsigned M = 1000000;
        for(int i=0; i<M; i++){
            for(const auto& polyrow: polys){
                for(const auto& poly: polyrow){
                    num_overlaps += poly2.convex_overlaps(poly);
                }
            }
        }

        auto duration = std::chrono::high_resolution_clock::now() - s1;
        std::cout << num_overlaps << " duration: " << duration.count()/1000000 << "ms\n";



        // Same thing, but with vectors instead of LL
        PolyV poly2v = poly2.as_T();

        std::vector<std::vector<PolyV>> polysv;
        for(const auto& polyrow: polys){
            std::vector<PolyV> row;
            for(const auto& poly: polyrow){
                row.push_back(poly.as_T());
            }
            polysv.push_back(row);
        }

        // std::cout << "poly2v\n";
        // for(auto& p: poly2v)
        //     std::cout << p << "\n";


        // std::cout << "polysv\n";
        // for(auto& row: polysv)
        //     for(auto& pol: row){
        //         for(auto& p: pol)
        //             std::cout << p << "\n";

        //         std::cout << "\n";
        //     }
                
        auto s3 = std::chrono::high_resolution_clock::now();
        num_overlaps = 0;
        for(int i=0; i<M; i++){
            for(const auto& polyrow: polysv){
                for(const auto& poly: polyrow){
                    num_overlaps += convex_overlaps_vector(poly2v, poly);
                }
            }
        }

        auto duration2 = std::chrono::high_resolution_clock::now() - s3;
        std::cout << num_overlaps << " duration: " << duration2.count()/1000000 << "ms\n";


    }
}