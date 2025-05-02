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

    auto PolyPoolPtr = std::make_shared<Polygon::Pool<Polygon::BarePoly>>(input.polygon_pool);
    auto first_state = State::factory(settings.poly_type)(input.frame, *PolyPoolPtr, settings);
    // first_state.iterate(input.initializer); // SIMAO: implementar

    auto container = Container::factory<std::shared_ptr<State::IState>>(settings.container_type);
    container->insert({first_state});

    std::vector<std::thread> threads;
    std::vector<AnalyticsThread> analytics;
    

    for(unsigned int i=0; i<settings.num_threads; i++){
        analytics.emplace_back(AnalyticsThread());
    }

    for(unsigned int i=0; i<settings.num_threads; i++){
        
        threads.emplace_back([i, &analytics, &container, &settings](){
            AnalyticsThread& analytics_thread = analytics.at(i);
            
            while(true){
                auto state = container->pop();
                (*state)->print();
                if(!state)
                    break;
                auto next_states = (*state)->find_next_states(analytics_thread.branch("find_next_states"));
                // SIMAO: container não tem nada que saber se o estado está 
                // finalizado ou não. essa lógica pode ser passada para aqui
                container->insert(next_states);
            }
            
        });
    }

    for(auto& thread: threads)
        thread.join();


    for(unsigned int i = 1; i < settings.num_threads; i++)
        analytics.at(0).join(analytics.at(i));

    analytics.at(0).summary();


    std::cout << "total states: " << container->get_data().size() << "\n";
    for(auto& state: container->get_data()){
        state->activate_history(input.frame, *PolyPoolPtr, settings);
        // state->print();
    }

    auto uniques = find_uniques_brute(container->get_data(), settings.transformation);
    std::cout << "total unique: " << uniques.size() << "\n";

    return {uniques, container->get_analytics()};
}