#ifndef SLEIPNIR_TYPES_H
#define SLEIPNIR_TYPES_H

namespace sleipnir {

// 32 bits unsigned int 16 bits alligned
typedef unsigned u32 __attribute__ ((aligned (16)));

// 32 bits int 16 bits alligned
typedef int i32 __attribute__ ((aligned (16)));

// 32 bits float 16 bits alligned
typedef float f32 __attribute__ ((aligned (16)));

// 64 bits float 32 bits alligned
typedef double f64 __attribute__ ((aligned (32)));

}
#endif