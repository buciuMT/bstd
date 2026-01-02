#pragma once

#include "bstd_utils.h"

#define BSTD_CLAMP(expr, min, max)                                             \
  ({                                                                           \
    typeof(expr) v = (expr), mx = (max), mn = (min);                           \
    v < mn ? mn : (v > mx ? mx : v);                                           \
  })

#define BSTD_ABS(expr)                                                         \
  ({                                                                           \
    typeof(expr) v = (expr);                                                   \
    v < 0 ? -v : v;                                                            \
  });

#define BSTD_ROUND(expr)                                                       \
  ({                                                                           \
    typeof(expr) v = (expr);                                                   \
    (typeof(expr))(isize)(v + 0.5);                                            \
  })

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_MATH__
#endif

#ifdef __BSTD_MATH__

#endif
