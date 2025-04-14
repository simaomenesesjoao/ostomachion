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
    std::vector<unsigned int> results{2, 1, 0, 3};


    for(unsigned int i=0; i<points.size(); i++){
        auto& [x,y] = points.at(i);
        unsigned result = results.at(i);
        unsigned int index = poly.get_farthest_node(x,y);
        std::cout << (index==result) << "\n";

    }

}