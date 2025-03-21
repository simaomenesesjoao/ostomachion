
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
    