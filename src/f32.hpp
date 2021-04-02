#ifndef F32_H
#define F32_H

#include<immintrin.h>
#include<stdio.h>
#include"types.hpp"
#include"i32.hpp"

union f32x4 {
    __m128 data;
    f32 vec[4];
};

// f32 f32x4_get(f32x4 v, int i);
// void f32x4_cp_values(f32x4 v, f32* a);
void f32x4_transpose(f32x4* a, f32x4* b, f32x4* c, f32x4* d);

f32 f32x4_sum(f32x4 a);

f32x4 f32x4_new(f32 a, f32 b, f32 c, f32 d) {
    f32x4 v;
    v.data = _mm_set_ps(a,b,c,d);
    return v;
}


f32x4 f32x4_add(f32x4 a, f32x4 b);
f32x4 f32x4_sub(f32x4 a, f32x4 b);
f32x4 f32x4_mul(f32x4 a, f32x4 b);
f32x4 f32x4_div(f32x4 a, f32x4 b);
f32x4 f32x4_max(f32x4 a, f32x4 b);
f32x4 f32x4_min(f32x4 a, f32x4 b);
f32x4 f32x4_rcp(f32x4 a);
f32x4 f32x4_sqr(f32x4 a);

f32x4 f32x4_as_mat2_mul(f32x4 vec1, f32x4 vec2);
f32x4 f32x4_as_mat2_adj_mul(f32x4 vec1, f32x4 vec2);
f32x4 f32x4_as_mat2_mul_adj(f32x4 vec1, f32x4 vec2);

// inline void f64_sqrt_mat_mul_block(i32 n, i32 si, i32 sj, i32 sk, const f32* A, const f32* B, f32* C, i32 unroll = 16, i32 block_size = 32) {
//     for(int i=si; i<si+block_size; i+=unroll*16) {
//         for(int j= sk; j<sk+block_size; j++) {
//             __m512 c[unroll];
//             for(int x=0; x<unroll; x++) {
//                 c[x] = _mm512_load_ps(&C[i+(x*16)+(j*n)]);   
//             }

//             for(int k=sk; k<sk+block_size; k++) {
//                 f32 arr[16];
//                 for(i32 i=0; i<16;i++)
//                     arr[i] = B[k+j*n];

//                 __m512 b = _mm512_load_ps(arr);

//                 for(int x=0; x<unroll; x++) 
//                     c[x] = _mm512_add_ps(c[x], _mm512_mul_ps(_mm512_load_ps(&A[n*k+x*16+i]), b));
//             }

//             for(int x=0; x<unroll; x++) {
//                 _mm512_store_ps(&C[i+x*16+j*n], c[x]);
//             }
//         }
//     }
// }

// inline void mul(f64* a, f64* b, f64* c, i32 n) {
//     __m512d sv = _mm512_set1_pd();
// }
f32x4 f32x4_dot(f32x4 a, f32x4 b) {
    f32x4 v;
    v.data = _mm_dp_ps(a.data, b.data, 0xff);
    return v;
}
#endif