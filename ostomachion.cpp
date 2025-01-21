#include <vector>
#include <iostream>
#include <set>
#include "polygon.cpp"

using Poly = Polygon<2,5,13,17>;

struct polygons{

    static Poly const frame, poly1, poly2, poly3,  poly4,  poly5,  poly6,  poly7,
                             poly8, poly9, poly10, poly11, poly12, poly13, poly14;

    static std::vector<Poly> const polyset;
};

Poly const polygons::frame({{0,0},  {0,12}, {12,12}, {12,0}});
Poly const polygons::poly1({{0,0},  {3,0},  {2,4}});
Poly const polygons::poly2({{3,0},  {3,6},  {2,4}});
Poly const polygons::poly3({{3,0},  {6,0},  {6,6},    {4,8}, {3,6}});
Poly const polygons::poly4({{0,0},  {4,8},  {2,10}});
Poly const polygons::poly5({{0,0},  {2,10}, {0,12}});
Poly const polygons::poly6({{0,12}, {4,8},  {6,6}});
Poly const polygons::poly7({{4,8},  {6,6},  {6,12}});
Poly const polygons::poly8({{6,0},  {8,4},  {6,6}});
Poly const polygons::poly9({{6,0},  {12,0}, {8,4}});
Poly const polygons::poly10({{8,4},  {12,0}, {9,6}});
Poly const polygons::poly11({{9,6},  {12,0}, {12,6}});
Poly const polygons::poly12({{9,6},  {12,6}, {12,8}});
Poly const polygons::poly13({{6,6},  {8,4},  {10,6},   {6,12}});
Poly const polygons::poly14({{9,6},  {12,8}, {12,12},  {6,12}});

std::vector<Poly> const polygons::polyset{polygons::poly1,  polygons::poly2,   polygons::poly3,
                        polygons::poly4,  polygons::poly5,  polygons::poly6,   polygons::poly7, 
                        polygons::poly8,  polygons::poly9,  polygons::poly10,  polygons::poly11, 
                        polygons::poly12, polygons::poly13, polygons::poly14};

int main(){
    
    std::cout << "Frame:" << std::endl;
    polygons::frame.print();

    int i = 0;
    for(auto& polygon: polygons::polyset){
        std::cout << "polygon " << i << std::endl;
        polygon.print();
        i++;
    }
}