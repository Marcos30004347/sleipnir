#ifndef I32_H
#define I32_H

#include<immintrin.h>
#include"types.hpp"

union i32x4 {
    __m128i data;
    i32 vec[4];
};

i32x4 i32x4_new(i32 a, i32 b, i32 c, i32 d);
i32x4 i32x4_add(i32x4 a, i32x4 b);
i32x4 i32x4_sub(i32x4 a, i32x4 b);
i32x4 i32x4_mul(i32x4 a, i32x4 b);
i32x4 i32x4_max(i32x4 a, i32x4 b);
i32x4 i32x4_min(i32x4 a, i32x4 b);


#endif