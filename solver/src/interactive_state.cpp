// #pragma once
#include <string>
#include <gmpxx.h>
#include <iostream>

#include "polygon.cpp"
#include "state.hpp"
#include "settings.hpp"

int main(int argc, char** argv){
    
    std::string puzzle_type = argv[1];
    auto input = Input::get_premade_input(puzzle_type);
    if(not input){
        std::cout << "Input not valid\n";
        exit(1);
    }

    std::vector<std::pair<unsigned short, unsigned short>> history;    
    for(int i = 2; i < argc; i += 2){
        int pol_index = atoi(argv[i]);
        int var_index = atoi(argv[i+1]);
        history.push_back({pol_index, var_index});
    }

    std::string poly_type = "ContigPoly";
    CalcSettings calc_settings("Double", poly_type, 
        "Stack", "complete", "leftest", Polygon::Transformations::RotFlip, 1, 0);


    auto state_factory = State::factory(poly_type, input->frame, input->polygon_pool, calc_settings);
    auto allocator = Allocator::factory<std::shared_ptr<State::IState>>(calc_settings.container_type);
        
    unsigned int memory_size = 100;
    allocator->init(memory_size, state_factory);
    auto state = allocator->allocate();

    
    state->set_history(history);
    state->activate_history();

    auto next_states = state->find_next_states();

    for(auto& st: next_states){
        st->activate_history();
        st->print_output();
    }

    return 0;
}