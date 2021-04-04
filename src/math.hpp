#ifndef SLEIPNIR_MATH_H
#define SLEIPNIR_MATH_H

#include "types.hpp"
namespace sleipnir {

i32 max(i32 a, i32 b) {
  i32 diff = a - b;
  i32 dsgn = diff >> 31;
  return a - (diff & dsgn);
}

i32 min(i32 a, i32 b) {
  i32 diff = a - b;
  i32 dsgn = diff >> 31;
  return b + (diff & dsgn);
}

}

#endif