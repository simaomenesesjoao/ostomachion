#pragma once

#include "numbers.cpp"


template <typename Num>
class Angle{
    
public:
    Angle(int x, int y);
    Angle(Num const& c, Num const& s);
    Angle();
    Angle(Angle&&);
    Angle(const Angle&);
    Angle& operator=(const Angle&);
    Angle& operator=(Angle&&);

    Angle operator+(Angle const& a) const;
    Angle operator-(Angle const& a) const;
    Angle operator-() const;
    bool operator==(Angle const& a) const;
    bool operator!=(Angle const& a) const;
    bool operator<(Angle const& a) const;
    // bool operator>(Angle const& a) const;
    bool is_zero() const;
    bool is_180() const;
    bool is_larger_than_180() const;
    Num get_cos() const;
    Num get_sin() const;
    template <typename U> explicit operator U() const;

    template <typename N>
    friend std::ostream& operator<<(std::ostream&, Angle<N> const &);
    
    double angle_double;
private:
    Num sin, cos;

};


template <typename Num>
class Point{
    
public:
    Point(Num const& x, Num const& y);
    Point(int x, int y);
    Point(Point&&);
    Point(const Point&);
    Point& operator=(Point&&);
    Point& operator=(const Point&);
    
    Point rotate(Angle<Num> const& a) const;
    Point operator+(Point const& P) const;
    Point operator-(Point const& P) const;
    Point operator-() const;
    Point operator*(Num const& x) const;
    bool operator==(Point const& P) const;
    operator std::pair<Num, Num>() const;

    Num get_x() const;
    Num get_y() const;
    Num dot(Point const&) const;
    Num cross(Point const&) const;

    template <typename N>
    friend std::ostream& operator<<(std::ostream&, Point<N> const &);
    void print();

private:
    Num x, y;
};
