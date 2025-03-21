// #pragma once

#include "state.cpp"
#include <fstream>
#include <gmpxx.h>
#include <iostream>
#include "customFloat.cpp"

int main(int argc, char** argv){
    bool allow_reflection = false;

    std::vector<int> indices;
    for(int i=1; i<argc; i++){
        int index = atoi(argv[i]);
        indices.push_back(index);
    }

    using Num = Number<mpz_class, 2, 5, 13, 17>;
    using Inner = InnerState<Num, true, true>;
    //using Num = Float<double>;
    
    State<Num, Inner> state;
    std::vector<State<Num, Inner>> next_states = state.find_next_states(allow_reflection);

    for(auto& index: indices){
        
        if(next_states.size() == 0){
            std::cout << "empty" << std::endl;
            break;
        }

        state = next_states.at(index);
        next_states = state.find_next_states(allow_reflection);
    }

    for(auto& state: next_states)
        std::cout << state;
    
    return 0;
}