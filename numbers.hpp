#ifndef NUMBERS_H
#define NUMBERS_H

#include <cmath>
#include <ostream>
#include <map>
#include "fraction.cpp"
#include "fracroot.cpp"

template <typename T = int, int... Ints>
class Number{
    // using T = int-;
    
public:
    Number(){};
    template <typename... U> explicit Number(U... digit_list);
    Number(Number const& x);
    Number& operator=(Number const&);


    template <typename U, int...Args>
    friend std::ostream& operator<<(std::ostream & os, Number<U, Args...> const& number);
    Number  operator+(T x) const;
    Number& operator+=(T x);
    Number  operator+(Fraction<T> const& x) const;
    Number& operator+=(Fraction<T> const& x);
    Number  operator+(FracRoot<T, Ints...> const& x) const;
    Number& operator+=(FracRoot<T, Ints...> const& x);
    Number  operator+(Number const& a) const;    
    Number& operator+=(Number const& x);
    Number  operator-() const;
    Number  operator-(Number const& a) const;
    Number& operator-=(Number const& x);
    Number  operator*(Number const& x) const;
    Number  operator/(Number const& x) const;
    Number  operator/(int x) const;
    template <typename U> Number operator*(U const& x) const;
    bool operator==(Number const& x) const;
    bool operator==(int x) const;
    template <typename U> bool operator!=(U const& x) const;
    bool operator>(Number const& x) const;
    template <typename U> bool operator>=(U const& x) const;
    template <typename U> bool operator<=(U const& x) const;
    bool operator>(int x) const;
    bool operator<(Number const& x) const;
    bool operator<(int x) const;
    template <typename U> explicit operator U() const;
    bool is_zero() const;
    Number conjugate(int) const;
    Number inverse() const;

    double number_double;
private:
    std::map<int, FracRoot<T,Ints...>> digits;
    template <typename U, int A, int... Ints2> friend bool is_pos(Number<U,A, Ints2...> const& x);
    template <typename U, int... Ints2> friend bool is_pos(Number<U,Ints2...> const& x);
};

template <typename T, typename U> bool equal(T x, U y);


#endif // NUMBERS_H