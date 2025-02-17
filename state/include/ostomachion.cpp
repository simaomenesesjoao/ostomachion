#pragma once

#include <vector>
#include <iostream>
#include <set>
#include "polygon.cpp"

template <typename T>
struct polygons{

    using Poly = Polygon<T>;
    static Poly frame, poly1, poly2, poly3,  poly4,  poly5,  poly6,  poly7,
                             poly8, poly9, poly10, poly11, poly12, poly13, poly14;

    static std::vector<Poly> polyset;
    static unsigned const num_polygons{14};
};

template <typename T> polygons<T>::Poly polygons<T>::frame({{0,0},  {0,12}, {12,12}, {12,0}});
template <typename T> polygons<T>::Poly polygons<T>::poly1({{0,0},  {3,0},  {2,4}});
template <typename T> polygons<T>::Poly polygons<T>::poly2({{3,0},  {3,6},  {2,4}});
template <typename T> polygons<T>::Poly polygons<T>::poly3({{3,0},  {6,0},  {6,6},    {4,8}, {3,6}});
template <typename T> polygons<T>::Poly polygons<T>::poly4({{0,0},  {4,8},  {2,10}});
template <typename T> polygons<T>::Poly polygons<T>::poly5({{0,0},  {2,10}, {0,12}});
template <typename T> polygons<T>::Poly polygons<T>::poly6({{0,12}, {4,8},  {6,12}});
template <typename T> polygons<T>::Poly polygons<T>::poly7({{4,8},  {6,6},  {6,12}});
template <typename T> polygons<T>::Poly polygons<T>::poly8({{6,0},  {8,4},  {6,6}});
template <typename T> polygons<T>::Poly polygons<T>::poly9({{6,0},  {12,0}, {8,4}});
template <typename T> polygons<T>::Poly polygons<T>::poly10({{8,4},  {12,0}, {9,6}});
template <typename T> polygons<T>::Poly polygons<T>::poly11({{9,6},  {12,0}, {12,6}});
template <typename T> polygons<T>::Poly polygons<T>::poly12({{9,6},  {12,6}, {12,8}});
template <typename T> polygons<T>::Poly polygons<T>::poly13({{6,6},  {8,4},  {9,6},   {6,12}});
template <typename T> polygons<T>::Poly polygons<T>::poly14({{9,6},  {12,8}, {12,12},  {6,12}});

template <typename T>
std::vector<typename polygons<T>::Poly> polygons<T>::polyset{
        polygons::poly1,  polygons::poly2,  polygons::poly3,
        polygons::poly4,  polygons::poly5,  polygons::poly6,   polygons::poly7, 
        polygons::poly8,  polygons::poly9,  polygons::poly10,  polygons::poly11, 
        polygons::poly12, polygons::poly13, polygons::poly14};
