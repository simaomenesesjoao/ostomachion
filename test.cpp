#include <iostream>

#include "intersection_algorithms.cpp"
#include "polygon.cpp"
#include "customFloat.cpp"

int main(){
{
    //using Num = Number<int, 2, 3>;
    using Num = Float<double>;
    Angle<Num> A{0,1}, B{-1,1}, C{-1,-1}; // 90, 135, 225
    std::cout << A.get_cos() << " " << A.get_sin() << "\n";
    std::cout << B.get_cos() << " " << B.get_sin() << "\n";
    std::cout << C.get_cos() << " " << C.get_sin() << "\n";

    assert(angles_compatible(A,B) == true);
    assert(angles_compatible(B,A) == true);
    assert(angles_compatible(C,C) == false);
    assert(angles_compatible(A,C) == true);
    assert(angles_compatible(B,C) == true);
}

{
    using Num = Float<double>;
    std::vector<std::vector<int>> points{{1,2},{2,3},{3,1}};
    Polygon::ContigPoly<Num> poly(points);
    
}
}