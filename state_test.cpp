// #pragma once

#include "state.cpp"
#include <fstream>

int main(){
    // Tracker<2,3> tracker;
    State<2,5,13,17> state;

    std::cout << state << std::endl;
    auto next_states = state.find_next_states();

    if(next_states.size() == 0){
        std::cout << "empty1" << std::endl;
        return 1;
    }
    auto next_state = next_states.at(0);
    std::cout << next_state << std::endl;
    next_states = next_state.find_next_states();


    if(next_states.size() == 0){
        std::cout << "empty1" << std::endl;
        return 1;
    }

    next_state = next_states.at(0);
    std::cout << next_state << std::endl;
    next_states = next_state.find_next_states();

    


    std::ofstream out("polys.dat");
    for(auto& state: next_states)
        out << state;
    out.close();
    return 0;
}