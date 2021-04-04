#include<stdio.h>

#include"src/mat.hpp"
#include"src/f32.hpp"
#include"src/f64.hpp"

#include <stdalign.h>
#include <cstdlib>
#include <chrono>
#include <iostream>

int main() {
    sleipnir::f32 data0[16];
    sleipnir::f32 data1[16];

    for(int i=0; i<16;i++) {
        data0[i] = sleipnir::f32(i+1);
        data1[i] = sleipnir::f32(i+1);
    }

    sleipnir::matrix<4, 4> a(data0);
    a.print();

    sleipnir::matrix<4, 4> b(data1);


    sleipnir::matrix<4, 4> c0 = sleipnir::matrix<4, 4>::block_mul(a, b);

    c0.print();

    sleipnir::f32 data2[8];
    sleipnir::f32 data3[8];

    for(int i=0; i<8;i++) {
        data2[i] = sleipnir::f32(i+1);
        data3[i] = sleipnir::f32(i+1);
    }

    sleipnir::matrix<2, 4> d(data2);
    sleipnir::matrix<4, 2> e(data3);

    d.print();
    e.print();

    
    sleipnir::matrix<4,4> f0 = sleipnir::matrix<4,4>::block_mul(e, d);
    
    f0.print();
    
    sleipnir::matrix<2,2> f1 = sleipnir::matrix<2,2>::block_mul(d, e);
    sleipnir::matrix<4,4> f2 = sleipnir::matrix<4,4>::gpu_mul(a, a);


    f1.print();
    f2.print();
}