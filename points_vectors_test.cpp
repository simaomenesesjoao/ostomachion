#include <iostream>

#include "points_vectors.hpp"
#include "points_vectors.cpp"
#include <cassert>

int main(){

    {
        std::cout << "Angle: testing equality" << std::endl;
        Angle<2,3> angle1(1,1);
        Angle<2,3> angle2(2,2);
        assert(angle1 == angle1);
        assert(angle1 == angle2);
    }
    

    {
        std::cout << "Angle: testing sines and cosines" << std::endl;
        Angle<5> angle1(2,0);
        Angle<3> angle2(0,1);
        Angle<2> angle3(1,1);
        assert(angle1.get_cos() == 1 && angle1.get_sin() == 0);
        assert(angle2.get_cos() == 0 && angle2.get_sin() == 1);
        assert(angle3.get_cos() == angle3.get_sin() && !angle3.get_cos().is_zero());
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
    }

    {
        std::cout << "Angle: testing comparison <" << std::endl;
        Angle<2,5> A{0,1}; //  90º 
        Angle<2,5> B{1,1}; //  45º
        Angle<2,5> C{1,2}; // ~66º?
        Angle<2,5> D{-1,-1}; // 225º
        Angle<2,5> E{1,-1}; // 315º

        assert(A.is_larger_than_180() == false);
        assert(B.is_larger_than_180() == false);
        assert(C.is_larger_than_180() == false);
        assert(D.is_larger_than_180() == true);
        assert(E.is_larger_than_180() == true);

        assert((A<B) == false);
        assert((B<C) == true);
        assert((B<B) == false);
        assert((D<B) == false);
        assert((D<E) == true);
        assert((A<E) == true);


    }

}