#include <fstream>
#include <iostream>
#include <thread>
#include <unordered_set>
#include <vector>
#include <ostream>
#include <thread>
#include <mutex>
#include <optional>
#include "state.cpp"
#include "customFloat.cpp"
#include <chrono>
#include "graphics.cpp"
#include "ArchiveQueue.cpp"

using Num = Float<double>;
using Inner = InnerState<Num, true, true>;
using Stat = State<Num, Inner>;

template <typename Archive, typename Queue>
void process(Archive& archive, 
             Queue& queue,
             bool allow_reflection){

    // double mutex_wait{0}, process_time{0};
    while(true){
        // std::cout << std::this_thread::get_id() << "\n";
        
        auto maybe_state = queue.get_next_pointer();
            
        if(not maybe_state){
            // std::cout << "." << std::endl;
            // threads_status.at(thread_index) = false;
            // mtx2.lock();
            bool all_stopped = true;
            // for(auto running: threads_status){
            //     if(running){
            //         all_stopped = false;
            //         break;
            //     }
            // }
            // mtx2.unlock();

            if(all_stopped)
                break;
            // else{
            //     std::this_thread::sleep_for(std::chrono::milliseconds(30));
            //     continue;
            // }
        }

        // mtx2.lock();
        // threads_status.at(thread_index) = true;
        // mtx2.unlock();
        
        auto next_states = maybe_state->find_next_states(allow_reflection);
        std::vector<bool> mask = archive.insert(next_states);
        queue.insert(next_states, mask);
    }
}

int main(){


    bool allow_reflection = true;
    Stat first_state;
    first_state = first_state.iterate({68, 28, 49, 1, 0}, allow_reflection);
    std::vector<Stat> next_states{first_state};
    // auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 3, 9, 2, 0, 0, 0, 0, 0}; no reflection

    Archive<Inner> archive;
    Queue<Stat> queue;
    
    std::vector<bool> mask = archive.insert(next_states);
    queue.insert(next_states, mask);

    process(archive, queue, allow_reflection);
    

    // std::vector<std::thread> threads;
    // int num_threads = 1;
    // std::vector<bool> status;

    // for (int i = 0; i < num_threads; i++) 
    //     status.push_back(false);

    // for (int i = 0; i < num_threads; i++) {
    //     threads.push_back(std::thread([&archive, &next_states, &status, &allow_reflection, i](){
    //         process(archive, next_states, status, i, allow_reflection);
    //     }));
    // }

    // for (auto& t : threads) {
    //     t.join();
    // }


    auto completes = archive.find_completes();
    auto uniques = archive.find_uniques(completes);
    std::cout << "number of states: " << archive.size() << "\n";
    std::cout << "complete: " << completes.size() << "\n";
    std::cout << "unique: " << uniques.size() << "\n";
    
    // Graphics graphics;
    // for(auto& state: uniques){
    //     auto used_polys = used_polys_to_vector(state);
    //     auto frame = Stat{}.frame_as_vector();
    //     graphics.draw_state(used_polys, frame);
    // }
}