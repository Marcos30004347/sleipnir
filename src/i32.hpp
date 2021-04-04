#ifndef SLEIPNIR_I32_H
#define SLEIPNIR_I32_H

#include<immintrin.h>
#include"types.hpp"
namespace sleipnir {

union i32x4 {
    __m128i data;
    i32 vec[4];
};


i32x4 i32x4_new(i32 a, i32 b, i32 c, i32 d) {
    i32x4 v;
    v.data = _mm_set_epi32(a,b,c,d);
    return v;
}

i32x4 i32x4_add(i32x4 a, i32x4 b) {
    i32x4 v;
    v.data = _mm_add_epi32(a.data, b.data);
    return v;
}

i32x4 i32x4_sub(i32x4 a, i32x4 b) {
    i32x4 v;
    v.data = _mm_sub_epi32(a.data, b.data);
    return v;
}

i32x4 i32x4_mul(i32x4 a, i32x4 b) {
    i32x4 v;
    v.data = _mm_mul_epi32(a.data, b.data);
    return v;
}

i32x4 i32x4_min(i32x4 a, i32x4 b) {
    i32x4 v;
    v.data = _mm_min_epi32(a.data, b.data);
    return v;
}

i32x4 i32x4_max(i32x4 a, i32x4 b) {
    i32x4 v;
    v.data = _mm_max_epi32(a.data, b.data);
    return v;
}

i32 i32x4_sum(i32x4 a) {
    i32x4 v;
    v.data =_mm_hadd_epi32(a.data,a.data);
    v.data =_mm_hadd_epi32(v.data,v.data);
    return _mm_extract_epi32(v.data, 0);
}

}
#endif