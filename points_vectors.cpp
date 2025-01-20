#ifndef PVECTORS_C
#define PVECTORS_C

#include <cmath>
#include "points_vectors.hpp"
#include <iostream>

template <int... Ints>
Angle<Ints...>::Angle(int x, int y){
    int norm2 = x*x + y*y;
    cos = Num{FracRoot<Ints...>{{x,norm2},norm2}};
    sin = Num{FracRoot<Ints...>{{y,norm2},norm2}};
}

template <int... Ints>
Angle<Ints...>::Angle(Num const& cos, Num const& sin):
    sin(sin), cos(cos){}

// template <int... Ints>
// Angle<Ints...>::Angle(Poin const& P){
//     Number<Ints...> x = P.get_x();
//     Number<Ints...> y = P.get_y();
//     Number<Ints...> norm2 = x*x + y*y;

//     // cos = ? ;
// }


template <int... Ints>
Angle<Ints...>::Angle(){};


template <int... Ints>
Number<Ints...> Angle<Ints...>::get_cos() const{ return cos;}

template <int... Ints>
Number<Ints...> Angle<Ints...>::get_sin() const{ return sin;}


template <int... Ints>
Angle<Ints...> Angle<Ints...>::operator+(Angle const& a) const{
    Num new_cos = cos*a.cos - sin*a.sin;
    Num new_sin = sin*a.cos + cos*a.sin;
    return Angle(new_cos, new_sin);
}

template <int... Ints>
Angle<Ints...> Angle<Ints...>::operator-(Angle const& a) const{
    Num new_cos = cos*a.cos + sin*a.sin;
    Num new_sin = sin*a.cos - cos*a.sin;
    return Angle(new_cos, new_sin);
}

template <int... Ints>
Angle<Ints...> Angle<Ints...>::operator-() const{
    return Angle(-cos, -sin);
}

template <int... Ints>
bool Angle<Ints...>::operator==(Angle const& a) const{
    return sin == a.sin && cos == a.cos;
}


template <int... Ints>
bool Angle<Ints...>::operator!=(Angle const& a) const{
    return !(a==*this);
}


template <int... Ints>
bool Angle<Ints...>::is_zero() const{
    return cos == 1 && sin == 0;
}


template <int... Ints>
std::ostream& operator<<(std::ostream& stream, Angle<Ints...> const& angle){
    stream << "ang=(" << angle.get_sin() << " " << angle.get_cos() << ")";
    return stream;    
}

template <int... Ints>
template <typename T>
Angle<Ints...>::operator T() const{
    T pi = 2*std::acos(0.0);
    T angle = std::atan2((T)sin, (T)cos)/pi*180;
    if(angle<0)
        angle += 360;
    return angle;
};


template <int... Ints>
Point<Ints...>::Point(int x, int y):x{Num{x}}, y{Num{y}}{}

template <int... Ints>
Point<Ints...>::Point(Num const& x, Num const& y):x{x}, y{y}{}

template <int... Ints>
Point<Ints...> Point<Ints...>::operator+(Point const& P) const{
    return Point{x + P.x, y + P.y};
}

template <int... Ints>
Point<Ints...> Point<Ints...>::operator-(Point const& P) const{
    return Point{x - P.x, y - P.y};
}

template <int... Ints>
Point<Ints...> Point<Ints...>::operator*(Number<Ints...> const& n) const{
    return Point{x*n, y*n};    
}

template <int... Ints>
bool Point<Ints...>::operator==(Point const& P) const{
    return x == P.x && y == P.y;
}

template <int... Ints>
Point<Ints...> Point<Ints...>::rotate(Ang const& a) const{
    return Point{x*a.get_cos() - y*a.get_sin(), x*a.get_sin() + y*a.get_cos()};  
}

template <int... Ints>
void Point<Ints...>::print(){
    std::cout << "(" << (double)x << "," << (double)y << ")" << std::endl;
}

template <int... Ints>
Number<Ints...> Point<Ints...>::get_x() const{
    return x;  
}

template <int... Ints>
Number<Ints...> Point<Ints...>::get_y() const{
    return y;  
}

template <int... Ints>
std::ostream& operator<<(std::ostream& stream, Point<Ints...> const& point){
    stream << "(x,y)=(" << point.get_x() << " " << point.get_y() << ")";
    return stream;    
}



#endif // PVECTORS_C
