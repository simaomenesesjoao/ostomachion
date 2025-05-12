#include "solver.cpp"
#include "settings.hpp"
#include <string>
#include <iostream>

// std::string demangle(const char* name) {
//     int status = 0;
//     char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
//     std::string result = (status == 0 && demangled) ? demangled : name;
//     free(demangled);
//     return result;
// }

// void print_stacktrace() {
//     void* buffer[100];
//     int size = backtrace(buffer, 100);                     // Get stack addresses
//     char** symbols = backtrace_symbols(buffer, size);      // Get function names (mangled)

//     std::cerr << "Stack trace:\n";
    
//     for (int i = 0; i < size; ++i) {
//         // Demangle the symbol names and print them
//         std::cerr << demangle(symbols[i]) << "\n";
//     }
    
//     free(symbols);
// }

// void resolve_addr(const void* addr) {
//     std::stringstream cmd;
//     cmd << "addr2line -f -e ./main " << addr;
//     FILE* fp = popen(cmd.str().c_str(), "r");
//     if (!fp) {
//         std::cerr << "Failed to resolve address with addr2line" << std::endl;
//         return;
//     }
    
//     char buffer[256];
//     while (fgets(buffer, sizeof(buffer), fp)) {
//         std::cerr << buffer;
//     }
//     fclose(fp);
// }

// void crash_handler(int sig) {
//     std::cerr << "Caught signal " << sig << "\n";
//     print_stacktrace();
//     exit(1);
// }



int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    // signal(SIGSEGV, crash_handler); // handle segmentation fault
    
    std::string poly_type = "ContigPoly"; 
    std::string puzzle_type = "Ostomini";
    unsigned int num_threads = 1;
    if (argc > 3) {
        poly_type = argv[1];
        puzzle_type = argv[2];
        num_threads = atoi(argv[3]);

    } else {
        std::cout << "Input not valid.\n";
        // exit(1);
    }

    // Outer-level function: set input, receive number of combinations
    auto input = Input::get_premade_input(puzzle_type);
    if(not input){
        std::cout << "Input not valid\n";
        exit(1);
    }

    CalcSettings calc_settings("Double", poly_type, 
        "Stack", "complete", "leftest", Polygon::Transformations::RotFlip, num_threads);
    AnalyticsSettings analytics_settings;

    auto [states, analytics] = get_combinations(*input, calc_settings, analytics_settings);
    
    // std::cout << states.size() << "\n";

    // for(const auto& s: states){
    //     s->print_output();
    // }
    
    // analytics.display();

    // save_to_png(combinations);


}