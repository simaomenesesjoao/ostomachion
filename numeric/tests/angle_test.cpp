#include <iostream>
#include "points_vectors.cpp"

#include <iostream>

#include "points_vectors.hpp"
#include "points_vectors.cpp"
#include <cassert>

int main(){

    
    {
        std::cout << "Angle: testing equality" << std::endl;
        using Num = Number<int,2,3>;
        Angle<Num> angle1(1,1);
        Angle<Num> angle2(2,2);
        assert(angle1 == angle1);
        assert(angle1 == angle2);
    }
    

    {
        std::cout << "Angle: testing sines and cosines" << std::endl;
        Angle<Number<int,5>> angle1(2,0);
        Angle<Number<int,3>> angle2(0,1);
        Angle<Number<int,2>> angle3(1,1);
        assert(angle1.get_cos() == 1 && angle1.get_sin() == 0);
        assert(angle2.get_cos() == 0 && angle2.get_sin() == 1);
        assert(angle3.get_cos() == angle3.get_sin() && !angle3.get_cos().is_zero());
    }

    {
        std::cout << "Angle: testing angle sum and subtraction" << std::endl;
        using Num = Number<int,2,3>;
        Angle<Num> angle1(1,1);
        Angle<Num> angle2(-1,1);
        auto angle3 = angle2 + angle1;
        auto angle4 = angle2 - angle1;
        std::cout << angle1.get_cos() << " " << angle1.get_sin() << std::endl;
        std::cout << angle2.get_cos() << " " << angle2.get_sin() << std::endl;
        std::cout << angle3.get_cos() << " " << angle3.get_sin() << std::endl;
        std::cout << angle4.get_cos() << " " << angle4.get_sin() << std::endl;
    }


    {
        std::cout << "Angle: testing comparison <" << std::endl;
        using Num = Number<int,2,5>;
        Angle<Num> A{0,1}; //  90º 
        Angle<Num> B{1,1}; //  45º
        Angle<Num> C{1,2}; // ~66º?
        Angle<Num> D{-1,-1}; // 225º
        Angle<Num> E{1,-1}; // 315º

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