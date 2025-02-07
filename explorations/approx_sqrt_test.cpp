#include "approx_sqrt.cpp"
#include <iostream>

int main(){
    static_assert(nearest_perfect_sqrt(10)==3);
    static_assert(nearest_perfect_sqrt(6)==2);
    static_assert(nearest_perfect_sqrt(15)==3);
    static_assert(nearest_perfect_sqrt(16)==4);
    static_assert(nearest_perfect_sqrt(20)==4);

    constexpr std::pair<int, int> pair = find_fractional_approximation(2,1);
    static_assert(pair.first == 3);
    static_assert(pair.second == 2);

    constexpr std::pair<int, int> pair2 = find_fractional_approximation(2,2);
    static_assert(pair2.first == 7);
    static_assert(pair2.second == 5);

    constexpr std::pair<int, int> pair3 = find_fractional_approximation(2,3);
    static_assert(pair3.first == 17);
    static_assert(pair3.second == 12);

    constexpr std::pair<int, int> pair4 = find_fractional_approximation(6,4);
    static_assert(pair4.first == 218);
    static_assert(pair4.second == 89);
    double pair4_d = (double)pair4.first/pair4.second;

    std::cout << pair4.first << " " << pair4.second << " " << pair4_d << std::endl;

    constexpr long N = 21;
    constexpr Limits limits = find_bounds(N, 1e-6);
    constexpr double lower = (double)limits.lower_num/limits.lower_den;
    constexpr double upper = (double)limits.upper_num/limits.upper_den;
    static_assert(lower*lower < N and upper*upper > N);
    std::cout << lower*lower - N << " " << upper*upper - N << std::endl;
    
    std::cout << "here" << std::endl;
    using T = long;
    constexpr auto array = precompute_array<T, 3, 2, 3, 5>();
    for(auto& bounds: array){
        T a = bounds;
        std::cout << a << std::endl;
    }

    std::cout << " \n";
    constexpr auto arr = populate_array<T, 2, 3, 5>();

    for(auto& comb: array){
        auto& limits = arr[comb-1];

        double lower = (double)limits.lower_num/limits.lower_den;
        double upper = (double)limits.upper_num/limits.upper_den;
        std::cout << comb << " " << lower*lower - comb << " " <<  upper*upper - comb << std::endl;

    }
    // std::cout << pow2<4>();
}