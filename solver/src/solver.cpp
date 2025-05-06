#pragma once
#include <thread>
#include <vector>
#include <gmpxx.h>
#include <fstream>

#include "polygon.cpp"
#include "state.hpp"
#include "container.hpp"
#include "settings.hpp"
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
    // first_state.iterate(input.initializer); // SIMAO: implementar


    // unsigned int memory_size = 100;
    // State::MemoryPool memory_pool;
    // memory_pool.initialize(memory_size, state_factory);
    

    auto container = Container::factory<std::shared_ptr<State::IState>>(settings.container_type);
    // auto first_state = memory_pool.get();
    // first_state->set_initial_state();
    // container->insert({first_state});

    // first_state->print();
    exit(1);
    std::vector<std::thread> threads;
    
    // for(unsigned int i=0; i<settings.num_threads; i++){
        
    //     threads.emplace_back([i, &memory_pool, &container, &settings](){
    //         // AnalyticsThread& analytics_thread = analytics.at(i);
            
    //         std::vector<std::shared_ptr<State::IState>> finalized;
    //         while(true){
                
    //             auto state = container->pop();
                
    //             if(!state)
    //                 break;

    //             if((*state)->finalized()){
    //                 finalized.push_back(*state);

    //             } else {
    //                 auto next_states = (*state)->find_next_states();
    //                 container->insert(next_states);
    //                 // SIMAO: será necessário metodo para finalizar State?
    //                 memory_pool.release(*state); 

    //             }
                
                
                
    //         }
            
    //     });
    // }

    for(auto& thread: threads)
        thread.join();


    // for(unsigned int i = 1; i < settings.num_threads; i++)
    //     analytics.at(0).join(analytics.at(i));

    // analytics.at(0).summary();


    unsigned int total_states = container->get_data().size();
    for(auto& state: container->get_data()){
        state->activate_history();
    }

    auto uniques = find_uniques_brute(container->get_data(), settings.transformation);
    
    std::cout << "total states: " << total_states << "\n";
    std::cout << "total unique: " << uniques.size() << "\n";

    return {uniques, container->get_analytics()};
}