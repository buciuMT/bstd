#pragma once
#ifdef __BSTD_ERROR_IMPLEMENTATION__
#define __BSTD_CORE_IMPLEMENTATION__
#endif
#include "bstd_core.h"
#include "bstd_utils.h"

#define bstd_unreachable(...)                                                  \
  {                                                                            \
    bstd_err_printf("Unreachable point reached at `%s:%s`", __FILE__,          \
                    __LINE__);                                                 \
    bstd_err_printf(__VA_ARGS__);                                              \
    bstd_exit(1);                                                              \
  }

#define bstd_error_union_def(err_type, value_type)                             \
  typedef struct {                                                             \
    union {                                                                    \
      err_type err;                                                            \
      value_type val;                                                          \
    };                                                                         \
    bool is_err;                                                               \
  }

#define bstd_err(type_name, err_value)                                         \
  (type_name) { .err = err_value, .is_err = true }

#define bstd_val(type_name, value)                                             \
  (type_name) { .val = value }

#define bstd_try(expr)                                                         \
  ({                                                                           \
    __typeof__(expr) tmp = (expr);                                             \
    if (tmp.is_err)                                                            \
      return tmp;                                                              \
    tmp.val;                                                                   \
  })

#define bstd_or_else(expr, val)                                                \
  ({                                                                           \
    __typeof__(expr) _tmp = (expr);                                            \
    _tmp.is_err ? (val) : _tmp.val;                                            \
  })

#define bstd_expect(expr, err_message)                                         \
  ({                                                                           \
    __typeof__(expr) _tmp = (expr);                                            \
    if (expr.is_err) {                                                         \
    }                                                                          \
  })
