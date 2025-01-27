#ifndef NUMBERS_H
#define NUMBERS_H

#include <cmath>
#include <ostream>
#include <map>

template <typename T>
class Fraction{

public:
    Fraction(T n, T d);

    T get_num() const;
    T get_den() const;

    template <typename U>
    friend std::ostream& operator<<(std::ostream& os, Fraction<U> const & frac);

    Fraction operator-() const;
    Fraction operator*(Fraction const& other) const;
    Fraction operator/(Fraction const& other) const;
    template <typename W, typename U> friend bool operator<(U, Fraction<T> const&);
    template <typename W, typename U> friend bool operator<(Fraction<W> const&, U);
    template <typename U> friend bool operator<(Fraction<U> const&, Fraction<U> const&);
    template <typename U> friend Fraction<U> operator+(Fraction<U> const& x, Fraction<U> const& y);
    template <typename U> friend Fraction<U> operator-(Fraction<U> const& x, Fraction<U> const& y);
    
private:
    T num, den;
};


template<int... Ints>
class FracRoot{
public:
    using T = int;
    FracRoot();
    FracRoot(Fraction<T> const& fraction, int r);

    template<int... Args>
    friend std::ostream& operator<<(std::ostream& os, FracRoot<Args...> const & x);
    FracRoot operator*(FracRoot const & other) const;
    FracRoot operator*(Fraction<T> const & x) const;
    FracRoot operator*(T x) const;
    FracRoot operator-() const;
    FracRoot operator/(FracRoot const & other) const;
    FracRoot operator+(FracRoot const& b) const;
    FracRoot operator+(T b) const;
    FracRoot operator-(FracRoot const & other) const;
    bool operator==(T x) const;
    bool operator!=(T x) const;
    T get_num() const;
    T get_den() const;
    int get_root() const;

private:
    Fraction<T> frac;
    int root;
    std::pair<int, int> remove_perfect_squares(int r) const;

};

template <int... Ints>
class Number{
    using T = int;
    
public:
    Number(){};
    template <typename... U> explicit Number(U... digit_list);
    Number(Number const& x);
    template <int...Args>
    friend std::ostream& operator<<(std::ostream & os, Number<Args...> const& number);
    Number  operator+(T x) const;
    Number& operator+=(T x);
    Number  operator+(Fraction<T> const& x) const;
    Number& operator+=(Fraction<T> const& x);
    Number  operator+(FracRoot<Ints...> const& x) const;
    Number& operator+=(FracRoot<Ints...> const& x);
    Number  operator+(Number const& a) const;    
    Number& operator+=(Number const& x) const;
    Number  operator-() const;
    Number  operator-(Number const& a) const;
    Number& operator-=(Number const& x);
    Number  operator*(Number const& x) const;
    Number  operator/(Number const& x) const;
    template <typename U> Number operator*(U const& x) const;
    bool operator==(Number const& x) const;
    bool operator==(int x) const;
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

private:
    std::map<int, FracRoot<Ints...>> digits;
    template <int A, int... Ints2> friend bool is_pos(Number<A, Ints2...> const& x);
    template <int... Ints2> friend bool is_pos(Number<Ints2...> const& x);
};

template <typename T, typename U> bool equal(T x, U y);


#endif // NUMBERS_H