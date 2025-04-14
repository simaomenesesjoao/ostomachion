#pragma once
#include <memory>
#include <string>
#include <functional>
#include <iostream>

namespace Polygon{

    class PlaceHolder{
    public:
        PlaceHolder(){};
    };

    class IPoly{
    public:
        bool is_valid(){
            return true;
        };
    };

    class BarePoly: public IPoly{
    public:
        BarePoly(const std::vector<std::pair<int, int>>&){};
    };

    template <typename Num>
    class Poly: public IPoly{
    public:
        Poly(const BarePoly& poly){};
    };

    template <typename Num>
    class ContigPoly: public IPoly{
    public:
        ContigPoly(const BarePoly& poly){};
    };



    class RestrictedPoly: public IPoly{
    public:
        RestrictedPoly(const BarePoly& poly, 
            unsigned int num_copies, 
            const BarePoly& restriction, 
            const PlaceHolder& transformations){};
        RestrictedPoly(const BarePoly& poly, 
            unsigned int num_copies){};
    };

    std::function<std::unique_ptr<IPoly>(const BarePoly&)> factory(const std::string& name){
        return [](const BarePoly& original){
            return std::make_unique<Poly<double>>(original);
        };
    };
}