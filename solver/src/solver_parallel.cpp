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


using Num = Number<mpz_class, 2, 5, 13, 17>;
// using Num = Float<double>;
using Stat = State<Num>;
using Poi = Point<Num>;
using PolyRay = std::vector<std::vector<Poi>>;



namespace std {
    template <>
    struct hash<PolyRay> {
        size_t operator()(const PolyRay& used_polys) const {

            long h = 0;
            for(unsigned i=0; i<polygons<Num>::num_polygons; i++){
                auto& poly = used_polys.at(i);
                for(auto& point: poly){
                    double x = (double)point.get_x();
                    double y = (double)point.get_y();
                    //std::cout << x << " " << y << "\n";
                    h += (long)(x*100000000) + (long)(y*10000);
                    if(i==6 or i==7)
                        h *= 6+7;
                    else if(i==5 or i==8)
                        h *= 5+8;
                    else
                        h *= (i+1);

                    h = h%100000;
                    // std::cout << hash <<;
                }


            }
            
            return h;
        }
    };
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

    // auto id = std::this_thread::get_id();


    auto start_wait = std::chrono::high_resolution_clock::now();
    std::lock_guard<std::mutex> lock(mtx);
    auto end_wait = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> wait_duration = end_wait - start_wait;
    duration += wait_duration.count();

    for(auto& next_state: new_next_states){
        //std::cout << "hash: " << next_state.state_hash << "\n";

        // std::cout << "thread " << id << " next states found: ";
        // for(auto& n: new_next_states){
        //     std::cout << n.get_num() << " ";
        // }
        // std::cout << "\n";

        archive.insert(next_state.used_polys);
        next_states.push_back(std::move(next_state));

        // const auto& [it, successful] = archive.insert(next_state);
        // const auto& [it, successful] = archive.insert(std::move(next_state));
        //std::cout << "buckets: " << archive.bucket_count() << "\n";

        // if(successful){
        //     next_states.push_back(&*it);
            // std::cout << "thread " << id <<  " inserting " << it->get_num() << "\n";
        // } else {
        //     // std::cout << it->get_num() << " unsuccessful, thread " << id << " \n";
        // }
    }
    
}



int main(){

    Stat first_state;
    {
        std::vector<Stat> next_statess = first_state.find_next_states();
        std::vector<unsigned> indices{10, 4, 4, 0, 22};
        // std::vector<unsigned> indices{10, 4, 4, 0, 22, 11, 3, 9, 2};
        // auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 3, 9, 2, 0, 0, 0, 0, 0};

        for(auto& index: indices){
            
            if(next_statess.size() == 0){
                std::cout << "empty" << std::endl;
                break;
            }

            first_state = std::move(next_statess.at(index));
            next_statess = first_state.find_next_states();
        }
    }
    // std::cout << first_state << "\n";

    Container archive;
    archive.insert(first_state.used_polys);
    std::vector<Stat> next_states{std::move(first_state)};

    std::vector<std::thread> threads;
    for (int i = 0; i < 10; i++) {
        // std::cout << i << "\n";
        threads.push_back(std::thread([&archive, &next_states](){

            bool tried = false;
            double count = 0;
            while(true){
                // std::cout << std::this_thread::get_id() << "\n";
                
                auto it_next = get_next_pointer(next_states);

                if(not it_next){
                    std::cout << ".";
                    if(tried){
                        break;
                    } else{
                        std::this_thread::sleep_for(std::chrono::seconds(1));
                        tried = true;
                        continue;
                    }
                }
                
                auto new_next_states = it_next->find_next_states();
                
                add_next_states(archive, new_next_states, next_states, count);
            }
            std::cout << count << "\n";
        }));
    }

    for (auto& t : threads) {
        t.join();
    }

    Stat aa;
    // Graphics graphics;
    std::vector<long> hashes;
    std::cout << "archive:\n";
    unsigned complete_states = 0;
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
            complete_states++;
            // auto used_polys = used_polys_to_vector(state);
            // auto frame = aa.frame_as_vector();   
            // graphics.draw_state(used_polys, frame);
        }


        // hashes.push_back(state.state_hash);

        // if(state.current_polygon.size_ll < 3){
        //     std::cout << state << "\n";
        //     // graphics.draw_state(used_polys, frame);
        

        // }
        
    }
    std::cout << "complete: " << complete_states << "\n";
    std::cout << "number of states: " << archive.size() << "\n";
    // std::unordered_set<int> unique_numbers(hashes.begin(), hashes.end());
    // std::cout << "number of hashes: " << unique_numbers.size() << "\n";
}