#include "fracroot.cpp"
#include <iostream>
#include <cassert>

int main(){
    using T = int;
    {
        int x = 2;
        int norm2 = x*x;
        std::cout << "ex1\n";
        FracRoot<T,5> f{{x,norm2},norm2};
    }
    
    
    {
        std::cout << "ex2\n";
        FracRoot<T,1,2,5> x{{6,3},40};
        assert(x.get_num() == 4 and x.get_den() == 1 and x.get_root() == 10);
    }

    {
        std::cout << "ex3\n";
        FracRoot<T,1,2,5> x{{-1,7},100};
        assert(x.get_num() == -10 and x.get_den() == 7 and x.get_root() == 1);
    }

    {
        FracRoot<T,2,5> x{{-1,7},100};
        assert(x.get_num() == -10 and x.get_den() == 7 and x.get_root() == 1);
    }

    {
        FracRoot<T,3,7> x{{-1,7},21};
        assert(x.get_num() == -1 and x.get_den() == 7 and x.get_root() == 21);
    }


    {
        std::cout << "Testing FracRoot product and subtraction: " << std::endl;
        FracRoot<T,1,2,5> x{{6,3},40};
        FracRoot<T,1,2,5> y{{3,6},5};
        FracRoot<T,1,2,5> z{{4,6},5};
        std::cout << x*y << std::endl;
        std::cout << y-z << std::endl;
    }


    {
        std::cout << "Testing FracRoot product and addition: " << std::endl;
        FracRoot<T> x{{6,3},1};
        FracRoot<T> y{{3,6},1};
        std::cout << x*y << std::endl;
        std::cout << x+y << std::endl;
    }


    {
        std::cout << "Testing FracRoot comparison" << std::endl;
        FracRoot<T,2,5> x{{1,2},1};
        FracRoot<T,2,5> y{{3,2},1};

        assert((y-x)==1);
        assert((x-x)==0);
    }
}