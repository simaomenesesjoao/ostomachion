#include <iostream>
#include <bits/stdc++.h>
#include <x86intrin.h>
#include <chrono>
// #include <immintrin.h>

bool overlap(unsigned int NP, unsigned int NQ, double* Px, double* Py, double* Qx, double* Qy){

    for(unsigned i=0; i<NP; i++){
        double vx = Px[i+1] - Px[i];
        double vy = Py[i+1] - Py[i];

        for(unsigned int j=0; j<NQ; j++){
            double proj = (Qx[j] - Px[i])*vy - (Qy[j] - Py[i])*vx;
            if(proj < 0)
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
        x2 = _mm256_broadcast_sd(Qx+j);
        y2 = _mm256_broadcast_sd(Qy+j);

        __m256d x3 = _mm256_sub_pd(x2, mPx);
        __m256d y3 = _mm256_sub_pd(y2, mPy);

        x2 = _mm256_mul_pd(vy, x3);
        y2 = _mm256_mul_pd(vx, y3);

        // x3 = _mm256_sub_pd(x2, y2);

        
        x3 = _mm256_cmp_pd(x2, y2, _CMP_GT_OQ);
    }
    
}

int main(){


}