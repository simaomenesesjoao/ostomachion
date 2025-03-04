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


// using Num = Number<mpz_class, 2, 5, 13, 17>;
using Num = Float<double>;
using Stat = State<Num>;
using Poi = Point<Num>;
using PolyRay = std::vector<std::vector<Poi>>;



namespace std {
    template <>
    struct hash<PolyRay> {
        size_t operator()(const PolyRay& used_polys) const {
            // return 0;

            long h = 0;
            for(unsigned i=0; i<polygons<Num>::num_polygons; i++){
                auto& poly = used_polys.at(i);
                for(auto& point: poly){
                    double x = (double)point.get_x();
                    double y = (double)point.get_y();
                    //std::cout << x << " " << y << "\n";
                    h += (long)(x*100000000) + (long)(y*10000);
                    // if(i==6 or i==7)
                    //     h *= 6+7;
                    // else if(i==5 or i==8)
                    //     h *= 5+8;
                    // else
                    //     h *= (i+1);

                    h = h%100000;
                    // std::cout << hash <<;
                }


            }
            
            return h;
        }
    };

    template<>
    bool operator==(const PolyRay& state1, const PolyRay& state2){

        for(unsigned i=0; i<state1.size(); i++){
            if(i == 5 or i == 6 or i == 7 or i == 8){
                continue;
            }
            if(state1.at(i) != state2.at(i))
                return false;
            
        }

        auto a_list = std::vector<unsigned>{5, 6}; 
        auto b_list = std::vector<unsigned>{8, 7};

        for(unsigned k=0; k<2; k++){
            unsigned a = a_list.at(k);
            unsigned b = b_list.at(k);
            bool b1 = (state1.at(a) == state2.at(a));
            bool b2 = (state1.at(b) == state2.at(b));
            bool b3 = (state1.at(b) == state2.at(a));
            bool b4 = (state1.at(a) == state2.at(b));

            if(!(b1 and b2) and !(b3 and b4))
                return false;
        }


        return true;
    }
}


using Container = std::unordered_set<PolyRay>;
// using Container = std::unordered_set<Stat, Stat::HashFunction>;


std::vector<std::vector<std::vector<double>>> used_polys_to_vector(const PolyRay& used_polys) {
    std::vector<std::vector<std::vector<double>>> vector;
    for(auto& poly: used_polys){
        std::vector<std::vector<double>> pol;
        for(auto& point: poly){
            double x = (double)point.get_x();
            double y = (double)point.get_y();
            std::vector<double> P{x,y};
            pol.push_back(P);
        }
        vector.push_back(pol);

    }
    return vector;
}



std::mutex mtx;

std::optional<Stat> get_next_pointer(std::vector<Stat>& next_states){
    std::lock_guard<std::mutex> lock(mtx);

    if(next_states.size() == 0)
        return std::nullopt;
            
    State last_state = std::move(next_states.back());
    next_states.pop_back();

    return last_state;
}

void add_next_states(Container& archive, 
                     std::vector<Stat>& new_next_states, 
                     std::vector<Stat>& next_states, double& duration){

    auto start_wait = std::chrono::high_resolution_clock::now();
    std::lock_guard<std::mutex> lock(mtx);
    auto end_wait = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> wait_duration = end_wait - start_wait;
    duration += wait_duration.count();

    for(auto& next_state: new_next_states){

        auto [it, success] = archive.insert(*next_state.used_polys);

        if(success)
            next_states.push_back(std::move(next_state));

    }
    
}

void loop(){

    // std::cout << i << "\n";
    threads.push_back(std::thread([&archive, &next_states, &status, &allow_reflection, i](){

        double count = 0;
        while(true){
            // std::cout << std::this_thread::get_id() << "\n";
            
            auto it_next = get_next_pointer(next_states);
                
            
            if(not it_next){
                // std::cout << "." << std::endl;
                status.at(i) = false;
                mtx.lock();
                bool all_stopped = true;
                for(auto running: status){
                    if(running){
                        all_stopped = false;
                        break;
                    }
                }
                mtx.unlock();

                if(all_stopped)
                    break;
                else{
                    std::this_thread::sleep_for(std::chrono::milliseconds(30));
                    continue;

                }
                    
                    
                
            }
            status.at(i) = true;
            
            auto new_next_states = it_next->find_next_states(allow_reflection);
            
            add_next_states(archive, new_next_states, next_states, count);
        }
        std::cout << count << "\n";
    }));

}


int main(){

    bool allow_reflection = true;
    Stat first_state;
    // 68, 28, 23,  9, 20, 14,  0, 2, 15, 1, 9, 2
    // 50, 33, 12, 47,  0, 11, 15, 5, 15, 9, 0, 3
    first_state.iterate({68, 28, 23, 9, 20, 14, 0, 2, 15, 1, 9}, allow_reflection);
    // auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 3, 9, 2, 0, 0, 0, 0, 0};

    Container archive;
    // archive.reserve(100000);    
    archive.insert(*first_state.used_polys);
    std::vector<Stat> next_states{std::move(first_state)};

    std::vector<std::thread> threads;
    int num_threads = 16;
    std::vector<bool> status;
    for (int i = 0; i < num_threads; i++) 
        status.push_back(false);

    for (int i = 0; i < num_threads; i++) {
        
        // std::cout << i << "\n";
        threads.push_back(std::thread([&archive, &next_states, &status, &allow_reflection, i](){

            double count = 0;
            while(true){
                // std::cout << std::this_thread::get_id() << "\n";
                
                auto it_next = get_next_pointer(next_states);
                 
                
                if(not it_next){
                    // std::cout << "." << std::endl;
                    status.at(i) = false;
                    mtx.lock();
                    bool all_stopped = true;
                    for(auto running: status){
                        if(running){
                            all_stopped = false;
                            break;
                        }
                    }
                    mtx.unlock();

                    if(all_stopped)
                        break;
                    else{
                        std::this_thread::sleep_for(std::chrono::milliseconds(30));
                        continue;

                    }
                        
                        
                    
                }
                status.at(i) = true;
                
                auto new_next_states = it_next->find_next_states(allow_reflection);
                
                add_next_states(archive, new_next_states, next_states, count);
            }
            std::cout << count << "\n";
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    // std::vector<long> hashes;
    // std::cout << "archive:\n";
    // unsigned complete_states = 0;
    std::vector<PolyRay> completes;
    for(auto& state: archive){
        // auto used_polys = state.used_polys_as_vector();
        bool complete = true;
        for(auto& poly: state){
            
            if(poly.size() == 0){
                complete = false;
                break;
            }
            
        }

        if(complete){
            // complete_states++;
            completes.push_back(state);
            
        }


        // hashes.push_back(state.state_hash);

        // if(state.current_polygon.size_ll < 3){
        //     std::cout << state << "\n";
        //     // graphics.draw_state(used_polys, frame);
        

        // }
        
    }

    std::cout << "complete: " << completes.size() << "\n";
    std::cout << "number of states: " << archive.size() << "\n";
    Stat aa;
    Graphics graphics;
    // unsigned i=0;
    for(auto& state: completes){
        // std::cout << "new state" << i++ << ":\n";
        // unsigned ii = 0;
        // for(auto& poly: state){
        //     std::cout << ii << " ";
        //     ii++;
            
        //     for(auto& point: poly){
        //         std::cout << point << " ";
        //     }
        //     std::cout << "\n";
        // }
        auto used_polys = used_polys_to_vector(state);
        auto frame = aa.frame_as_vector();   
        graphics.draw_state(used_polys, frame);
    }

    // for(unsigned i=0; i<completes.size(); i++){
    //     auto& state1 = completes.at(i);
    //     for(unsigned j=0; j<completes.size(); j++){
    //         auto& state2 = completes.at(j);
    //         std::cout << "(i,j):" << i << " " << j << " - " << (state2 == state1) << "\n";
    //     }
    // }

    // std::cout << "these should be the same: " << (completes.at(0) == completes.at(1)) << "\n";

    // std::unordered_set<int> unique_numbers(hashes.begin(), hashes.end());
    // std::cout << "number of hashes: " << unique_numbers.size() << "\n";
}