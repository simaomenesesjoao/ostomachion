#include <iostream>

#include "points_vectors.hpp"
#include "points_vectors.cpp"
#include <cassert>

int main(){


    {
        std::cout << "Testing point rotation" << std::endl;
        using Num = Number<int, 2, 3>;
        Point<Num> P{1,1};
        Angle<Num> a{1,-1};
        Point<Num> Q = P.rotate(a);
        std::cout << Q.get_x() << " " << Q.get_y() << std::endl;    
    }

}