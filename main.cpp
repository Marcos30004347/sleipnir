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

int main() {
    f32 data0[16];
    f32 data1[16];
    for(int i=0; i<16;i++) {
        data0[i] = f32(i+1);
        data1[i] = f32(i+1);
    }

    matrix<4, 4, 2, 2> a(data0);
    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            std::cout << a.get(i,j) << " "; 
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    matrix<4, 4, 2, 2> b(data1);

    matrix<4, 4> c = matrix<4, 4>::block_mul(a, b);

    for(int i=0; i<4; i++) {
        for(int j=0; j<4; j++) {
            std::cout << c.get(i,j) << " "; 
        }
        std::cout << std::endl;
    }

    f32 data2[8];
    f32 data3[8];

    for(int i=0; i<8;i++) {
        data2[i] = f32(i+1);
        data3[i] = f32(i+1);
    }

    matrix<2, 4> d(data2);
    for(int i=0; i<2; i++) {
        for(int j=0; j<4; j++) {
            std::cout << d.get(i,j) << " "; 
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    matrix<4,2> e(data3);
    for(int i=0; i<4; i++) {
        for(int j=0; j<2; j++) {
            std::cout << e.get(i,j) << " "; 
        }
        std::cout << std::endl;
    }

    std::chrono::steady_clock::time_point begin;
    std::chrono::steady_clock::time_point end;
    
    begin = std::chrono::steady_clock::now();
    matrix<2,2> f0 = matrix<2,2>::mul(d, e);
    end = std::chrono::steady_clock::now();
    long long t0 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    std::cout << "Time difference = " << t0 << "[ns]" << std::endl;

    begin = std::chrono::steady_clock::now();
    matrix<2,2> f1 = matrix<2,2>::block_mul(d, e);
    end = std::chrono::steady_clock::now();
    long long t1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    std::cout << "Time difference = " << t1 << "[ns]" << std::endl;


    for(int i=0; i<2; i++) {
        for(int j=0; j<4; j++) {
            std::cout << d.get(i,j) << " "; 
        }
        std::cout << std::endl;
    }

    std::cout << std::endl;

    for(int i=0; i<4; i++) {
        for(int j=0; j<2; j++) {
            std::cout << e.get(i,j) << " "; 
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) {
            std::cout << f0.get(i,j) << " "; 
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    for(int i=0; i<2; i++) {
        for(int j=0; j<2; j++) {
            std::cout << f1.get(i,j) << " "; 
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;


    // // 4/4 = 1; -> 1

    // // 8/4 = 2; -> 2
    // // 12/4 = 3; -> 3
    // // 16/4 = 4; -> 4
    // // 20/4 = 5; -> 2

    // // alignas(16) f32 a[n*n] ;
    // // alignas(16) f32 b[n*n];
    // // alignas(16) f32 c[n*n];


    // i32 n = 64;

    // printf("n** %i\n", n*n);

    // f64 a[n*n] ;
    // f64 b[n*n];
    // f64 c[n*n];

    // for(int i=0; i<n*n; i++) {
    //     a[i] = 0.0;        
    //     b[i] = 0.0;        
    //     c[i] = 0.0;        
    // }

    // for(int i=0; i<n; i++) {
    //     a[(i*n) + i] = 1.0;
    //     b[(i*n) + i] = 1.0;        
    // }


    // set(a, n, 0, 0, 1);
    // set(a, n, 0, 1, 1);
    // set(a, n, 0, 2, 1);
    // set(a, n, 0, 3, 1);
    // set(a, n, 1, 0, 1);
    // set(a, n, 1, 1, 1);
    // set(a, n, 1, 2, 1);
    // set(a, n, 1, 3, 1);
    // set(a, n, 2, 0, 1);
    // set(a, n, 2, 1, 1);
    // set(a, n, 2, 2, 1);
    // set(a, n, 2, 3, 1);
    // set(a, n, 3, 0, 1);
    // set(a, n, 3, 1, 1);
    // set(a, n, 3, 2, 1);
    // set(a, n, 3, 3, 1);

    // set(b, n, 0, 0, 1);
    // set(b, n, 0, 1, 1);
    // set(b, n, 0, 2, 1);
    // set(b, n, 0, 3, 1);
    // set(b, n, 1, 0, 1);
    // set(b, n, 1, 1, 1);
    // set(b, n, 1, 2, 1);
    // set(b, n, 1, 3, 1);
    // set(b, n, 2, 0, 1);
    // set(b, n, 2, 1, 1);
    // set(b, n, 2, 2, 1);
    // set(b, n, 2, 3, 1);
    // set(b, n, 3, 0, 1);
    // set(b, n, 3, 1, 1);
    // set(b, n, 3, 2, 1);
    // set(b, n, 3, 3, 1);


    // std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    // f64_sqrt_mat_mul(n, a, b, c, 16);
    // std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    

    // for(int i=0; i<n; i++) {
    //     for(int j=0; j<n; j++) {
    //         printf("%.0f ", c[i*n+j]);
    //     }
    //     printf("\n");
    // }
    // long long t0 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    // begin = std::chrono::steady_clock::now();

    // naive_mat_mul(a,b,c, n);

    // end = std::chrono::steady_clock::now();
    // long long t1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count();
    // std::cout << "Time difference = " << t1/f64(t0) << "[ns]" << std::endl;

}