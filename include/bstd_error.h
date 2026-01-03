#pragma once
#include "bstd_core.h"
#include "bstd_utils.h"

#define bstd_unreachable(...)                                                  \
  {                                                                            \
    bstd_err_printf("Unreachable point reached at `%s:%s`", __FILE__,          \
                    __LINE__);                                                 \
    bstd_err_printf(__VA_ARGS__);                                              \
    bstd_exit(1);                                                              \
  }

#define bstd_none(type)                                                        \
  (type) { .is_err = true }

#define bstd_optional_def(value_type)                                          \
  typedef struct {                                                             \
    union {                                                                    \
      value_type val;                                                          \
      u8 err;                                                                  \
    };                                                                         \
    bool is_err;                                                               \
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
  (type_name) { .err = (err_value), .is_err = true }

#define bstd_val(type_name, value)                                             \
  (type_name) { .val = (value) }

#define bstd_try(expr)                                                         \
  ({                                                                           \
    __typeof__(expr) tmp = (expr);                                             \
    if (tmp.is_err)                                                            \
      return tmp;                                                              \
    tmp.val;                                                                   \
  })

#define bstd_or_else(expr, alt_val)                                            \
  ({                                                                           \
    __typeof__(expr) _tmp = (expr);                                            \
    __typeof__(_tmp.err) err = _tmp.err;                                       \
    (void)err;                                                                 \
    _tmp.is_err ? (alt_val) : _tmp.val;                                        \
  })

#define bstd_or_ret(expr, ret_expr)                                            \
  ({                                                                           \
    __typeof__(expr) _tmp = (expr);                                            \
    __typeof__(_tmp.err) err = _tmp.err;                                       \
    (void)err;                                                                 \
    _tmp.is_err ? ({                                                           \
      return (ret_expr);                                                       \
      _tmp.val;                                                                \
    })                                                                         \
                : _tmp.val;                                                    \
  })

#define bstd_expect(expr, err_message)                                         \
  ({                                                                           \
    __typeof__(expr) _tmp = (expr);                                            \
    if (_tmp.is_err)                                                           \
      bstd_panic("%s", err_message);                                           \
    _tmp.val;                                                                  \
  })
