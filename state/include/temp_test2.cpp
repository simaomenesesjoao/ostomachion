#include <thread>
#include <gmpxx.h>
#include <random>
#include "state.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "ArchiveQueue.cpp"
#include <mutex>
#include "../../solver/src/graphics.cpp"

// namespace std{
//     bool operator==(const std::vector<std::vector<double>>& v1, const std::vector<std::vector<double>>& v2){
//         for(auto& P1: v1){
//             for(auto& P2: v2){
//                 double tol = 1e-6;
//                 if(std::abs(P1.at(0) - P2.at(0)) > tol or std::abs(P1.at(1) - P2.at(1)) > tol)
//                     return false;
//             }
//         }

//         return true;
//     }
// }

std::mutex mtx2;

template <typename Archive, typename Queue>
void process(Archive& archive, 
             Queue& queue, std::ostream& stream){//, std::vector<bool>& threads_status, int thread_index){

    bool allow_reflection = true;
    // double mutex_wait{0}, process_time{0};
    while(true){


        auto maybe_state = queue.get_next_pointer(0);
        if(not maybe_state)
            break;

        // if(not maybe_state){
        //     // std::cout << "stopped\n";
        //     mtx2.lock();
        //     threads_status.at(thread_index) = false;
        //     bool all_stopped = true;
        //     for(auto running: threads_status){
        //         if(running){
        //             all_stopped = false;
        //             break;
        //         }
        //     }
        //     mtx2.unlock();

        //     if(all_stopped)
        //         break;
        //     else{
        //         std::this_thread::sleep_for(std::chrono::milliseconds(30));
                
        //         continue;
        //     }
        // }

        // mtx2.lock();
        // threads_status.at(thread_index) = true;
        // mtx2.unlock();
        
        // Graphics graphics;
        
        stream << "count: " << maybe_state->countme << "\n";
        maybe_state->countme++;
        auto next_states = maybe_state->find_next_states(allow_reflection, stream);

        
        // auto used_polys = maybe_state->used_polys->used_polys_as_vector();
        // graphics.draw_state(used_polys, maybe_state->current_polygon->as_vector());
        // std::cout << "gave rise to\n";
        std::vector<bool> mask = archive.insert(next_states);
        // for(int i=0; i<next_states.size(); i++){
        //     if(!mask.at(i))
        //         std::cout << "      already exists\n";
        //     else
        //         std::cout << "      doesn't exist yet\n";

        //     std::cout << next_states.at(i) << "\n";
        // }
        queue.insert(next_states, mask);
    }
}

template <typename Num, bool hash, bool comp>
Archive<State<Num, InnerState<Num, hash, comp>>> get_(std::vector<unsigned int> indices, unsigned int num_threads, std::ostream& stream){
    using Inner = InnerState<Num, hash, comp>;
    using Stat = State<Num, Inner>;
    bool allow_reflection = true;

    State<Num, Inner> first_state;
    first_state = first_state.iterate(indices, allow_reflection);
    std::vector<Stat> next_states{first_state};

    Archive<Stat> archive;
    Queue<Stat> queue;
    std::vector<bool> mask = archive.insert(next_states);
    queue.insert(next_states, mask);
    std::vector<bool> status;


    process<Archive<Stat>, Queue<Stat>>(archive, queue, stream);
    // std::vector<std::thread> threads;

    // for(int i=0; i<num_threads; i++){
    //     status.push_back(false);
    // }

    // for(int i=0; i<num_threads; i++){
    //     threads.push_back(std::thread([&archive, &queue, &status, i](){
    //         process<Archive<Inner>, Queue<Stat>>(archive, queue, status, i);
    //     }));
    // }

    // for (auto& t : threads) {
    //     t.join();
    // }

    return archive;
}

int main(){
    
    // int index = 0;
    // if(argc>0)
    //     index = atoi(argv[1]);
    // std::cout << "argc: " << argc << " index: " << index << "\n";
    std::vector<unsigned int> indices{68, 28, 49, 1, 0, 37, 11, 1,14};
    // 68, 28, 49,  1,  0, 37, 11,  1, 14,  2,  1,  0,  0,  0},
    using Num1 = Float<double>;
    // using Num1 = Number<mpz_class, 2, 5, 13, 17>;
    std::ofstream file1, file2, file3, file4;
    file1.open("out1");
    file2.open("out2");
    file3.open("out3");
    file4.open("out4");
    
    auto archive1 = get_<Num1, false, true>(indices, 1, file1);
    auto archive2 = get_<Num1, false, true>(indices, 1, file2);
    auto archive3 = get_<Num1, false, true>(indices, 1, file3);
    auto archive4 = get_<Num1, false, true>(indices, 1, file4);

    for(unsigned int i = 0; i < polygons<Float<double>>::num_polygons; i++){
        auto s1 = archive1.find_states_with_size(i);
        auto s2 = archive2.find_states_with_size(i);
        auto s3 = archive3.find_states_with_size(i);
        auto s4 = archive4.find_states_with_size(i);
        std::cout << i << " " << s1.size() << " " << s2.size() << " " << s3.size() << " " << s4.size() << "\n";
    }

    file1.close();
    file2.close();
    file3.close();
    file4.close();

    // unsigned int ii = 8;
    // auto s1 = archive1.find_states_with_size(ii);
    // auto s2 = archive2.find_states_with_size(ii);

    
    // std::cout << s1.at(0) << "\n";
    // std::cout << s2.at(0) << "\n";
    // Graphics graphics;
    // auto used_polys1 = s1.at(0).used_polys_as_vector();
    // graphics.draw_state(used_polys, first_state.current_polygon->as_vector());
    // auto used_polys2 = s.used_polys_as_vector();
    // graphics.draw_state(used_polys, first_state.current_polygon->as_vector());

    // std::vector<unsigned int> original1, original2;
    // for(unsigned int i = 0; i < s1.size(); i++){
    //     auto v1 = s1.at(i).used_polys_as_vector();
    //     bool found = false;
    //     unsigned int found_index = 99999;
    //     for(unsigned int j = 0; j < s2.size(); j++){
    //         auto v2 = s2.at(j).used_polys_as_vector();
    //         bool b = (v1 == v2);
    //         // if(b){
    //         //     found = true;
    //         //     found_index = j;
    //         //     break;
    //         // }
    //         std::cout << b << "" ;
    //     }
    //     if(!found){
    //         // original1.push_back()
    //     }
    //     std::cout << "\n";
    // }

    // Graphics graphics;
    // for(unsigned i=6; i < polygons<Float<double>>::num_polygons; i++){
        

    //     auto s1 = archive.find_states_with_size(i);

        // std::cout << "comparing hash\n";
        // for(unsigned int i = 0; i<s1.size(); i++){
        //     for(unsigned int j = 0; j<s1.size(); j++){
        //         std::cout << (s1.at(i).get_hash() == s1.at(j).get_hash()) << " ";
        //     }
        //     std::cout << "\n";
        // }

        
        // std::cout << "comparing equality\n";
        // for(unsigned int i = 0; i<s1.size(); i++){
        //     for(unsigned int j = 0; j<s1.size(); j++){
        //         std::cout << (s1.at(i) == s1.at(j)) << " ";
        //     }
        //     std::cout << "\n";
        // }

        // std::cout << "equal but with different hash\n";
        // int ii{-1}, jj{-1};
        // for(unsigned int i = 0; i<s1.size(); i++){
        //     bool found = false;
        //     for(unsigned int j = 0; j<s1.size(); j++){
        //         if((s1.at(i) == s1.at(j)) and s1.at(i).get_hash() != s1.at(j).get_hash()){
        //             found = true;
        //             ii = i;
        //             jj = j;
        //             break;
        //         }
        //     }
        //     if(found)
        //         break;
        //     std::cout << "\n";
        // }

        // if(ii >= 0 and jj >= 0){
        //     std::cout << "These have different hash but are identical\n";
        //     std::cout << s1.at(ii).get_hash() << "\n" << s1.at(ii) << "\n";
        //     std::cout << s1.at(jj).get_hash() << "\n" << s1.at(jj) << "\n";
        // }
        
        // for(auto kk: std::vector<unsigned int>{ii, jj}){
        //     auto s = s1.at(kk);
        //     auto used_polys = s.used_polys_as_vector();
        //     graphics.draw_state(used_polys, first_state.current_polygon->as_vector());
        // }

    //     auto u1 = archive.find_uniques(s1);
    //     std::cout << "size " << i << ": " << s1.size() << " " << u1.size() << "\n"; // 30

    // }
    // std::cout << "\n";

    // auto s1 = archive.find_states_with_size(polygons<Float<double>>::num_polygons);
    // auto u1 = archive.find_uniques(s1);
    // std::cout << archive.size() << "\n"; // 26222
    // std::cout << s1.size() << "\n"; // 30
    


}