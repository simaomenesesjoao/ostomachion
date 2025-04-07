#include <iostream>
#include <bits/stdc++.h>
#include <x86intrin.h>
#include <chrono>

#define M 10000000

void scalar(double* a, double* b, double *c){
    auto s1 = std::chrono::high_resolution_clock::now();

    for(unsigned int i = 0; i < M; i++){
        c[i] = a[i] + b[i];
    }


    auto s2 = std::chrono::high_resolution_clock::now()-s1;
    std::cout << c[M/2] << " " << s2.count() << "\n";
}

void vector(double* a, double* b, double* c){
    auto s1 = std::chrono::high_resolution_clock::now();

    __m256d x, y, z;
    for(unsigned int i = 0; i < M; i+=4){
        x = _mm256_loadu_pd(a + i);
        y = _mm256_loadu_pd(b + i);
        z = _mm256_add_pd(x, y);
        _mm256_storeu_pd(c + i, z);
    }
    
    auto s2 = std::chrono::high_resolution_clock::now()-s1;
    std::cout << c[M/2] << " " << s2.count() << "\n";
}



int main(){
    
    double *a1 = new double[M];
    double *b1 = new double[M];
    double *c1 = new double[M];

    double *a2 = new double[M];
    double *b2 = new double[M];
    double *c2 = new double[M];

    for(unsigned int i=0; i<M; i++){
        a1[i] = 1;
        a2[i] = 1;
        b1[i] = 1;
        b2[i] = 1;
    }


    scalar(a1, b1, c1);
    vector(a2, b2, c2);

    delete[] a1;
    delete[] a2;
    delete[] b1;
    delete[] b2;
    delete[] c1;
    delete[] c2;
    
}
