#include "numbers.cpp"
#include "gmpxx.h"

using T = mpz_class;

template class Number<T,2,5,13,17>;
template class Number<T,2,5>;
template class Number<int,2,5,13,17>;
template class Number<int,2,5>;
template class Number<int,2,3>;
template class Number<int,5>;