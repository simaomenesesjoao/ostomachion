#ifndef PVECTORS_C
#define PVECTORS_C

#include <cmath>
#include "points_vectors.hpp"

template <int... Ints>
Angle<Ints...>::Angle(int x, int y){
    int norm2 = x*x + y*y;
    cos = Num{FracRoot<Ints...>{{x,norm2},norm2}};
    sin = Num{FracRoot<Ints...>{{y,norm2},norm2}};
}

template <int... Ints>
Angle<Ints...>::Angle(Num const& cos, Num const& sin):
    cos(cos), sin(sin){}


template <int... Ints>
Angle<Ints...>::Angle(Point const& P):
    cos(P.get_x()), sin(P.get_y){}


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
template <typename T>
T Angle<Ints...>::to() const{
    T pi = 2*std::acos(0.0);
    T angle = std::atan2(sin.template to<T>(), cos.template to<T>())/pi*180;
    if(angle<0)
        angle += 180;
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
Point<Ints...> Point<Ints...>::rotate(Ang const& a) const{
    return Point{x*a.get_cos() - y*a.get_sin(), x*a.get_sin() + y*a.get_cos()};  
}

template <int... Ints>
Number<Ints...> Point<Ints...>::get_x() const{
    return x;  
}

template <int... Ints>
Number<Ints...> Point<Ints...>::get_y() const{
    return y;  
}




#endif // PVECTORS_C
