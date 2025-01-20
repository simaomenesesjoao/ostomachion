#ifndef NUMBERS_C
#define NUMBERS_C 

#include "numbers.hpp"
#include <numeric>
#include <vector>

Fraction::Fraction(int n, int d):num{n}, den{d}{
    int gcd = std::gcd(num, den);
    // std::cout << "gcd: " << gcd << std::endl;
    num /= gcd;
    den /= gcd;
}

Fraction Fraction::operator-() const{
    return Fraction(-num, den);
}

Fraction Fraction::operator*(Fraction const& other) const{
    return Fraction(other.num*num, other.den*den);        
}

Fraction Fraction::operator/(Fraction const& other) const{
    return Fraction(other.den*num, other.num*den);        
}

Fraction operator+(Fraction const& x, Fraction const& y){
    return Fraction(x.num*y.den + x.den*y.num, x.den*y.den);        
}

Fraction operator-(Fraction const& x, Fraction const& y){
    return Fraction(y.den*x.num - y.num*x.den, y.den*x.den);
}

int Fraction::get_num() const{
    return num;
}
int Fraction::get_den() const{
    return den;
}

std::ostream& operator<<(std::ostream& os, Fraction const & frac){
    return os << frac.num << "/" << frac.den;
}



template <int... Ints>
std::pair<int, int> FracRoot<Ints...>::remove_perfect_squares(int r) const{
    // Check if there's any perfect square inside r
    std::vector<int> primes{2,3,5,7,11,13,17,19,23,29};

    int num{1}, root{1};

    for(auto& prime: primes){
        // std::cout << prime << " " << r << " " << num << " " << root <<  std::endl;

        if(prime > r) break;

        while(r % (prime*prime) == 0){
            r /= prime*prime;
            num *= prime;
        } 
        if(r % prime == 0){

            if(not ((prime == Ints) or ...)){
                throw std::invalid_argument("Square root argument cannot be represented with the factors provided.");                
            }

            root *= prime;
            r /= prime;
        }
    }

    if( r != 1){
        throw std::invalid_argument("Square root argument cannot be represented with the factors provided.");
    }
    return std::pair<int, int>(num, root);
}

template <int... Ints>
FracRoot<Ints...>::FracRoot():frac{0,1},root{1}{}

template <int... Ints>
FracRoot<Ints...>::FracRoot(const Fraction fraction, int r):frac{fraction},root{1}{

    auto [square, root2] = remove_perfect_squares(r);
    root = root2;
    int num = fraction.get_num()*square;
    int den = fraction.get_den();
    
    frac = Fraction(num, den);
}


template <int... Ints>
std::ostream& operator<<(std::ostream& os, FracRoot<Ints...> const & x){
    return os << x.frac.get_num() << "/" << x.frac.get_den() << "√" << x.root;
}

template <int... Ints>
FracRoot<Ints...> FracRoot<Ints...>::operator*(FracRoot const & other) const{
    auto [square, root2] = remove_perfect_squares(root*other.root);
    int new_num = get_num()*other.get_num()*square;
    int new_den = get_den()*other.get_den();
    return FracRoot{{new_num, new_den}, root2};
}

template <int... Ints>
FracRoot<Ints...> FracRoot<Ints...>::operator*(Fraction const & x) const{
    return FracRoot{{get_num()*x.get_num(), get_den()*x.get_den()}, root};
}

template <int... Ints>
FracRoot<Ints...> FracRoot<Ints...>::operator*(int x) const{
    return FracRoot{{get_num()*x, get_den()}, root};
}

template <int... Ints>
FracRoot<Ints...> FracRoot<Ints...>::operator-() const{
    return FracRoot{-frac, root};
}


template <int... Ints>
FracRoot<Ints...> FracRoot<Ints...>::operator/(FracRoot const & other) const{
    auto [square, root2] = remove_perfect_squares(root*other.root);
    int new_num = get_num()*other.get_den()*square;
    int new_den = get_den()*other.get_num()*other.get_root();
    return FracRoot{{new_num, new_den}, root2};
}

template <int... Ints>
FracRoot<Ints...> FracRoot<Ints...>::operator+(FracRoot const& b) const{
    if(this->get_root() != b.get_root())
        throw std::invalid_argument("Square root argument must be identical when summing.");
    
    return FracRoot{{this->frac + b.frac}, this->get_root()};
}



template <int... Ints>
FracRoot<Ints...> FracRoot<Ints...>::operator+(int b) const{
    if(this->get_root() != 1)
        throw std::invalid_argument("Square root argument must be identical when summing.");
    
    return FracRoot{{this->frac + Fraction(b,1)}, this->get_root()};
}


template <int... Ints>
FracRoot<Ints...> FracRoot<Ints...>::operator-(FracRoot const & other) const{
    if(other.get_root() != get_root())
        throw std::invalid_argument("Square root argument must be identical when subtracting.");
    
    return FracRoot{{frac - other.frac}, get_root()};
}


template <int... Ints>
bool FracRoot<Ints...>::operator==(int x) const{
    return x == get_num() and get_den()==1 and get_root()==1;
}

template <int... Ints>
bool FracRoot<Ints...>::operator!=(int x) const{
    return !(*this==x);
}



template <int... Ints>
int FracRoot<Ints...>::get_num() const{ 
    return frac.get_num();
}

template <int... Ints>
int FracRoot<Ints...>::get_den() const{ 
    return frac.get_den(); 
}

template <int... Ints>
int FracRoot<Ints...>::get_root() const{
    return root;
}










template <int... Ints>
bool Number<Ints...>::is_zero() const{
    return digits.empty();
}

template <int A, int... Ints>
bool is_pos(Number<A, Ints...> const& x){

    if(x == 0){
        return false;
    }
        

    Number<Ints...> a{}, b{}; 
    
    for(auto& [root, digit]: x.digits){
        int num = digit.get_num();
        int den = digit.get_den();
        if(root % A == 0){
            b += FracRoot<Ints...>{Fraction{num, den}, root/A};
        } else {
            a += FracRoot<Ints...>{Fraction{num, den}, root};
        }
    }

    if(a == 0){
        return is_pos(b);
    } else if(b == 0){
        return is_pos(a);
    }

    bool a_pos = is_pos(a);
    bool b_pos = is_pos(b);

    if(a_pos and b_pos) return true;
    if(!a_pos and !b_pos) return false;

    Number<Ints...> c = a*a - b*b*A;
    bool c_pos = is_pos(c);
    return (a_pos and c_pos) or (!a_pos and !c_pos);
}

// This will only match when there's an empty parameter stack
template <int... Ints>
bool is_pos(Number<Ints...> const& x){
    if(x == 0) {
        return false;
    } else {
        auto& [root, digit] = *x.digits.begin();
        return digit.get_num()*digit.get_den() > 0;

    }   
}

template <int... Ints>
template <typename... T>
Number<Ints...>::Number(T... digit_list){
    // std::cout << "Entered constructor T... digit_list" << std::endl;
    Number x{};
    (x += ... += digit_list);
    digits = std::move(x.digits);
}

template <int... Ints>
Number<Ints...>::Number(Number const& x){
    digits = x.digits;
}

template <int... Ints>
std::ostream& operator<<(std::ostream & os, Number<Ints...> const& number){
    if(number.digits.empty()){
        os << "Empty";
    }
    for(auto& digit: number.digits){
        os << digit.second << " "; 
    }
    return os;
}

template <int... Ints>
Number<Ints...> Number<Ints...>::operator+(int x) const{
    Number novo{*this};

    // If the fraction is zero, do nothing
    if(x == 0) 
        return novo;

    if(novo.digits.count(1) == 0){
        novo.digits[1] = FracRoot<Ints...>({x,1},1);
    } else {
        FracRoot<Ints...>& digit = novo.digits[1];
        digit = digit + x;
    }
    return novo;
}


template <int... Ints>
Number<Ints...>& Number<Ints...>::operator+=(int x){
    *this = *this + x;
    return *this;
}


template <int... Ints>
Number<Ints...> Number<Ints...>::operator+(FracRoot<Ints...> const& x) const{
    Number novo{*this};
    const int& r = x.get_root();

    // If the fraction is zero, do nothing
    if(x.get_num() == 0) 
        return novo;

    if(novo.digits.count(r) == 0){
        novo.digits[r] = x;
    } else {
        FracRoot<Ints...>& digit = novo.digits[r];
        digit = digit + x;
    }
    return novo;
}


template <int... Ints>
Number<Ints...>& Number<Ints...>::operator+=(FracRoot<Ints...> const& x){
    *this = *this + x;
    return *this;
}


template <int... Ints>
Number<Ints...> Number<Ints...>::operator+(Number const& a) const{
    auto it1 = digits.begin();
    auto it2 = a.digits.begin();

    Number sum;
    while(it1!=digits.end() and it2!=a.digits.end()){
        const int& root1 = it1->first;
        const int& root2 = it2->first;

        if(root1 == root2){
            FracRoot<Ints...> plus = it1->second + it2->second;
            if(plus != 0)
                sum.digits[root1] = plus;

            it1++;
            it2++;
        } else if(root1 < root2){
            sum.digits[root1] = it1->second;
            it1++;
        } else {
            sum.digits[root2] = it2->second;
            it2++;
        }
    }

    while(it1 != digits.end()){
        sum.digits[it1->first] = it1->second;
        it1++;
    }

    while(it2 != a.digits.end()){
        sum.digits[it2->first] = it2->second;
        it2++;
    }
    return sum;
}


template <int... Ints>
Number<Ints...>& Number<Ints...>::operator+=(Number const& x) const{
    *this = *this+x;
    return *this; 
}



template <int... Ints>
Number<Ints...> Number<Ints...>::operator-() const{

    Number result;
    for(auto &[root, frac]: digits){
        result.digits[root] = -frac;
    }

    return result;
}


template <int... Ints>
Number<Ints...> Number<Ints...>::operator-(Number const& a) const{
    auto it1 = digits.begin();
    auto it2 = a.digits.begin();

    Number result;
    while(it1!=digits.end() and it2!=a.digits.end()){
        const int& root1 = it1->first;
        const int& root2 = it2->first;

        if(root1 == root2){
            FracRoot<Ints...> dif = it1->second - it2->second;
            if(dif != 0)
                result.digits[root1] = dif;

            it1++;
            it2++;
        } else if(root1 < root2){
            result.digits[root1] = it1->second;
            it1++;
        } else {
            result.digits[root2] = -it2->second;
            it2++;
        }
    }

    while(it1 != digits.end()){
        result.digits[it1->first] = it1->second;
        it1++;
    }

    while(it2 != a.digits.end()){
        result.digits[it2->first] = -it2->second;
        it2++;
    }
    return result;
}


template <int... Ints>
Number<Ints...>& Number<Ints...>::operator-=(Number const& x){
    *this = *this-x;
    return *this; 
}


template <int... Ints>
template <typename T>
Number<Ints...>::operator T() const{
    T decimal{};
    for(auto& [root, digit]: digits)
        decimal += (T)digit.get_num()/digit.get_den()*std::sqrt((T)root);
    return decimal;
}


template <int... Ints>
Number<Ints...> Number<Ints...>::operator*(Number const& x) const{
    Number product{};
    for(auto& [root1, digit1]: digits)
        for(auto& [root2, digit2]: x.digits)
            product += digit1*digit2;
        
    return product;
}


template <int... Ints>
template <typename T>
Number<Ints...> Number<Ints...>::operator*(T const& x) const{
    Number product{};
    for(auto& [root, digit]: digits)
        product += digit*x;
        
    return product;
}

    
template <int... Ints>
bool Number<Ints...>::operator==(Number const& x) const{
    // std::cout << "Entering == number" << std::endl;
    Number y{*this-x};
    return y.is_zero();
}

template <int... Ints>
bool Number<Ints...>::operator==(int x) const{
    Number y{x};
    return *this==y;
}

template <int... Ints>
bool Number<Ints...>::operator>(Number const& x) const{
    return is_pos(*this-x);
}

template <int... Ints>
bool Number<Ints...>::operator>(int x) const{
    Number y{x};
    y -= *this;
    return !is_pos(y) and !(y==0);
}

template <int... Ints>
bool Number<Ints...>::operator<(Number const& x) const{
    return !(*this==x) and !(*this>x);
}

template <int... Ints>
bool Number<Ints...>::operator<(int x) const{
    return !(*this==x) and !(*this>x);
}


template <typename T, typename U>
bool equal(T x, U y){
    double tolerance = 1e-8;
    return std::abs(x-y) < tolerance;
}

#endif // NUMBERS_C