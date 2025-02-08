#include "state.cpp"

// g++ -std=c++20 -Wall -O3 frame_dfs.cpp -lgmp -lgmpxx -o test && ./test
// valgrind --tool=callgrind ./test


using Num = Number<mpz_class, 2, 5, 13, 17>;

std::vector<State<Num>> dfs(State<Num> const& state){
    std::cout << "Calling dfs " << state.current_polygon.size_ll << std::endl;
    if(state.current_polygon.size_ll == 0)
        return {state};
    
    auto next_states = state.find_next_states();

    std::vector<State<Num>> final_states;
    for(auto& new_state: next_states){
        auto finals = dfs(new_state);
        for(auto& final: finals)
            final_states.push_back(final);
    }

    return final_states;
}

int main(){


    // using T = mpz_class;
    // using Num = Number<T, 2, 5, 13, 17>;
    State<Num> state;
    auto indices = std::vector<unsigned>{10, 4, 4, 0, 22};

    int n=0;
    for(auto& index: indices){
        auto next_states = state.find_next_states();
        state = next_states.at(index);
        n++;
    }

    // State<Num> state;
    auto final_states = dfs(state);
    std::cout << final_states.size() << std::endl;
    for(auto& final_state: final_states){
        std::cout << final_state << std::endl;
    }
    return 0;
}