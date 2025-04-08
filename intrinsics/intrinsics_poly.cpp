#include <iostream>
#include <bits/stdc++.h>
#include <x86intrin.h>
#include <chrono>
// #include <immintrin.h>
#include <immintrin.h>
#include <iostream>
#include <bitset>
#include <cstring>

void print_m256d_bits(__m256d vec) {
    // Step 1: Store the __m256d values into a regular array of double
    double values[4];
    _mm256_storeu_pd(values, vec);
    
    // Step 2: For each double in the array, print its bitwise representation
    for (int i = 0; i < 4; ++i) {
        // Step 3: Use union to interpret the bits of the double
        union {
            double d;
            uint64_t u;
        } converter;
        
        converter.d = values[i];
        
        // Step 4: Print the 64-bit bitwise representation of each double
        std::bitset<64> bits(converter.u);
        std::cout << "Double " << i << " value: " << values[i]
                  << " -> Bitwise: " << bits << std::endl;
    }
}


bool overlap(unsigned int NP, unsigned int NQ, double* Px, double* Py, double* Qx, double* Qy){

    for(unsigned i=0; i<NP; i++){
        std::cout << "i:" << i << "\n";
        double vx = Px[i+1] - Px[i];
        double vy = Py[i+1] - Py[i];

        for(unsigned int j=0; j<NQ; j++){
            std::cout << "j:" << j << "\n";
            
            if((Qx[j] - Px[i])*vy < (Qy[j] - Py[i])*vx)
                return true;
        }

    }
    return false;
}


bool overlap_intrinsic(unsigned int NP, unsigned int NQ, double* Px, double* Py, double* Qx, double* Qy){

    __m256d mPx = _mm256_loadu_pd(Px);
    __m256d x2  = _mm256_loadu_pd(Px+1);
    __m256d mPy = _mm256_loadu_pd(Py);
    __m256d y2  = _mm256_loadu_pd(Py+1);

    __m256d vx = _mm256_sub_pd(x2, mPx);
    __m256d vy = _mm256_sub_pd(y2, mPy);

    for(unsigned int j=0; j<NQ; j++){
        std::cout << "j:" << j << "\n";
        x2 = _mm256_broadcast_sd(Qx+j);
        y2 = _mm256_broadcast_sd(Qy+j);

        __m256d x3 = _mm256_sub_pd(x2, mPx);
        __m256d y3 = _mm256_sub_pd(y2, mPy);

        x2 = _mm256_mul_pd(vy, x3);
        y2 = _mm256_mul_pd(vx, y3);

        // x3 = _mm256_sub_pd(x2, y2);

        // std::cout << "-------\nnew\n";
        // print_m256d_bits(x2);
        // print_m256d_bits(y2);
        
        x3 = _mm256_cmp_pd(x2, y2, _CMP_GT_OQ);

        // print_m256d_bits(x3);

        double result[4];
        _mm256_storeu_pd(result, x3);

        if(result[0] && result[1] && result[2] && result[3])
            return true;
        // std::cout << "done\n";

        // __m256d and_first_half = _mm256_and_pd(x3, _mm256_permute2f128_pd(x3, x3, 0x01));
    
        // // Step 2: Reduce the result to a single value by ANDing the lower and upper 128-bits
        // and_first_half = _mm256_and_pd(and_first_half, _mm256_shuffle_pd(and_first_half, and_first_half, 0x0F));

        // // double result[4];
        // // _mm256_storeu_pd(result, and_first_half);

        // print_m256d_bits(and_first_half);

    }

    return false;
    
}

int main(){
    // {

    //     __m256d x1 = {1.0, 2.0, 1.0, 2.0};
    //     // __m256d x1 = {0.0, 0.0, 1.0, 1.0};
    //     __m256d x2 = {0.0, 1.0, -1.0, 1.0};
    //     __m256d x3 = _mm256_cmp_pd(x1, x2, _CMP_GT_OQ);
    //     print_m256d_bits(x3);

    //     __m256d and_first_half = _mm256_and_pd(x3, _mm256_permute2f128_pd(x3, x3, 0x01));
    
    //     // Step 2: Reduce the result to a single value by ANDing the lower and upper 128-bits
    //     and_first_half = _mm256_and_pd(and_first_half, _mm256_shuffle_pd(and_first_half, and_first_half, 0x0F));
    //     print_m256d_bits(and_first_half);

    // }
    const unsigned int M = 1;

    const unsigned int NP{3}, NQ{6};
    double Px[NP+2] = {0,2,2,0,0};
    double Py[NP+2] = {0,0,2,0,0};
    double Qx[NQ+1] = {1,2,1,1,2,1,1};
    double Qy[NQ+1] = {1,1,1,3,3,3,1};


    auto s1 = std::chrono::high_resolution_clock::now();
    bool o1 = false;
    for(unsigned int i=0; i<M; i++){
        o1 = overlap(NP, NQ, Px, Py, Qx, Qy) ^ o1;
    }

    std::cout << "in\n";

    auto s2 = std::chrono::high_resolution_clock::now();
    bool o2 = false;
    for(unsigned int i=0; i<M; i++){
        o2 = overlap_intrinsic(NP, NQ, Px, Py, Qx, Qy) ^ o2;
    }
    auto s3 = std::chrono::high_resolution_clock::now();


    std::cout << o1 << o2 << " " << (s2-s1).count() << " " << (s3-s2).count() << "\n";

}