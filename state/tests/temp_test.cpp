#include <thread>
#include <gmpxx.h>
#include <random>
#include "state.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "ArchiveQueue.cpp"
#include <mutex>
#include "../../solver/src/graphics.cpp"

// std::mutex mtx2;

// template <typename Archive>
// void process_level(Archive& archive_in, 
//              Archive& archive_out,
//              std::ostream& stream, 
//              Getter& getter,
//              std::vector<bool>& threads_status, 
//              int thread_index){

//     bool allow_reflection = true;
//     while(true){

//         auto maybe_state = archive_in.get_next_pointer();
        
//         if(not maybe_state){
//             mtx2.lock();
//             threads_status.at(thread_index) = false;
//             bool all_stopped = true;
//             for(auto running: threads_status){
//                 if(running){
//                     all_stopped = false;
//                     break;
//                 }
//             }
//             mtx2.unlock();

//             if(all_stopped)
//                 break;
//             else{
//                 std::this_thread::sleep_for(std::chrono::milliseconds(30));
                
//                 continue;
//             }
//         }

//         mtx2.lock();
//         threads_status.at(thread_index) = true;
//         mtx2.unlock();
        
        
//         auto next_states = maybe_state->find_next_states(allow_reflection, getter, stream);
//         // std::cout << "next states " << next_states.size() << "\n";;   
//         archive_out.insert(next_states);
//     }
// }

// template <typename Num, bool hash, bool comp>
// Archive<State<Num, InnerState<Num, hash, comp>,SelectLeftest>> get_(std::vector<unsigned int> indices, Getter& getter, unsigned int num_threads, std::ostream& stream){

//     // Definitions
//     using Inner = InnerState<Num, hash, comp>;
//     using Stat = State<Num, Inner, Selec>;
//     bool allow_reflection = true;

//     // Initialization    
//     State<Num, Inner> first_state;
//     std::vector<Stat> next_states{first_state.iterate(indices, allow_reflection, getter, stream)};
//     Archive<Stat> archive_in, archive_out;
//     // Queue<Stat> queue_input, queue_output;

//     archive_in.insert(next_states);
//     // std::vector<bool> mask = archive.insert(next_states);
//     // queue_input.insert(next_states, mask);
//     std::vector<std::thread> threads;
//     std::vector<bool> thread_status;

//     std::cout << "initial size: " << archive_in.size() << "\n";
    
//     // for(int i=0; i<num_threads;i++)
//     for(unsigned i=indices.size(); i<14; i++){
//         process_level<Archive<Stat>>(archive_in, archive_out, stream, getter, thread_status, 0);
//         archive_in.archive = std::move(archive_out.archive);
//         // queue_input.queue = queue_output.queue;
//         // queue_output.reset();
//         std::cout << "archive sizes: " << archive_in.size() << "\n";
        
//     }


//     return archive_out;
// }

template <typename State, template <typename> class Container>
Container<State> find_all(const std::vector<std::tuple<unsigned int, unsigned int, bool>>& indices){
    State first_state;
    bool allow_reflection = true;
    std::ofstream stream("/dev/null");
    std::vector<State> next_states{first_state.apply_iterations(indices, stream)};

    std::cout << next_states.at(0) << "\n";
    Container<State> container;
    container.insert(next_states);

    while(true){
        auto state = container.get_next();
        if(!state)
            break;
        auto next = state->find_next_states(allow_reflection, stream);
        // std::cout << next.size();
        container.insert(next);

    }
    return container;

}

template <typename T>
std::vector<T> find_states_with_size(const std::vector<T>& container, unsigned int i){
    
    std::vector<T> states;
    for(auto& state: container){
        if(state.size() == i){
            states.push_back(state);
        }
    }
    return states;
}


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

template <typename T>
std::vector<T> find_uniques_hash(const std::vector<T>& container){


    struct HashStruct {
        size_t operator()(const T& x) const {
            return x.get_hash();
        }
    };

    std::unordered_set<T, HashStruct> uniques_hash;
    for(auto& state: container){
        uniques_hash.insert(state);
    }

    std::vector<T> uniques;
    for(auto& state: uniques_hash){
        uniques.insert(state);
    }
    return uniques;
}

template <typename T, typename U>
bool sets_are_equal(const std::vector<T>& set1, const std::vector<U>& set2){
    if(set1.size() != set2.size())
        return false;

    for(auto& s1: set1){
        bool found = false;
        for(auto& s2: set2){
            if(*(s1.used_polys) == *(s2.used_polys))
                found = true;
        }
        if(!found)
            return false;
    }
    return true;
}

int main(int argc, char** argv){
    
    int index = 0;
    if(argc>0)
        index = atoi(argv[1]);
    std::cout << "argc: " << argc << " index: " << index << "\n";

    std::vector<std::tuple<unsigned int, unsigned int, bool>> indices;
    // indices = {{4,0,0}, {3,0,0}, {0,0,0}, {5,0,0}, {1,2,0}, {2,0,0}, {6,0,0}};
    indices = {{4,0,0}, {3,0,0}, {0,0,0}};

    using Num = Float<double>;
    using Inner = InnerState<Num, true, true>;    
    // using st1 = State<Num, Inner, SelectObtusest, GetFirst>;
    // using st2 = State<Num, Inner, SelectObtusest, GetLast>;
    using st2 = State<Num, Inner, SelectObtusest, GetLast>;
    using st3 = State<Num, Inner, SelectLeftest, GetLast>;
    using st4 = State<Num, Inner, SelectLeftest, GetLast>;

    // auto res1 = find_all<st1>(indices);
    // auto res2 = find_all<st2>(indices);
    auto re2 = find_all<st2, Hash>(indices);
    auto re3 = find_all<st3, Hash>(indices);
    auto re4 = find_all<st4, Stack>(indices);

    auto res2 = re2.get_solutions();
    auto res3 = re3.get_solutions();
    auto res4 = re4.get_solutions();
    // for(auto& r: res4.final_container){
    //     std::cout << r << "\n";
    // }


    std::cout << "complete: " << res2.size() << " " << res3.size() << " " << res4.size() << "\n";
    auto v2 = find_uniques_brute(res2);
    auto v3 = find_uniques_brute(res3);
    auto v4 = find_uniques_brute(res4);

    std::cout << "uniques: " << v2.size() << " " << v3.size() << " " << v4.size() << "\n";

    std::cout << sets_are_equal(v2, v3) << std::endl;
    std::cout << sets_are_equal(v3, v4) << std::endl;

    re2.print_analytics();
    re3.print_analytics();
    re4.print_analytics();

    
}