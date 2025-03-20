#include <thread>
#include <gmpxx.h>
#include "state.cpp"
#include <fstream>
#include "customFloat.cpp"
#include "polyset.cpp"

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
{
    using Num = Float<double>;
    using Poly = Polygon<Num>;
    
    Ostomini<Poly> polyset;
    Poly poly1({{1,1}, {1,2}, {2,1}});
    Poly poly2({{3,1}, {1,2}, {2,1}});

    polyset.insert(1, poly1);
    polyset.insert(1, poly2);
    std::cout << polyset << "\n";

}

{
    using Num = Float<double>;
    using Poly = Polygon<Num>;
    
    Ostomini<Poly> polyset1, polyset2, polyset3;
    Poly poly1({{0,0}, {2,0}, {0,2}});
    polyset1.insert(1, poly1);

    Poly poly2({{0,0}, {2,0}, {0,2}});
    polyset2.insert(1, poly2);

    Poly poly3({{0,2}, {2,4}, {0,4}});
    polyset3.insert(1, poly3);

    std::cout << polyset1 << "\n" << polyset2 << "\n" << polyset3 << "\n";


    std::cout << "equal? " << (polyset1 == polyset2) << "\n";
    std::cout << "equal? " << (polyset1 == polyset3) << "\n";
}

}