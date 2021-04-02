#ifndef F64_H
#define F64_H

#include <immintrin.h>
#include <stdio.h>
#include "types.hpp"

union f64x4 {
    __m256d data;
    f64 vec[4];
};


inline f64x4 f64x4_new(f64 a, f64 b, f64 c, f64 d) {
    f64x4 v;
    v.data = _mm256_set_pd(a,b,c,d);
    return v;
}

inline f64x4 f64x4_add(f64x4 a, f64x4 b) {
    f64x4 v;
    v.data = _mm256_add_pd(a.data, b.data);
    return v;
}

inline f64x4 f64x4_sub(f64x4 a, f64x4 b) {
    f64x4 v;
    v.data = _mm256_sub_pd(a.data, b.data);
    return v;
}

inline f64x4 f64x4_mul(f64x4 a, f64x4 b) {
    f64x4 v;
    v.data = _mm256_mul_pd(a.data, b.data);
    return v;
}

inline f64x4 f64x4_div(f64x4 a, f64x4 b) {
    f64x4 v;
    v.data = _mm256_div_pd(a.data, b.data);
    return v;
}

inline f64x4 f64x4_sqr(f64x4 a, f64x4 b) {
    f64x4 v;
    v.data = _mm256_sqrt_pd(a.data);
    return v;
}


inline f64x4 f64x4_min(f64x4 a, f64x4 b) {
    f64x4 v;
    v.data = _mm256_min_pd(a.data, b.data);
    return v;
}

inline f64x4 f64x4_rcp(f64x4 a) {
    f64x4 v;
    v.data = _mm256_rcp14_pd(a.data);
    return v;
}


inline f64x4 f64x4_dot(f64x4 a, f64x4 b) {
    f64x4 v;
    v.data = _mm256_dp_ps(a.data, b.data, 0xff);
    return v;
}

inline f64 f64x4_sum(f64x4 a) {
    f64x4 v;
    v.data = _mm256_hadd_pd(a.data,a.data);
    v.data = _mm256_hadd_pd(v.data,v.data);
    return v.vec[0];
}


inline void f64_sqrt_mat_mul_block(i32 n, i32 si, i32 sj, i32 sk, const f64* A, const f64* B, f64* C, i32 unroll = 4, i32 block_size = 32) {
    for(int i=si; i<si+block_size; i+=unroll*4) {
        for(int j= sk; j<sk+block_size; j++) {
            __m256d c[4];
            for(int x=0; x<unroll; x++) {
                c[x] = _mm256_load_pd(&C[i+(x*4)+(j*n)]);   
            }

            for(int k=sk; k<sk+block_size; k++) {
                __m256d b = _mm256_broadcast_sd(&B[k+j*n]);

                for(int x=0; x<unroll; x++) 
                    c[x] = _mm256_add_pd(c[x], _mm256_mul_pd(_mm256_load_pd(&A[n*k+x*4+i]), b));
            }

            for(int x=0; x<unroll; x++) {
                _mm256_store_pd(&C[i+x*4+j*n], c[x]);
            }
        }
    }
}



inline void f64_sqrt_mat_mul(int n, const f64* A, const f64* B, f64* C, i32 block_size = 32, i32 unroll = 4) {
    for(int sj=0; sj<n; sj+=block_size)
        for(int si=0; si<n; si+=block_size)
            for(int sk=0; sk<n; sk+= block_size)
                f64_sqrt_mat_mul_block(n, si, sj, sk, A,B,C, unroll, block_size);

    for(int i=0; i<(n*n); i+= 4*unroll) {
        __m256d c[4];
        for(int x=0; x<unroll; x++)
            c[x] = _mm256_load_pd(&C[i+(x*4)]);   

        for(int x=0; x<unroll; x++) {
            c[x] = _mm256_div_pd(c[x], _mm256_set1_pd(n/f64(block_size)));
            _mm256_store_pd(&C[i+x*4], c[x]);
        }
    }
}

#endif