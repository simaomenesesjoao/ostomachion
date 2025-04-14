#include <thread>
#include <gmpxx.h>
#include "polygon.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "polyset.cpp"
#include "state.cpp"
#include "group.cpp"
#include "ArchiveQueue.cpp"

std::pair<std::vector<State>, Analytics> get_combinations(
    const Input& input, 
    const CalcSettings& settings, 
    const AnalyticsSettings& analytics_settings){

    State<type> first_state(input.frame, input.polygon_list);
    first_state.iterate(input.initializer);

    Container container = ContainerFactory(settings.container_type);
    container.insert(first_state);

    std::vector<std::thread> threads([&container](){
        while(true){
            const State& state = container.pop();
            if(!state)
                break;
            std::vector<State> next_states = find_next_states(state, thread_register);
            container.insert(next_states);
        }
    }, settings.num_threads);

    const Analytics& analytics = container.get_analytics();
    const std::vector<State>& solutions = container.get_solutions();

    return {solutions, analytics};

}

int main([[maybe_unused]] int argc, char** argv){
    
    // Define input
    frame = PolyInt({points});
    polygon_list = {RestrictedPolygon{{1,1}, containment_frame, transformations}, ...};
    Input = PredefinedInputs::Ostomachion;

    // Outer-level function: set input, receive number of combinations
    Input input(frame, polygon_list, initializer=opt);
    CalcSettings settings(numeric_type, overlapper, container_type, node_selector, num_threads);
    AnalyticsSettings analytics_settings(enable_container_analytics);

    combinations, analytics = get_combinations(input, settings, analytics_settings);
    
    analytics.display();

    save_to_png(combinations);


}