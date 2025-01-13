#include <iostream>

#include "points_vectors.hpp"
#include "points_vectors.cpp"

// TO DO: Q.get_x() should print Empty, but prints 0/1 sq(2)
int main(){
    {
        Angle<5> angle1(2,0);
        Angle<3> angle2(0,1);
        std::cout << angle1.get_cos() << " " << angle1.get_sin() << std::endl;
        std::cout << angle2.get_cos() << " " << angle2.get_sin() << std::endl;
    }

    {
        Angle<2,3> angle1(1,1);
        Angle<2,3> angle2(-1,1);
        auto angle3 = angle2 + angle1;
        auto angle4 = angle2 - angle1;
        std::cout << angle1.get_cos() << " " << angle1.get_sin() << std::endl;
        std::cout << angle2.get_cos() << " " << angle2.get_sin() << std::endl;
        std::cout << angle3.get_cos() << " " << angle3.get_sin() << std::endl;
        std::cout << angle4.get_cos() << " " << angle4.get_sin() << std::endl;
    }

    {
        Point<2,3> P{1,1};
        Angle<2,3> a{1,-1};
        Point<2,3> Q = P.rotate(a);
        std::cout << Q.get_x() << " " << Q.get_y() << std::endl;
        // std::cout << "angle: " << a.to<double>() <<  std::endl;
    }


    
}