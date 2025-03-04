// #pragma once


#include <thread>
#include <gmpxx.h>
#include <random>
#include "state.cpp"
#include "customFloat.cpp"
#include "ArchiveQueue.cpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>



// TEST_CASE("Float<double>", "[custom]"){
//     using U = Float<double>;
//     using I = InnerState<U, true, true>;
//     bool allow_reflections = false;
    
//     State<U, I> stateU;

//     auto indices = std::vector<unsigned>{10,  4,  4,  0, 22, 11, 3,  9,  2, 0, 0, 1, 0, 0};
//     auto results = std::vector<unsigned>{35, 14, 16, 18, 24, 12, 6, 10, 10, 1, 5, 5, 1, 1};
    

//     for(unsigned i=0; i<indices.size(); i++){
//         auto next_statesU = stateU.find_next_states(allow_reflections);
//         REQUIRE(next_statesU.size() == results.at(i));
//         stateU = next_statesU.at(indices.at(i));
//     }
    
// }




// TEST_CASE("Several known solutions Float<double>", "[custom]"){
//     using U = Float<double>;
//     using I = InnerState<U, true, true>;
//     bool allow_reflections = true;
    
//     std::vector<std::vector<unsigned>> solutions{
//         {68, 28, 49,  1,  0, 37, 11,  1, 14,  2,  1,  0,  0,  0},
//         {68, 28, 23,  9, 20, 14,  0,  2, 15,  1,  9,  2,  0,  0},
//         {50, 33, 12, 47,  0, 11, 15,  5, 15,  9,  0,  3,  1,  0},
//         {37, 19, 58,  9, 11, 11,  5, 10,  5,  0,  2,  3,  0,  0},
//         { 6,  2, 56, 16, 28, 36, 11,  1,  9,  6,  0,  0,  1,  0},
//         {21,  8,  6, 17, 47, 23, 25,  3, 10,  8,  2,  0,  0,  0}};

//     for(auto& solution: solutions){
//         State<U, I> stateU;
        
//         for(auto& index: solution){
//             auto next_statesU = stateU.find_next_states(allow_reflections);
//             stateU = next_statesU.at(index);
//         }

//         REQUIRE(stateU.current_polygon->size_ll == 0);
//     }
    
// }



// TEST_CASE("Float<double> = mpz_class", "[custom]"){
//     using T = Number<mpz_class, 2, 5, 13, 17>;
//     using U = Float<double>;
//     using IU = InnerState<U, true, true>;
//     using IT = InnerState<T, true, true>;
//     bool allow_reflections = false;
    
//     State<T, IT> stateT;
//     State<U, IU> stateU;

    
//     auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 3, 9, 2, 0, 0, 1, 0, 0};
    

//     for(unsigned index: indices){
//         auto next_statesT = stateT.find_next_states(allow_reflections);
//         auto next_statesU = stateU.find_next_states(allow_reflections);

//         REQUIRE(next_statesT.size() == next_statesU.size());
//         stateT = next_statesT.at(index);
//         stateU = next_statesU.at(index);
//     }
    
// }

// bool randomized_comparison(unsigned NR){
//     using T = Number<mpz_class, 2, 5, 13, 17>;
//     using U = Float<double>;
//     using IT = InnerState<T, true, true>;
//     using IU = InnerState<U, true, true>;
//     bool allow_reflections = false;
    
//     // Initialize the random number generators
//     std::random_device rd;
//     std::mt19937 gen(rd());
    
//     for(unsigned i=0; i<NR; i++){
//         State<T, IT> stateT;
//         State<U, IU> stateU;

//         while(true){

//             auto next_statesT = stateT.find_next_states(allow_reflections);
//             auto next_statesU = stateU.find_next_states(allow_reflections);
//             if(next_statesT.size() != next_statesU.size())
//                 return false;

//             if(next_statesT.size() == 0)
//                 break;

//             std::uniform_int_distribution<> distr(0, next_statesT.size()-1);
//             unsigned next_index = static_cast<unsigned>(distr(gen));
//             stateT = next_statesT.at(next_index);
//             stateU = next_statesU.at(next_index);
//         }
//     }

//     return true;
// }

// TEST_CASE("Float<double> = mpz_class randomized", "[custom]"){
//     REQUIRE(randomized_comparison(10));
// }


// template <typename Num, typename Inner>
// Archive<Inner> process(bool allow_reflection){

//     State<Num, Inner> first_state;
//     Archive<Inner> archive;
//     Queue<State<Num, Inner>> queue;
// // 68, 28, 49,  1,  0, 37, 11,  1, 14,  2,  1,  0,  0,  0},
//     first_state = first_state.iterate({68, 28, 49, 1, 0, 37}, allow_reflection);
//     std::vector<State<Num, Inner>> next_states{first_state};
//     std::vector<bool> mask = archive.insert(next_states);
//     queue.insert(next_states, mask);


//     while(true){
        
//         auto maybe_state = queue.get_next_pointer();
            
//         if(not maybe_state)
//             break;
        
//         auto next_states = maybe_state->find_next_states(allow_reflection);
//         std::vector<bool> mask = archive.insert(next_states);
//         queue.insert(next_states, mask);
//     }

//     return archive;
// }

// template <typename T, typename U>
// bool check_if_identical(const std::vector<T>& vector1, const std::vector<U>& vector2){

//     if(vector1.size() != vector2.size())
//         return false;

//     for(auto& state1: vector1){
//         unsigned count = 0;
//         for(auto& state2: vector2){
//             if(state2.are_polys_same(state1))
//                 count++;
//         }

//         if(count != 1)
//             return false;
        
//     }

//     return true;

// }

// TEST_CASE("archive", "[custom]"){
//     using Num = Float<double>;
//     using Inner = InnerState<Num, true, true>;
//     using Inner_NoHash = InnerState<Num, false, true>;
//     using Inner_NoHashComp = InnerState<Num, false, false>;
//     bool allow_reflection = true;

//     auto archive = process<Num, Inner>(allow_reflection);
//     auto archive_nh = process<Num, Inner_NoHash>(allow_reflection);
//     auto archive_nhc = process<Num, Inner_NoHashComp>(allow_reflection);

//     // std::cout << "size: " << archive.size() << "\n";
//     // std::cout << "no hash: " << archive_nh.size() << "\n";
//     // std::cout << "no hash comp: " << archive_nhc.size() << "\n";

//     for(unsigned i=1; i < polygons<Float<double>>::num_polygons; i++){
//         auto s1 = archive.find_states_with_size(i);
//         auto s2 = archive_nh.find_states_with_size(i);
//         auto s3 = archive_nhc.find_states_with_size(i);


//         auto u1 = archive.find_uniques(s1);
//         auto u2 = archive_nh.find_uniques(s2);
//         auto u3 = archive_nhc.find_uniques(s3);

//         // std::cout << "filtered:" << s1.size() << " " << s2.size() << " " << s3.size() << "     ";
//         // std::cout << "uniqued:" << u1.size() << " " << u2.size() << " " << u3.size() << "\n";

//         bool b1 = check_if_identical(u1, u2);
//         bool b2 = check_if_identical(u1, u3);

//         REQUIRE(b1);
//         REQUIRE(b2);

//     }

// }

std::mutex mtx2;

template <typename Archive, typename Queue>
void process(Archive& archive, 
             Queue& queue,
             std::vector<bool>& threads_status,
             unsigned thread_index,
             bool allow_reflection){

    // double mutex_wait{0}, process_time{0};
    while(true){


        auto maybe_state = queue.get_next_pointer();
            
        if(not maybe_state){
            // std::cout << "stopped\n";
            mtx2.lock();
            threads_status.at(thread_index) = false;
            bool all_stopped = true;
            for(auto running: threads_status){
                if(running){
                    all_stopped = false;
                    break;
                }
            }
            mtx2.unlock();

            if(all_stopped)
                break;
            else{
                std::this_thread::sleep_for(std::chrono::milliseconds(30));
                
                continue;
            }
        }

        mtx2.lock();
        threads_status.at(thread_index) = true;
        mtx2.unlock();
        
        auto next_states = maybe_state->find_next_states(allow_reflection);
        std::vector<bool> mask = archive.insert(next_states);
        queue.insert(next_states, mask);
    }
}


TEST_CASE("Multithreading + archive", "[custom]"){
    using Num = Float<double>;
    using Inner = InnerState<Num, true, true>;
    using Stat = State<Num, Inner>;
    bool allow_reflection = true;

    State<Num, Inner> first_state;
    first_state = first_state.iterate({68, 28, 48}, allow_reflection);
    // 68, 28, 49,  1,  0, 37, 11,  1, 14,  2,  1,  0,  0,  0},
    std::vector<Stat> next_states{first_state};

    Archive<Inner> archive;
    Queue<Stat> queue;
    std::vector<bool> mask = archive.insert(next_states);
    queue.insert(next_states, mask);



    std::vector<std::thread> threads;
    int num_threads = 2;
    std::vector<bool> status;

    for (int i = 0; i < num_threads; i++) 
        status.push_back(false);

    for (int i = 0; i < num_threads; i++) {
        threads.push_back(std::thread([&archive, &queue, &status, &allow_reflection, i](){
            process<Archive<Inner>, Queue<Stat>>(archive, queue, status, i, allow_reflection);
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    for(unsigned i=1; i < polygons<Float<double>>::num_polygons; i++){
        auto s1 = archive.find_states_with_size(i);
        // auto u1 = archive.find_uniques(s1);
        std::cout << s1.size() << " "; // 30
    }
    std::cout << "\n";

    auto s1 = archive.find_states_with_size(polygons<Float<double>>::num_polygons);
    auto u1 = archive.find_uniques(s1);
    std::cout << archive.size() << "\n"; // 26222
    std::cout << s1.size() << "\n"; // 30
    
    REQUIRE(true);


}