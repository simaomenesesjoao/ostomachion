#ifndef PVECTORS_H
#define PVECTORS_H

#include "numbers.cpp"

template <int... Ints> class Point;

template <int... Ints>
class Angle{
    using Num = Number<Ints...>;
    using Point = Point<Ints...>;
public:
    Angle(int x, int y);
    Angle(Num const& c, Num const& s);
    Angle(Point const& point);
    Angle operator+(Angle const& a) const;
    Angle operator-(Angle const& a) const;
    Num get_cos() const;
    Num get_sin() const;
    // template <typename T> T to() const;
private:
    Num sin, cos;

};


template <int... Ints>
class Point{
    using Num = Number<Ints...>;
    using Ang = Angle<Ints...>;
public:
    Point(Num const& x, Num const& y);
    Point(int x, int y);
    Point rotate(Ang const& a) const;
    Point operator+(Point const& P) const;
    Point operator-(Point const& P) const;
    Point operator*(Num const& x) const;
    Point operator==(Num const& x) const;

    Num distance(Num const& x) const;
    Num get_x() const;
    Num get_y() const;

private:
    Num x, y;
};



#endif // PVECTORS_H