#include "numbers.hpp"
#include <iostream>

int main(){
    Number<int, 2, 5> x{3};
    std::cout << x-x << std::endl;
}