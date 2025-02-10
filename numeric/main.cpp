#include "numbers.hpp"
#include <iostream>

int main(){
    Number<int, 2, 3> x{1};
    std::cout << x << std::endl;
}