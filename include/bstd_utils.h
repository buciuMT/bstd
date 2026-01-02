#pragma once
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__) || defined(__clang__)
#define BSTD_GNUC_LIKE 1
#else
#define BSTD_GNUC_LIKE 0
#endif

#if defined(_MSC_VER)
#define BSTD_MSVC 1
#else
#define BSTD_MSVC 0
#endif

#if BSTD_GNUC_LIKE
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

#if BSTD_GNUC_LIKE

#define MAYBE_UNUSED __attribute__((unused))
#define ALWAYS_INLINE __attribute__((always_inline)) inline
#define NEVER_INLINE __attribute__((noinline))
#define HOT __attribute__((hot))
#define COLD __attribute__((cold))
#define NO_RETURN __attribute__((noreturn))
#define PURE __attribute__((pure))
#define CONST_FN __attribute__((const))
#define USED __attribute__((used))
#define CONSTRUCTOR __attribute__((constructor))
#define DESTRUCTOR __attribute__((destructor))
#define RETURNS_NONNULL __attribute__((returns_nonnull))
#define MALLOC __attribute__((malloc))
#define WARN_UNUSED_RESULT __attribute__((warn_unused_result))

#else // Plain compilers/unknown

#define MAYBE_UNUSED
#define ALWAYS_INLINE inline
#define NEVER_INLINE
#define HOT
#define COLD
#define NO_RETURN
#define PURE
#define CONST_FN
#define USED
#define CONSTRUCTOR
#define DESTRUCTOR
#define RETURNS_NONNULL
#define MALLOC
#define WARN_UNUSED_RESULT

#endif /* BSTD_GNUC_LIKE */

#if BSTD_MSVC
#if !defined(NO_RETURN)
#include <sal.h>
#define NO_RETURN __declspec(noreturn)
#endif
#endif

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Unsigned integers
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// Signed integers
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// Floating point
typedef float f32;
typedef double f64;

typedef size_t usize;
typedef ptrdiff_t isize;

typedef struct {
  char *str;
  usize size;
} string;

#define SLIT(s) ((string){.str = (s), .size = sizeof((s)) - 1})

#define max(a, b)                                                              \
  ({                                                                           \
    typeof(a) tmpa = (a);                                                      \
    typeof(b) tmpb = (b);                                                      \
    (tmpa < tmpb ? tmpb : tmpa);                                               \
  })

#define min(a, b)                                                              \
  ({                                                                           \
    typeof(a) tmpa = (a);                                                      \
    typeof(b) tmpb = (b);                                                      \
    (tmpa < tmpb ? tmpa : tmpb);                                               \
  })
