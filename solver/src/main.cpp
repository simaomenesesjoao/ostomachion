#include <thread>
#include "polygon.cpp"
#include "solver.cpp"
#include "settings.hpp"

int main([[maybe_unused]] int argc, char** argv){
    
    // Outer-level function: set input, receive number of combinations
    auto input = Input::get_premade_input("Ostomini");
    if(not input){
        std::cout << "Input not valid\n";
    }

    CalcSettings calc_settings("Double", "LLPoly", "Overlapper", "Stack", "Leftest", 10);
    AnalyticsSettings analytics_settings;

    auto [states, analytics] = get_combinations(*input, calc_settings, analytics_settings);
    

    std::cout << states.size() << "\n";
    // analytics.display();

    // save_to_png(combinations);


}