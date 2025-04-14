#pragma once
#include "polygon.cpp"
#include <functional>
#include <vector>

namespace SymmetryGroup {

    template <typename Poly>
    static Poly poly_identity(const Poly& poly){
        Poly poly2 = poly;
        return poly2;
    }

    template <typename Poly>
    static Poly poly_rotate90(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({0,1},{0,0});
        return poly2;
    }

    template <typename Poly>
    static Poly poly_rotate270(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({0,-1},{0,0});
        return poly2;
    }

    template <typename Poly>
    static Poly poly_rotate180(const Poly& poly){
        Poly poly2 = poly;
        poly2.rotate({-1,0},{0,0});
        return poly2;
    }

    template <typename Poly>
    static Poly poly_flip_x(const Poly& poly){
        Poly poly2 = poly;
        poly2.flip_x();
        // poly2.translate({4,0});
        return poly2;
    }

    template <typename Poly>
    static Poly poly_flip_y(const Poly& poly){
        Poly poly2 = poly;
        poly2.flip_y();
        // poly2.translate({0,4});
        return poly2;
    }

    template <typename Poly>
    static Poly poly_flip_D(const Poly& poly){
        // Flip along the main diagonal
        Poly poly2 = poly;
        poly2 = poly_flip_x(poly2);
        poly2 = poly_rotate90(poly2);
        return poly2;
    }

    template <typename Poly>
    static Poly poly_flip_AD(const Poly& poly){
        // Flip along the other diagonal
        Poly poly2 = poly;
        poly2 = poly_flip_x(poly2);
        poly2 = poly_rotate270(poly2);
        return poly2;
    }

    template <typename Poly>
    struct D4 {
        static std::vector<std::function<Poly(const Poly&)>> transformations;
        static std::string name;
    };

    template <typename Poly>
    std::vector<std::function<Poly(const Poly&)>> D4<Poly>::transformations = 
        {poly_identity<Poly>,  poly_rotate90<Poly>, poly_rotate180<Poly>, 
         poly_rotate270<Poly>, poly_flip_x<Poly>,   poly_flip_y<Poly>,
         poly_flip_D<Poly>,    poly_flip_AD<Poly>};

    template <typename Poly>
    std::string D4<Poly>::name = "D4";


    template <typename Poly>
    struct C4 {
        static std::vector<std::function<Poly(const Poly&)>> transformations;
        static std::string name;
    };

    template <typename Poly>
    std::vector<std::function<Poly(const Poly&)>> C4<Poly>::transformations = 
        {poly_identity<Poly>,  poly_rotate90<Poly>, 
        poly_rotate180<Poly>, poly_rotate270<Poly>};

    template <typename Poly>
    std::string C4<Poly>::name = "C4";
    
    template <typename Poly>
    struct Id {
        static std::vector<std::function<Poly(const Poly&)>> transformations;
        static std::string name;
    };

    template <typename Poly>
    std::vector<std::function<Poly(const Poly&)>> Id<Poly>::transformations = {poly_identity<Poly>};

    template <typename Poly>
    std::string Id<Poly>::name = "Id";

}