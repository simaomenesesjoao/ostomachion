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

template <int... Ints>
Number<Ints...> Point<Ints...>::dot(Point<Ints...> const& other){
    return x*other.x + y*other.y;
}

bool edges_intersect(Point<Ints...> const& P1, Point<Ints... > const& P2, Point<Ints...> const& Q1, Points<Ints...> const& Q2){
    using V = Points<Ints...>;
    using Num = Number<Ints...>;
    V q{Q2-Q1}, p{P2-P1}, v{P1-Q1};

    // Check if the edges are parallel
    if(q.get_x()*p.get_y() == q.get_y()*p.get_x()){
        return false;
    }
    
    // Find vectors perpendicular to each of the edges
    V perp_q{q.get_y(), -q.get_x()}, perp_p{p.get_y(), -p.get_x()};
    Num pdv = perp_p.dot(v);
    Num qdv = perp_q.dot(v);

    return 0 < pdv && 0 < qdv && -pdv/perp_p.dot(q) < 1 && qdv/perp_q.dot(p);


}


#endif // PVECTORS_C
