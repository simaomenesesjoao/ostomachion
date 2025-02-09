#include <iostream>
#include "fraction.cpp"
#include <gmpxx.h>
#include <cassert>

int main(){
    {
        std::cout << "Testing fractions subtraction (int, long, mpz): " << std::endl;
        Fraction<int> xi{4,3}, yi{2,5}, ri{14,15};
        Fraction<long> xl{4,3}, yl{2,5}, rl{14,15};
        Fraction<mpz_class> xm{4,3}, ym{2,5}, rm{14,15};
        
        assert(xi-yi == ri);
        assert(xl-yl == rl);
        assert(xm-ym == rm);

    }


    {
        std::cout << "Testing negative fractions: " << std::endl;
        Fraction<int> x{-4,3}, y{2,-5}, z{-1,-1};
        std::cout << x << " " << y << " " << z << std::endl;
    }

    {
        std::cout << "Testing fractions comparison <: " << std::endl;
        Fraction<int> x{4,3}, y{2,5};
        
        std::cout << (x<y) << std::endl;
    }

    

    return 0;
}