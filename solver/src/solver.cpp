#pragma once
#include <thread>
#include <vector>
#include <gmpxx.h>
#include <fstream>

#include "polygon.cpp"
#include "state.hpp"
#include "container.hpp"
#include "settings.hpp"
#include "memory_pool.hpp"
#include "analytics.hpp"

std::vector<std::shared_ptr<State::IState>> find_uniques_brute(
                const std::vector<std::shared_ptr<State::IState>>& states, 
                Polygon::Transformations transformation){

    unsigned int N = states.size();
    std::vector<std::shared_ptr<State::IState>> uniques;
    for(unsigned int i = 0; i < N; i++){
        bool found_identical = false;
        for(unsigned int j = i+1; j < N; j++){
            if(states.at(i)->equals(states.at(j), transformation)){
                found_identical = true;
                break;
            }
        }
        if(!found_identical)
            uniques.push_back(states.at(i));
    }
    return uniques;
}

std::pair<std::vector<std::shared_ptr<State::IState>>, Analytics> get_combinations(
    const Input::Input& input,
    const CalcSettings& settings,
    [[maybe_unused]] const AnalyticsSettings& analytics_settings){ // SIMAO: implementar

    // auto PolyPoolPtr = std::make_shared<Polygon::Pool<Polygon::BarePoly>>(input.polygon_pool);
    auto state_factory = State::factory(settings.poly_type, input.frame, input.polygon_pool, settings);
    auto allocator = Allocator::factory<std::shared_ptr<State::IState>>(settings.container_type);
    // first_state.iterate(input.initializer); // SIMAO: implementar

    // input.polygon_pool.print();

    unsigned int memory_size = 10000;
    allocator->init(memory_size, state_factory);
    
    allocator->print();

    
    auto first_state = allocator->allocate();
    first_state->set_initial_state();
    allocator->send_prog({first_state});

    // first_state->print();
    // exit(1);
    std::vector<std::thread> threads;

    
    std::cout << "Start\n";
    for(unsigned int i=0; i<settings.num_threads; i++){
        
        threads.emplace_back([&allocator](){

            std::vector<std::shared_ptr<State::IState>> from_prog, finalized, to_prog, release;
            
            from_prog.reserve(100);
            finalized.reserve(100);
            to_prog.reserve(100);
            release.reserve(100);
            
            while(true){
                // allocator->print();

                from_prog = std::move(allocator->get_prog(8));
                finalized.clear();
                to_prog.clear();
                release.clear();

                // std::cout << "found " << from_prog.size() << " states from prog\n";
                
                if(from_prog.empty())
                    break;
                
                // std::cout << "----- Loop. Printing state\n";
                // (*state)->print();
                // std::cout << (*state)->size() << " ";
                for(const auto& state: from_prog){
                    if(state->finalized()){
                        finalized.push_back(state);
                    } else {
                        auto next_states = state->find_next_states();
                        
                        for(const auto& st: next_states){
                            to_prog.push_back(st);
                        }
                        release.push_back(state);
                    }
                }

                // std::cout << "before final";
                // allocator->print();
                
                allocator->send_final(finalized);
                // std::cout << "after final";
                // allocator->print();

                allocator->send_prog(to_prog);
                // std::cout << "after send prog";
                // allocator->print();

                allocator->release(release);
                // std::cout << "after release";
                // allocator->print();
                
            }
            // std::cout << "finished\n";
            
        });
    }


    

    for(auto& thread: threads)
        thread.join();

    // for(const auto& st: allocator->get_available_vec()){
    //     std::cout << st->get_age() << " ";
    // }
    // std::cout << "\n";

    std::cout << "FINISHED\n-----------------------\n";
    allocator->print();
    // std::cout << "joining finalized\n";
    std::vector<std::shared_ptr<State::IState>> finalized = allocator->get_finalized_vec();
    // for(const auto& vec: allocator->get_finalized_vec()){
    //     for(const auto& st: vec){
    //         joined.push_back(st);
    //         // st->print();
    //     }
    // }
    // std::cout << "finalized: "<< joined.size() << "\n";
    // std::cout << "done\n";

    AnalyticsThread analytics;

    for(const auto& state: finalized)
        analytics.join(state->get_analytics());

    analytics.summary();

    

    unsigned int total_states = finalized.size();
    std::cout << "total states: " << total_states << "\n";

    allocator->get_analytics().print();
    
    exit(1);
    for(auto& state: finalized){
        state->activate_history();
    }

    std::cout << "\n\n\nFinal states:\n";
    for(auto& state: finalized){
        state->print();
    }

    auto uniques = find_uniques_brute(finalized, settings.transformation);
    
    std::cout << "total states: " << total_states << "\n";
    std::cout << "total unique: " << uniques.size() << "\n";

    // return {uniques, container->get_analytics()};
    return {};
}