// #pragma once

#include "state.cpp"
#include <sstream>
#include <gmpxx.h>

using T = mpz_class;
using Num = Number<T, 2, 5, 13, 17>;

int main(){
    State<Num> state;
    // Tracker<2,3> tracker;
    std::vector<State<Num>> next_states;

    next_states = state.find_next_states();
    state = next_states.at(6);

    // state.current_polygon.print();
    // std::cout <<  << std::endl;
    next_states = state.find_next_states();
    state = next_states.at(3);

    std::cout << state;
    
    return 0;
}