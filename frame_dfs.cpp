#include "state.cpp"

using T = mpz_class;
using Num = Number<T, 2, 5, 13, 17>;

std::vector<State<Num>> dfs(State<Num> const& state){
    std::cout << "Calling dfs " << state.current_polygon.size_ll << std::endl;
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


    State<Num> state;
    auto final_states = dfs(state);
    std::cout << final_states.size() << std::endl;
    return 0;
}