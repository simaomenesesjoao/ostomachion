#include <gmpxx.h>
#include "polygon.cpp"
#include <fstream>
#include "customFloat.cpp"

#include "polyset.cpp"
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv){
    {
        using Num = Float<double>;
        using Poly = Polygon<Num>;
        using vec = std::vector<unsigned int>;
        
        Ostomini<Poly> polyset;
        Poly poly1({{1,1}, {1,2}, {2,1}});
        Poly poly2({{3,1}, {1,2}, {2,1}, {2,2}});
    
        std::cout << polyset << "\n";

        std::cout << (polyset.find_unfilled_types() == vec{0,1,2,3}) << "\n";
        polyset.insert(1, poly1);
        std::cout << (polyset.find_unfilled_types() == vec{0,1,2,3}) << "\n";
        polyset.insert(1, poly1);
        std::cout << (polyset.find_unfilled_types() == vec{0,2,3}) << "\n";
        polyset.insert(2, poly2);
        std::cout << (polyset.find_unfilled_types() == vec{0,3}) << "\n";
    }
    
    {
        using Num = Float<double>;
        using Poly = Polygon<Num>;
        
        Ostomini<Poly> polyset1, polyset2, polyset3, polyset4, polyset5, polyset6, polyset7;
        polyset1.insert(1, {{{0,0}, {3,0}, {0,2}}}); // original
        polyset2.insert(1, {{{0,0}, {3,0}, {0,2}}}); // identical
        polyset3.insert(1, {{{0,1}, {2,4}, {0,4}}}); // 90º rotation
        polyset4.insert(1, {{{0,2}, {2,4}, {0,3}}}); // different
        polyset5.insert(1, {{{1,0}, {4,0}, {4,2}}}); // flip x
        polyset6.insert(1, {{{0,2}, {3,4}, {0,4}}}); // flip y
        polyset7.insert(1, {{{4,2}, {4,4}, {1,4}}}); // 180º rotation
    
        // std::cout << polyset1 << "\n" << polyset2 << "\n" << polyset3 << "\n" << polyset4 << "\n";
    
        std::cout << "equal? " << (polyset1 == polyset2) << "\n";
        std::cout << "equal? " << (polyset1 == polyset3) << "\n";
        std::cout << "equal? " << (polyset1 == polyset4) << "\n";
        std::cout << "equal? " << (polyset1 == polyset5) << "\n";
        std::cout << "equal? " << (polyset1 == polyset6) << "\n";
        std::cout << "equal? " << (polyset1 == polyset7) << "\n";
    }

    {

        // Test the hashing functionality
        using Num = Float<double>;
        using Poly = Polygon<Num>;

        Ostomini<Poly> polyset1, polyset2, polyset3, polyset4;
        Poly poly1({{1,1}, {1,2}, {2,1}});
        Poly poly2({{3,1}, {2,1}, {1,2}, {2,2}});
        Poly poly3({{3,1}, {2,1}, {1,2}, {2,3}});

        polyset1.insert(1,poly1);
        std::cout << polyset1.get_hash() << "\n";

        polyset1.insert(2,poly2);
        std::cout << polyset1.get_hash() << "\n";

        polyset2 = polyset1;
        polyset2.flip_x();
        std::cout << polyset2.get_hash() << "\n";

        polyset3.insert(1, poly1);
        polyset3.insert(2, poly3);
        std::cout << polyset3.get_hash() << "\n";


        polyset4.insert(1, poly1);
        polyset4.insert(2, poly2);
        std::cout << polyset4.get_hash() << "\n";

        std::cout << "1:\n" << polyset1 << "\n";
        std::cout << "2:\n" << polyset2 << "\n";
        std::cout << "3:\n" << polyset3 << "\n";
        std::cout << "3:\n" << polyset4 << "\n";

        std::cout << "polyset 1 and 2\n";
        std::cout << "Hashes match? " << (polyset1.get_hash() == polyset2.get_hash()) << "\n";
        std::cout << "Equal? " << (polyset1 == polyset2) << "\n";
        std::cout << "Strict equality? " << polyset1.strict_equality(polyset2) << "\n\n";

        std::cout << "polyset 1 and 3\n";
        std::cout << "Hashes match? " << (polyset1.get_hash() == polyset3.get_hash()) << "\n";
        std::cout << "Equal? " << (polyset1 == polyset3) << "\n";
        std::cout << "Strict equality? " << polyset1.strict_equality(polyset3) << "\n\n";

        std::cout << "polyset 1 and 4\n";
        std::cout << "Hashes match? " << (polyset1.get_hash() == polyset4.get_hash()) << "\n";
        std::cout << "Equal? " << (polyset1 == polyset4) << "\n";
        std::cout << "Strict equality? " << polyset1.strict_equality(polyset4) << "\n";
    }
}