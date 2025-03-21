#include <thread>
#include <gmpxx.h>
#include "polygon.cpp"
#include <fstream>
#include "customFloat.cpp"

#include "polyset.cpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    using Num = Float<double>;
    using Poly = Polygon<Num>;
    
    Poly poly({{0,0}, {10,0}, {10,10}, {0,10}});

    std::vector<std::pair<int, int>> points{{1,1}, {1,9}, {9,9}, {9,1}};

    for(auto& [x,y]: points){
        unsigned int index = poly.get_farthest_node(x,y);
        std::cout << index << "\n";

    }

}