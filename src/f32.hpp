#ifndef SLEIPNIR_F32_H
#define SLEIPNIR_F32_H

#include<immintrin.h>

#include"types.hpp"
namespace sleipnir {

union f32x4 {
    __m128 data;
    f32 vec[4];
};

void f32x4_cp_values(f32x4 v, f32* a) {
    return _mm_store_ps(a, v.data);
}

f32x4 f32x4_new(i32 a) {
    f32x4 v;
    v.data = _mm_set1_ps(a);
    return v;
}

f32x4 f32x4_new(f32 a, f32 b, f32 c, f32 d) {
    f32x4 v;
    v.data = _mm_set_ps(a,b,c,d);
    return v;
}


f32x4 f32x4_add(f32x4 a, f32x4 b) {
    f32x4 v;
    v.data = _mm_add_ps(a.data, b.data);
    return v;
}

f32x4 f32x4_sub(f32x4 a, f32x4 b) {
    f32x4 v;
    v.data = _mm_sub_ps(a.data, b.data);
    return v;
}

f32x4 f32x4_mul(f32x4 a, f32x4 b) {
    f32x4 v;
    v.data = _mm_mul_ps(a.data, b.data);
    return v;
}

f32x4 f32x4_div(f32x4 a, f32x4 b) {
    f32x4 v;
    v.data = _mm_div_ps(a.data, b.data);
    return v;
}

f32x4 f32x4_sqr(f32x4 a) {
    f32x4 v;
    v.data = _mm_sqrt_ps(a.data);
    return v;
}

f32x4 f32x4_min(f32x4 a, f32x4 b) {
    f32x4 v;
    v.data = _mm_min_ps(a.data, b.data);
    return v;
}

f32x4 f32x4_rcp(f32x4 a) {
    f32x4 v;
    v.data = _mm_rcp_ps(a.data);
    return v;
}

void f32x4_transpose(f32x4* a, f32x4* b, f32x4* c, f32x4* d) {
    __m128 tmp3, tmp2, tmp1, tmp0;
    tmp0 = _mm_unpacklo_ps(a->data, b->data);
    tmp2 = _mm_unpacklo_ps(c->data, d->data);
    tmp1 = _mm_unpackhi_ps(a->data, b->data);
    tmp3 = _mm_unpackhi_ps(c->data, d->data);
    a->data = _mm_movelh_ps(tmp0, tmp2);
    b->data = _mm_movehl_ps(tmp2, tmp0);
    c->data = _mm_movelh_ps(tmp1, tmp3);
    d->data = _mm_movehl_ps(tmp3, tmp1);
}

f32 f32x4_sum(f32x4 a) {
    f32x4 v;
    v.data =_mm_hadd_ps(a.data,a.data);
    v.data =_mm_hadd_ps(v.data,v.data);
    return _mm_extract_ps(v.data, 0);
}

f32 f32x4_dot(f32x4 a, f32x4 b) {
    // v.data = _mm_dp_ps(a.data, b.data, 0xff);
    f32x4 v = f32x4_mul(a,b);
    return f32x4_sum(v);
}
}
#endif