// #pragma once

#include "state.cpp"
#include <sstream>
#include <gmpxx.h>
#include <random>
#include "customFloat.cpp"

#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>

TEST_CASE("Float<double> = mpz_class", "[custom]"){
    using T = Number<mpz_class, 2, 5, 13, 17>;
    using U = Float<double>;
    
    State<T> stateT;
    State<U> stateU;

    
    auto indices = std::vector<unsigned>{10, 4, 4, 0, 22, 11, 3, 9, 2, 0, 0, 0, 0, 0};
    

    for(unsigned index: indices){
        auto next_statesT = stateT.find_next_states();
        auto next_statesU = stateU.find_next_states();
        
        REQUIRE(next_statesT.size() == next_statesU.size());
        stateT = next_statesT.at(index);
        stateU = next_statesU.at(index);
    }
    
}

bool randomized_comparison(unsigned NR){
    using T = Number<mpz_class, 2, 5, 13, 17>;
    using U = Float<double>;
    
    // Initialize the random number generators
    std::random_device rd;
    std::mt19937 gen(rd());
    
    for(unsigned i=0; i<NR; i++){
        State<T> stateT;
        State<U> stateU;

        while(true){

            auto next_statesT = stateT.find_next_states();
            auto next_statesU = stateU.find_next_states();
            if(next_statesT.size() != next_statesU.size())
                return false;

            if(next_statesT.size() == 0)
                break;

            std::uniform_int_distribution<> distr(0, next_statesT.size()-1);
            unsigned next_index = static_cast<unsigned>(distr(gen));
            stateT = next_statesT.at(next_index);
            stateU = next_statesU.at(next_index);
        }
    }

    return true;
}

TEST_CASE("Float<double> = mpz_class randomized", "[custom]"){
    REQUIRE(randomized_comparison(10));
}