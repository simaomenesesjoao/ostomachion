#include <thread>
#include <gmpxx.h>
#include "state.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "polyset.cpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){

    using Num = Float<double>;
    using Poly = Polygon<Num>;
    
    Ostomini<Poly> polyset;
    Poly poly({{1,1}, {1,2}, {2,1}});
    polyset.insert(0, poly);
    std::cout << polyset << "\n";

}