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

template <typename State, typename Container>
Container find_all(const std::vector<std::tuple<unsigned int, unsigned int, bool>>& indices){
    State first_state;
    bool allow_reflection = true;
    std::ofstream stream("/dev/null");
    std::vector<State> next_states{first_state.apply_iterations(indices, stream)};

    std::cout << next_states.at(0) << "\n";
    Container container;
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


template <typename Container>
std::vector<typename Container::DataType> find_uniques(const Container& container){

    std::vector<typename Container::DataType> uniques;

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

int main(int argc, char** argv){
    
    int index = 0;
    if(argc>0)
        index = atoi(argv[1]);
    std::cout << "argc: " << argc << " index: " << index << "\n";

    // std::vector<std::tuple<unsigned int, unsigned int, bool>> indices{
    //     {4,0,0}, {3,0,0}, {0,0,0}, {5,0,0}, {1,2,0}, {2,0,0}, {6,0,0}};


    std::vector<std::tuple<unsigned int, unsigned int, bool>> indices{};
    using Num = Float<double>;
    using Inner = InnerState<Num, true, true>;
    // using st1 = State<Num, Inner, SelectObtusest, GetFirst>;
    // using st2 = State<Num, Inner, SelectObtusest, GetLast>;
    // using st3 = State<Num, Inner, SelectLeftest, GetFirst>;
    using st4 = State<Num, Inner, SelectLeftest, GetLast>;
    using ct4 = Stack<st4>;

    


    // auto res1 = find_all<st1>(indices);
    // auto res2 = find_all<st2>(indices);
    // auto res3 = find_all<st3>(indices);
    auto res4 = find_all<st4, ct4>(indices).final_container;
    // for(auto& r: res4.final_container){
    //     std::cout << r << "\n";
    // }


    // std::cout <<
    for(unsigned int i=0; i<polygons<Float<double>>::num_polygons+1; i++){
        auto s1 = find_states_with_size(res4, i);
        std::cout << i << " " << s1.size() << "\n";

    }
    
    // auto s1 = archive2.find_states_with_size(polygons<Float<double>>::num_polygons);
    // auto s2 = archive2.find_states_with_size(polygons<Float<double>>::num_polygons);
    // std::cout << "final: " << s1.size() << " " << s2.size() << "\n";
}