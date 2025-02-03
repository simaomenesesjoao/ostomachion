// #pragma once

#include "state.cpp"
#include <fstream>
#include <gmpxx.h>

int main(){
    // Tracker<2,3> tracker;
    using T = mpz_class;
    // using T = int;
    using Num = Number<T, 2, 5, 13, 17>;
    State<Num> state;

    std::vector<unsigned> indices{2, 0, 6, 1};
    std::vector<State<Num>> next_states;

    for(auto& index: indices){
        next_states = state.find_next_states();
        
        if(next_states.size() == 0){
            std::cout << "empty" << std::endl;
            break;
        }

        state = next_states.at(index);
    }

    std::ofstream out("polys.dat");
    for(auto& state: next_states)
        out << state;
    out.close();
    
    return 0;
}