#include "numbers.hpp"
#include "numbers.cpp"
#include <cassert>
#include <iostream>

int main(){

    {

        using T = int;
        FracRoot<T,2,5> a{{1,2},5};
        // FracRoot<T,2,5> b{{-3,2},2};
        // FracRoot<T,2,5> c{{4,2},10};
        // FracRoot<T,2,5> d{{1,6},1};
        Number<T,2,5> x{a};//, y{c}, z{x*y};
        
        int is_pos = is_pos_fractional<T, 2, 5>(x);

        std::cout << "is pos?" <<  is_pos << std::endl;

    }
    

    return 0;



    using T = long long;
    

    {
        std::cout << "Testing Number constructors: " << std::endl;
        Number<T, 2> x{2};
        Number<T, 1> y{2};
        assert(equal(2.0, (double)x));
        assert(equal(2.0, (double)y));
    }


    {
        std::cout << "Testing Number operations +*: " << std::endl;
        FracRoot<T,2,5> a{{1,2},5};
        FracRoot<T,2,5> b{{-3,2},2};
        FracRoot<T,2,5> c{{4,2},10};
        FracRoot<T,2,5> d{{1,6},1};
        Number<T,2,5> x{a}, y{c}, z{x*y};
        
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;
        std::cout << "z: " << z << std::endl;

        using T = double;
        assert(equal((T)x + (T)y ,(T)(x+y)));
        assert(equal((T)x * (T)y ,(T)(x*y)));
        

    }


    {
        std::cout << "Testing unary Number operations -: " << std::endl;
        FracRoot<T,2,5> a{{1,2},5};
        FracRoot<T,2,5> b{{-3,2},2};
        Number<T,2,5> x{a}, y{b}, z;
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

        FracRoot<T,2> a{{1,2},2};
        FracRoot<T,2> b{{-3,4},1};
        FracRoot<T,2> c{{0,4},2};
        Number<T,2> x{a}, y{a,b}, z{c};

        assert((x>0) == ((double)x>0));
        assert((y>0) == ((double)y>0));
        assert((z<0) == ((double)z>0));
        
        assert(equal((double)x, 0.5*std::sqrt(2)));
        assert(equal((double)y, 0.5*std::sqrt(2) - 0.75));
        assert(equal((double)z, 0.0));
    }


    {
        std::cout << "Testing is_pos for <2,5,13,17>." << std::endl;

        
        FracRoot<int,2,5,13,17>       ai({306,5},5), bi{{18,17},17}, ci{{-4254,85},85};
        FracRoot<long long,2,5,13,17> al({306,5},5), bl{{18,17},17}, cl{{-4254,85},85};
        FracRoot<mpz_class,2,5,13,17> am({306,5},5), bm{{18,17},17}, cm{{-4254,85},85};

        Number<int,2,5,13,17>       xi{ai,ci};
        Number<long long,2,5,13,17> xl{al,cl};
        Number<mpz_class,2,5,13,17> xm{am,cm};

        std::cout << xi << " " << (double)xi << std::endl;
        std::cout << xl << " " << (double)xl << std::endl;
        std::cout << xm << " " << (double)xm << std::endl;
        assert((xi > 0) == true);
        assert((xl > 0) == false);
        assert((xm > 0) == false);
        
    }
    

    {
        std::cout << "Testing is_zero for <2,3>." << std::endl;

        FracRoot<T,2,3> a{{1,2},2};
        Number<T,2,3> x{a}, y{a};

        assert((x-y).is_zero());
    }


    {
        std::cout << "Testing conjugation" << std::endl;   
        Number<T,2,5> z{1188};
        std::cout << "z2: " << z.conjugate(2) << std::endl;
        std::cout << "z5: " << z.conjugate(5) << std::endl;
        
    }


    {
        std::cout << "Testing conjugation" << std::endl;
        FracRoot<T,2,5> a{{3,1},1};
        FracRoot<T,2,5> b{{-3,2},2};
        FracRoot<T,2,5> c{{1,2},5};
        
        Number<T,2,5> z{a,b,c};
        Number<T,2,5> z2{a,-b,c};
        Number<T,2,5> z5{a,b,-c};

        assert(z.conjugate(2) == z2);
        assert(z.conjugate(5) == z5);
    }


    
    {
        std::cout << "Testing inverse" << std::endl;   
        Number<int,2,5> zi{1188};
        Number<long long,2,5> zl{1188};
        std::cout << "z: " << zi.inverse() << std::endl;
        std::cout << "z: " << zl.inverse() << std::endl;
        assert((zi*zi.inverse() != 1));
        assert(zl*zl.inverse() == 1);
    }

    {
        std::cout << "Testing inverse" << std::endl;
        using T = int;
        FracRoot<T,2,5> a{{3,1},1};
        FracRoot<T,2,5> b{{-3,2},2};  
        FracRoot<T,2,5> c{{1,2},5};      
        Number<T,2,5> z{a,b,c};
        assert(z*z.inverse() == 1);
    }

    {
        std::cout << "Testing operator /" << std::endl;
        using T = int;
        FracRoot<T,2,5> a{{3,1},1};
        FracRoot<T,2,5> b{{-3,2},2};   
        FracRoot<T,2,5> c{{1,2},5};     
        Number<T,2,5> x{a,b,c};
        Number<T,2,5> y{b,-c};
        Number<T,2,5> z{x/y};
        assert(y*z == x);
    }


    {
        std::cout << "Testing operator /" << std::endl;  
        using T = int;
        Number<T,2,5> x{1890};
        Number<T,2,5> y{-1188};
        std::cout << x/y << std::endl;
    }

    {
        std::cout << "Testing operator / int" << std::endl;  
        Number<T,2,5> x{1891};
        std::cout << x/2 << std::endl;
    }
}