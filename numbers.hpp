#ifndef NUMBERS_H
#define NUMBERS_H

#include <cmath>
#include <ostream>
#include <map>

class Fraction{

public:
    Fraction(int n, int d);

    int get_num() const;
    int get_den() const;

    friend std::ostream& operator<<(std::ostream& os, Fraction const & frac);

    Fraction operator-() const;
    Fraction operator*(Fraction const& other) const;
    Fraction operator/(Fraction const& other) const;
    friend Fraction operator+(Fraction const& x, Fraction const& y);
    friend Fraction operator-(Fraction const& x, Fraction const& y);
    
    
private:
    int num, den;
};


template<int... Ints>
class FracRoot{
public:
    FracRoot();
    FracRoot(const Fraction fraction, int r);

    template<int... Args>
    friend std::ostream& operator<<(std::ostream& os, FracRoot<Args...> const & x);
    FracRoot operator*(FracRoot const & other) const;
    FracRoot operator*(Fraction const & x) const;
    FracRoot operator*(int x) const;
    FracRoot operator-() const;
    FracRoot operator/(FracRoot const & other) const;
    FracRoot operator+(FracRoot const& b) const;
    FracRoot operator+(int b) const;
    FracRoot operator-(FracRoot const & other) const;
    bool operator==(int x) const;
    bool operator!=(int x) const;
    int get_num() const;
    int get_den() const;
    int get_root() const;

private:
    Fraction frac;
    int root;
    std::pair<int, int> remove_perfect_squares(int r) const;

};

template <int... Ints>
class Number{
    
public:
    Number(){};
    template <typename... T> explicit Number(T... digit_list);
    Number(Number const& x);
    template <int...Args>
    friend std::ostream& operator<<(std::ostream & os, Number<Args...> const& number);
    Number  operator+(int x) const;
    Number& operator+=(int x);
    Number  operator+(FracRoot<Ints...> const& x) const;
    Number& operator+=(FracRoot<Ints...> const& x);
    Number  operator+(Number const& a) const;    
    Number& operator+=(Number const& x) const;
    Number  operator-() const;
    Number  operator-(Number const& a) const;
    Number& operator-=(Number const& x);
    Number  operator*(Number const& x) const;
    template <typename T> Number operator*(T const& x) const;
    bool operator==(Number const& x) const;
    bool operator==(int x) const;
    bool operator>(Number const& x);
    bool operator>(int x) const;
    bool operator<(Number const& x) const;
    bool operator<(int x) const;
    template <typename T> explicit operator T() const;

private:
    std::map<int, FracRoot<Ints...>> digits;
    bool is_zero() const;
    template <int A, int... Ints2> friend bool is_pos(Number<A, Ints2...> const& x);
    template <int... Ints2> friend bool is_pos(Number<Ints2...> const& x);
};

template <typename T, typename U> bool equal(T x, U y);


#endif // NUMBERS_H