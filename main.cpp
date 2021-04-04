#include<stdio.h>

#include"src/mat.hpp"
#include"src/f32.hpp"
#include"src/f64.hpp"

#include <stdalign.h>
#include <cstdlib>
#include <chrono>
#include <iostream>


// void set(f64* v, int n, int i, int j, f64 val) {
//     v[i*n+j] = val;
// }

// void naive_mat_mul(f64* a, f64* b, f64* dest, int n) {
//     for(int i=0; i<n; i++) {
//         for(int j=0; j<n; j++) {
//             for(int k=0; k<n; k++) {
//                 dest[i*n+j] += a[j*n+k]* b[k*n+i];
//             }
//         }
//     }
// }


std::chrono::steady_clock::time_point begin;
std::chrono::steady_clock::time_point end;

int main() {
    f32 data0[16];
    f32 data1[16];

    for(int i=0; i<16;i++) {
        data0[i] = f32(i+1);
        data1[i] = f32(i+1);
    }

    matrix<4, 4> a(data0);
    a.print();

    matrix<4, 4> b(data1);


    matrix<4, 4> c0 = matrix<4, 4>::block_mul(a, b);
    c0.print();

    // matrix<4, 4> c1 = matrix<4, 4>::gpu_mul(a, b);
    // c1.print();

    f32 data2[8];
    f32 data3[8];

    for(int i=0; i<8;i++) {
        data2[i] = f32(i+1);
        data3[i] = f32(i+1);
    }

    matrix<2, 4> d(data2);
    d.print();
    matrix<4, 2> e(data3);
    e.print();

    // begin = std::chrono::steady_clock::now();
    
    matrix<4,4> f0 = matrix<4,4>::block_mul(e, d);
    
    f0.print();
    
    matrix<2,2> f1 = matrix<2,2>::block_mul(d, e);
    
    // end = std::chrono::steady_clock::now();
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    // begin = std::chrono::steady_clock::now();
    
    // matrix<2,2> f1 = matrix<2,2>::block_mul(d, e);
    
    // end = std::chrono::steady_clock::now();
    // std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

    // d.print();
    // e.print();

    f1.print();
    // f1.print();
    matrix<4,4> f2 = matrix<4,4>::gpu_mul(a, a);
    f2.print();
}