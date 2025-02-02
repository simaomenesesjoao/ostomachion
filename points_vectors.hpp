#ifndef PVECTORS_H
#define PVECTORS_H

#include "numbers.cpp"

template <int... Ints> class Point;

template <int... Ints>
class Angle{
    using Num = Number<Ints...>;
    using Poin = Point<Ints...>;
public:
    Angle(int x, int y);
    Angle(Num const& c, Num const& s);
    Angle(Poin const& point);
    Angle();
    Angle operator+(Angle const& a) const;
    Angle operator-(Angle const& a) const;
    Angle operator-() const;
    bool operator==(Angle const& a) const;
    bool operator!=(Angle const& a) const;
    bool operator<(Angle const& a) const;
    // bool operator>(Angle const& a) const;
    bool is_zero() const;
    bool is_larger_than_180() const;
    Num get_cos() const;
    Num get_sin() const;
    template <typename T> explicit operator T() const;

    template <int... Args>
    friend std::ostream& operator<<(std::ostream&, Angle<Args...> const &);
    
    double angle_double;
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
    Point operator-() const;
    Point operator*(Num const& x) const;
    bool operator==(Point const& P) const;
    operator std::pair<Number<Ints...>, Number<Ints...>>() const;


    // Num distance(Num const& x) const;
    Num get_x() const;
    Num get_y() const;
    Num dot(Point const&) const;
    Num cross(Point const&) const;

    template <int... Args>
    friend std::ostream& operator<<(std::ostream&, Point<Args...> const &);
    void print();

private:
    Num x, y;
};

template <int... Ints>
bool edges_intersect(Point<Ints...> const& P1, Point<Ints... > const& P2, Point<Ints...> const& Q1, Point<Ints...> const& Q2);

#endif // PVECTORS_H