#include "f32.hpp"

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

void f32x4_cp_values(f32x4 v, f32* a) {
    return _mm_store_ps(a, v.data);
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

// f32x4 f32x4_cmp_eq(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmpeq_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_neq(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmpneq_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_lt(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmplt_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_nlt(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmpnlt_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_le(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmple_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_nle(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmpnle_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_gt(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmpgt_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_ngt(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmpngt_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_ge(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmpge_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_cmp_nge(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_cmpnge_ps(a.data, b.data);
//     return v;
// }

// f32x4 f32x4_blend(f32x4 a, f32x4 b, f32 w) {
//     f32x4 v;
//     v.data = _mm_blend_ps(a.data, b.data, w);
//     return v;
// }


// f32x4 f32x4_dot(f32x4 a, f32x4 b) {
//     f32x4 v;
//     v.data = _mm_dp_ps(a.data, b.data, 0xff);
//     return v;
// }

f32 f32x4_sum(f32x4 a) {
    f32x4 v;
    v.data =_mm_hadd_ps(a.data,a.data);
    v.data =_mm_hadd_ps(v.data,v.data);
    return _mm_extract_ps(v.data, 0);
}



// f32x4 f32x4_as_mat2_mul(f32x4 vec1, f32x4 vec2) {
//     f32x4 v;
// 	v.data = _mm_add_ps(_mm_mul_ps(vec1.data, f32x4_swizzle(vec2, 0,3,0,3).data), _mm_mul_ps(f32x4_swizzle(vec1, 1,0,3,2).data, f32x4_swizzle(vec2, 2,1,2,1).data));
//     return v;
// }

// // 2x2 row major Matrix adjugate multiply (A#)*B
// f32x4 f32x4_as_mat2_adj_mul(f32x4 vec1, f32x4 vec2) {
//     f32x4 v;

// 	v.data =_mm_sub_ps(_mm_mul_ps(f32x4_swizzle(vec1, 3,3,0,0).data, vec2.data), _mm_mul_ps(f32x4_swizzle(vec1, 1,1,2,2).data, f32x4_swizzle(vec2, 2,3,0,1).data));
//     return v;

// }
// // 2x2 row major Matrix multiply adjugate A*(B#)
// f32x4 f32x4_as_mat2_mul_adj(f32x4 vec1, f32x4 vec2) {
//     f32x4 v;
// 	v.data = _mm_sub_ps(_mm_mul_ps(vec1.data, f32x4_swizzle(vec2, 3,0,3,0).data), _mm_mul_ps(f32x4_swizzle(vec1, 1,0,3,2).data, f32x4_swizzle(vec2, 2,1,2,1).data));
// 	return v;
// }

// f32x4 f32x4_add_adj(f32x4 vec) {
//     f32x4 v;
// 	v.data = _mm_hadd_ps(vec.data, vec.data);
// 	return v;

// }

// void f32_srq_mat4dimx4dim_mul(f32* dest[], f32* src1[], f32* src2[], int dim) {
//     for(int i=0;i<dim;i+=1){
//         for(int j=0;j<dim;j+=4){
//             for(int k=0;k<dim;k+=4){
//                 __m128 result = _mm_load_ps(&dest[i][j]);
//                 __m128 a_line  = _mm_load_ps(&src1[i][k]);
//                 __m128 b_line0 = _mm_load_ps(&src2[k][j+0]);
//                 __m128 b_line1 = _mm_loadu_ps(&src2[k][j+1]);
//                 __m128 b_line2 = _mm_loadu_ps(&src2[k][j+2]);
//                 __m128 b_line3 = _mm_loadu_ps(&src2[k][j+3]);

//                 result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a_line, a_line, 0x00), b_line0));
//                 result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a_line, a_line, 0x55), b_line1));
//                 result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a_line, a_line, 0xaa), b_line2));
//                 result = _mm_add_ps(result, _mm_mul_ps(_mm_shuffle_ps(a_line, a_line, 0xff), b_line3));
//                 _mm_store_ps(&dest[i][j],result);
//             }
//         }
//     }
// }

// void f32_mat_mul(f32* dst, f32* src1, f32* src2, int src1_h, int src1_w, int src2_h, int src2_w) {
//     for (int x = 0; x < src1_h; x += 4) {
//         for (int y = 0; y < src2_w; y += 4) {
//             __m128 des0 = _mm_setzero_ps ();
//             __m128 des1 = _mm_setzero_ps ();
//             __m128 des2 = _mm_setzero_ps ();
//             __m128 des3 = _mm_setzero_ps ();

//             for (int k = 0; k < src2_w; k += 4) {
//                 __m128 I0 = _mm_load_ps((src1 + (x + 0) * src1_w + k));
//                 __m128 I1 = _mm_load_ps((src1 + (x + 1) * src1_w + k));
//                 __m128 I2 = _mm_load_ps((src1 + (x + 2) * src1_w + k));
//                 __m128 I3 = _mm_load_ps((src1 + (x + 3) * src1_w + k));

//                 __m128 I4 = _mm_set_ps (src2[(k+3) * src2_w + y], src2[(k+2) * src2_w + y],
//                                             src2[(k+1) * src2_w + y], src2[k * src2_w + y]);
//                 __m128 I5 = _mm_set_ps (src2[(k+3) * src2_w + (y+1)],
//                                             src2[(k+2) * src2_w + (y+1)], src2[(k+1) * src2_w + (y+1)],
//                                             src2[(k+0) * src2_w + (y+1)]);
//                 __m128 I6 = _mm_set_ps (src2[(k+3) * src2_w + (y+2)],
//                                             src2[(k+2) * src2_w + (y+2)], src2[(k+1) * src2_w + (y+2)],
//                                             src2[(k+0) * src2_w + (y+2)]);
//                 __m128 I7 = _mm_set_ps (src2[(k+3) * src2_w + (y+3)],
//                                             src2[(k+2) * src2_w + (y+3)], src2[(k+1) * src2_w + (y+3)],
//                                             src2[(k+0) * src2_w + (y+3)]);

//                 __m128 T0 = _mm_mul_ps(I0, I4);
//                 __m128 T1 = _mm_mul_ps(I0, I5);
//                 __m128 T2 = _mm_mul_ps(I0, I6);
//                 __m128 T3 = _mm_mul_ps(I0, I7);

//                 __m128 T4 = _mm_mul_ps(I1, I4);
//                 __m128 T5 = _mm_mul_ps(I1, I5);
//                 __m128 T6 = _mm_mul_ps(I1, I6);
//                 __m128 T7 = _mm_mul_ps(I1, I7);

//                 __m128 T8 = _mm_mul_ps(I2, I4);
//                 __m128 T9 = _mm_mul_ps(I2, I5);
//                 __m128 T10 = _mm_mul_ps(I2, I6);
//                 __m128 T11 = _mm_mul_ps(I2, I7);

//                 __m128 T12 = _mm_mul_ps(I3, I4);
//                 __m128 T13 = _mm_mul_ps(I3, I5);
//                 __m128 T14 = _mm_mul_ps(I3, I6);
//                 __m128 T15 = _mm_mul_ps(I3, I7);

//                 __m128 T16 = _mm_unpacklo_ps(T0, T1);
//                 __m128 T17 = _mm_unpacklo_ps(T2, T3);
//                 __m128 T18 = _mm_unpackhi_ps(T0, T1);
//                 __m128 T19 = _mm_unpackhi_ps(T2, T3);
//                 __m128d T20 = _mm_unpacklo_pd(_mm_castps_pd(T16), _mm_castps_pd(T17));
//                 __m128d T21 = _mm_unpackhi_pd(_mm_castps_pd(T16), _mm_castps_pd(T17));
//                 __m128d T22 = _mm_unpacklo_pd(_mm_castps_pd(T18), _mm_castps_pd(T19));
//                 __m128d T23 = _mm_unpackhi_pd(_mm_castps_pd(T18), _mm_castps_pd(T19));

//                 T20 = _mm_add_pd(T20, T21);
//                 T20 = _mm_add_pd(T20, T22);
//                 T20 = _mm_add_pd(T20, T23);

//                 des0 = _mm_add_ps(_mm_castpd_ps(T20), des0);

//                 T16 = _mm_unpacklo_ps(T4, T5);
//                 T17 = _mm_unpacklo_ps(T6, T7);
//                 T18 = _mm_unpackhi_ps(T4, T5);
//                 T19 = _mm_unpackhi_ps(T6, T7);

//                 T20 = _mm_unpacklo_pd(_mm_castps_pd(T16), _mm_castps_pd(T17));
//                 T21 = _mm_unpackhi_pd(_mm_castps_pd(T16), _mm_castps_pd(T17));
//                 T22 = _mm_unpacklo_pd(_mm_castps_pd(T18), _mm_castps_pd(T19));
//                 T23 = _mm_unpackhi_pd(_mm_castps_pd(T18), _mm_castps_pd(T19));

//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T21));
//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T22));
//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T23));

//                 des1 = _mm_add_ps(_mm_castpd_ps(T20), des1);

//                 T16 = _mm_unpacklo_ps(T8, T9);
//                 T17 = _mm_unpacklo_ps(T10, T11);
//                 T18 = _mm_unpackhi_ps(T8, T9);
//                 T19 = _mm_unpackhi_ps(T10, T11);

//                 T20 = _mm_unpacklo_pd(_mm_castps_pd(T16), _mm_castps_pd(T17));
//                 T21 = _mm_unpackhi_pd(_mm_castps_pd(T16), _mm_castps_pd(T17));
//                 T22 = _mm_unpacklo_pd(_mm_castps_pd(T18), _mm_castps_pd(T19));
//                 T23 = _mm_unpackhi_pd(_mm_castps_pd(T18), _mm_castps_pd(T19));

//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T21));
//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T22));
//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T23));

//                 des2 = _mm_add_ps(_mm_castpd_ps(T20), des2);

//                 T16 = _mm_unpacklo_ps(T12, T13);
//                 T17 = _mm_unpacklo_ps(T14, T15);
//                 T18 = _mm_unpackhi_ps(T12, T13);
//                 T19 = _mm_unpackhi_ps(T14, T15);

//                 T20 = _mm_unpacklo_pd(_mm_castps_pd(T16), _mm_castps_pd(T17));
//                 T21 = _mm_unpackhi_pd(_mm_castps_pd(T16), _mm_castps_pd(T17));
//                 T22 = _mm_unpacklo_pd(_mm_castps_pd(T18), _mm_castps_pd(T19));
//                 T23 = _mm_unpackhi_pd(_mm_castps_pd(T18), _mm_castps_pd(T19));

//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T21));
//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T22));
//                 T20 = _mm_add_ps(_mm_castpd_ps(T20), _mm_castpd_ps(T23));

//                 des3 = _mm_add_ps(_mm_castpd_ps(T20), des3);
//             }

//             _mm_store_ps((dst + ((x + 0) * src2_w) + y), des0);
//             _mm_store_ps((dst + ((x + 1) * src2_w) + y), des1);
//             _mm_store_ps((dst + ((x + 2) * src2_w) + y), des2);
//             _mm_store_ps((dst + ((x + 3) * src2_w) + y), des3);
//         }
//     }
// }