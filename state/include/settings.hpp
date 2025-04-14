#pragma once
#include "polygon.cpp"
#include <optional>

struct CalcSettings{
    std::string numeric_type, poly_type, overlapper, container_type, node_selector;
    unsigned int num_threads;
};

struct AnalyticsSettings{
};

namespace Input{

    struct Input{
        Polygon::BarePoly frame;
        std::vector<Polygon::RestrictedPoly> polygon_pool;
        Polygon::PlaceHolder initializer;
        Input(const Polygon::BarePoly& frame, 
            const std::vector<Polygon::RestrictedPoly>& polygon_pool, 
            const Polygon::PlaceHolder& initializer): 
                frame{frame}, polygon_pool{polygon_pool}, initializer{initializer}{};
    };


    std::optional<Input> get_premade_input(const std::string& name){
        if(name == "Ostomini"){
            Polygon::BarePoly frame({{2,2}, {-2, 2}, {-2, -2}, {2, -2}});
            std::vector<Polygon::RestrictedPoly> polygon_pool{{
                {{{{0,0}, {2,4}, {0,4}}}, 2},
                {{{{0,0}, {1,0}, {1,2}}}, 2},
                {{{{1,0}, {3,0}, {3,2}, {1,2}}}, 1},
                {{{{1,2}, {3,2}, {2,4}}}, 1}
            }};
            return Input(frame, polygon_pool, {});
        } 

        return std::nullopt;
    }
}