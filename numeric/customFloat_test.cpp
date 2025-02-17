#include "include/customFloat.cpp"
#include <iostream>

int main(){
    using F = Float<float>;
    F x(0,1,2,3,4), y{3};

    std::cout << "1: " << x << " " << y << "\n";

    x = y + y;
    y += x;
    std::cout << "2: " << x << " " << y << "\n";
    
    y = y/2;
    y = y*2.3;
    y = y*2;

    x = x+1;
    std::cout << "3: " << x << " " << y << "\n";

    y = y*0 + 1.3;
    x = y*0 + 1.30000001;
    std::cout << "4: " << x << " " << y << "\n";

    std::cout << "are equal: " << (y == x) << "\n";
    std::cout << "are equal: " << (y == (x+1)) << "\n";

    y = y*0 + 2.1;
    std::cout << "5: " << x << " " << y << "\n";

    std::cout << (y<2.2) << "\n";
    std::cout << (y<2.0) << "\n";

    std::cout << (y>2.2) << "\n";
    std::cout << (y>2.0) << "\n";


    std::cout << y << "\n";
}