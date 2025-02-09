#include "numeric/numbers.cpp"
#include <gmpxx.h>
#include <iostream>

int main(){

    Number<mpz_class, 2, 5> x{3};
    std::cout << x.is_zero() << std::endl;

}