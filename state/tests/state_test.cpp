// #pragma once

#include "state.cpp"
#include <sstream>
#include <gmpxx.h>
#include "customFloat.cpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("edges_intersect", "[custom]"){
    using T = Number<mpz_class, 2, 5, 13, 17>;
    using U = Float<double>;
    
    State<T> stateT;
    State<U> stateU;

    auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 2, 9, 0, 0, 1, 0, 0, 0};
    // auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 2};

    for(unsigned index: indices){
        auto next_statesT = stateT.find_next_states();
        auto next_statesU = stateU.find_next_states();
        std::cout << index << "\n";
        REQUIRE(next_statesT.size() == next_statesU.size());
        stateT = next_statesT.at(index);
        stateU = next_statesU.at(index);
    }
    
}


// using T = mpz_class;
// using Num = Number<T, 2, 5, 13, 17>;

// int main(){
//     State<Num> state;
//     // Tracker<2,3> tracker;
//     std::vector<State<Num>> next_states;

//     next_states = state.find_next_states();
//     state = next_states.at(6);

//     // state.current_polygon.print();
//     // std::cout <<  << std::endl;
//     next_states = state.find_next_states();
//     state = next_states.at(3);

//     std::cout << state;
    
//     return 0;
// }