// #pragma once

#include "state.cpp"
#include <fstream>
#include <gmpxx.h>
#include <iostream>

int main(int argc, char** argv){


    std::vector<int> indices;
    for(int i=1; i<argc; i++){
        int index = atoi(argv[i]);
        indices.push_back(index);
        // std::cout << index << "i ";
    }


    using T = mpz_class;
    using Num = Number<T, 2, 5, 13, 17>;
    State<Num> state;
    std::vector<State<Num>> next_states = state.find_next_states();


    for(auto& index: indices){
        
        if(next_states.size() == 0){
            std::cout << "empty" << std::endl;
            break;
        }

        state = next_states.at(index);
        next_states = state.find_next_states();
    }


    for(auto& state: next_states)
        std::cout << state;


    // std::ofstream out("polys.dat");
    // for(auto& state: next_states)
    //     out << state;
    // out.close();
    
    return 0;
}