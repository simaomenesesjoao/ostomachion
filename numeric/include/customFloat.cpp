#include <ostream>
#include <iostream>
#include <cmath>

template <typename T>
class Float{
    
public:
    using Type = T;

// Constructors and assignment
    Float():x{0}{};
    template <typename... U> explicit Float(U... ys){
        x = (ys + ...);
    }

    Float(int n, int d, int r){ 
        x = (T)n/(T)d*sqrt(T(r));
    }

    Float(Float const& f):x{f.x}{};

    Float& operator=(Float const& f){ 
        x = f.x;
        return *this;
    }

    // Binary and unary operators
    template <typename U>
    Float  operator+(U other) const { 
        Float ff{*this};
        ff.x += (T)other;
        return ff;
    }
    template <typename U>
    Float& operator+=(U other){
        x += (U)other;
        return *this;
    }

    // Float  operator+(Fraction<T> const& x) const;
    // Float& operator+=(Fraction<T> const& x);
    // Float  operator+(FracRoot<T, Ints...> const& x) const;
    // Float& operator+=(FracRoot<T, Ints...> const& x);
    Float  operator+(Float const& other) const {
        Float ff{*this};
        ff.x += other.x;
        return ff;
    } 
    Float& operator+=(Float const& other){
        x += other.x;
        return *this;
    }
    Float  operator-() const{
        Float ff{*this};
        ff.x = -ff.x;
        return ff;
    }
    Float  operator-(Float const& other) const {
        Float ff{0};
        ff.x = x - other.x;
        return ff;
    }

    Float& operator-=(Float const& other){
        x -= other.x;
        return *this;
    }

    Float  operator*(Float const& other) const {
        Float ff;
        ff.x = x*other.x;
        return ff;
    }

    Float  operator/(Float const& other) const {
        Float ff;
        ff.x = x/other.x;
        return ff;        
    }
    Float  operator/(int other) const{
        Float ff{*this};
        ff.x /= (T)other;
        return ff;
    }

    template <typename U> Float operator*(U const& other) const {
        Float ff;
        ff.x = x*other;
        return ff;
    }

    // Comparison operators
    bool operator==(Float const& other) const {
        return abs(x - other.x) < tol;
    }

    bool operator==(int other) const {
        return abs(x - other) < tol;
    }

    template <typename U> bool operator!=(U const& other) const {
        return abs(x - other) > tol;
    }

    bool operator>(Float const& other) const {
        return x > other.x;
    }

    template <typename U> bool operator>(U other) const {
        return x > other;
    }

    bool operator<(Float const& other) const {
        return x < other.x;
    }

    template <typename U> bool operator<(U other) const {
        return x < other;
    }

    template <typename U> bool operator>=(U const& other) const {
        return !(x < other);
    }
    template <typename U> bool operator<=(U const& other) const {
        return !(x > other);
    }

    template <typename U> explicit operator U() const {
        return (U)x;
    };
    // bool is_zero() const;
    // Float conjugate(int) const;
    // Float inverse() const;

    template <typename U, int...Args>
    friend std::ostream& operator<<(std::ostream & os, Float<U> const& Float);

private:
    T x;
    T tol = 1e-6;
};

template <typename T, int... Ints>
std::ostream& operator<<(std::ostream & os, Float<T> const& ff){
    os << ff.x;
    return os;
}