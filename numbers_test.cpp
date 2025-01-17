#include "numbers.hpp"
#include "numbers.cpp"
#include <cassert>
#include <iostream>

// TO DO - is_pos não precisa de argumento

int main(){

    {
        std::cout << "Testing fractions: " << std::endl;
        Fraction x{4,3}, y{2,5};
        std::cout << x << std::endl;
        std::cout << x-y << std::endl;
    }

    
    {
        int x = 2;
        int norm2 = x*x;
        std::cout << "ex1\n";
        FracRoot<5> f{{x,norm2},norm2};
    }
    
    
    {
        std::cout << "ex2\n";
        FracRoot<1,2,5> x{{6,3},40};
        assert(x.get_num() == 4 and x.get_den() == 1 and x.get_root() == 10);
    }

    {
        std::cout << "ex3\n";
        FracRoot<1,2,5> x{{-1,7},100};
        assert(x.get_num() == -10 and x.get_den() == 7 and x.get_root() == 1);
    }

    {
        FracRoot<2,5> x{{-1,7},100};
        assert(x.get_num() == -10 and x.get_den() == 7 and x.get_root() == 1);
    }

    {
        FracRoot<3,7> x{{-1,7},21};
        assert(x.get_num() == -1 and x.get_den() == 7 and x.get_root() == 21);
    }


    {
        std::cout << "Testing FracRoot subtraction: " << std::endl;
        FracRoot<1,2,5> x{{6,3},40};
        FracRoot<1,2,5> y{{3,6},5};
        FracRoot<1,2,5> z{{4,6},5};
        std::cout << x*y << std::endl;
        std::cout << y-z << std::endl;
    }


    {
        FracRoot<> x{{6,3},1};
        FracRoot<> y{{3,6},1};
        std::cout << x*y << std::endl;
        std::cout << x+y << std::endl;
    }



    {
        std::cout << "Testing Number constructors: " << std::endl;
        Number<2> x{2};
        Number<1> y{2};
        assert(equal(2.0, (double)x));
        assert(equal(2.0, (double)y));
    }


    {
        std::cout << "Testing Number operations +*: " << std::endl;
        FracRoot<2,5> a{{1,2},5};
        FracRoot<2,5> b{{-3,2},2};
        FracRoot<2,5> c{{4,2},10};
        FracRoot<2,5> d{{1,6},1};
        Number<2,5> x{a}, y{c}, z{x*y};
        
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        std::cout << "z: " << z << std::endl;

        using T = double;
        std::cout << "x+y: " << (T)x + (T)y << " " <<  (T)(x+y) << std::endl; 
        std::cout << "x*y: " << (T)x * (T)y << " " <<  (T)(x*y) << std::endl; 


    }


    {
        std::cout << "Testing unary Number operations -: " << std::endl;
        FracRoot<2,5> a{{1,2},5};
        FracRoot<2,5> b{{-3,2},2};
        Number<2,5> x{a}, y{b}, z;
        z = x + y;
        
        std::cout << "z: " << z << std::endl;
        std::cout << "-z: " << -z << std::endl;


    }



    {
        std::cout << "Testing is_pos for <>." << std::endl;

        FracRoot<> a{{1,2},1};
        FracRoot<> b{{-3,4},1};
        FracRoot<> c{{0,4},1};
        Number<> x{a}, y{b}, z{c};
        assert(x>0);
        assert(y<0);
        assert(z==0);
    }

    

    {
        std::cout << "Testing is_pos for <2>." << std::endl;

        FracRoot<2> a{{1,2},2};
        FracRoot<2> b{{-3,4},1};
        FracRoot<2> c{{0,4},2};
        Number<2> x{a}, y{a,b}, z{c};

        assert((x>0) == ((double)x>0));
        assert((y>0) == ((double)y>0));
        assert((z<0) == ((double)z>0));
        
        assert(equal((double)x, 0.5*std::sqrt(2)));
        assert(equal((double)y, 0.5*std::sqrt(2) - 0.75));
        assert(equal((double)z, 0.0));
    }

    {
        std::cout << "Testing is_pos for <2,3>." << std::endl;

        FracRoot<2,3> a{{1,2},2};
        FracRoot<2,3> b{{-3,4},3};
        FracRoot<2,3> c{{0,4},6};
        Number<2,3> x{}, y{}, z{};
        x += a;
        y += a;
        y += b;
        z += c;

        assert((x>0) == ((double)x>0));
        assert((y>0) == ((double)y>0));
        assert((z>0) == ((double)z>0));
    }



}