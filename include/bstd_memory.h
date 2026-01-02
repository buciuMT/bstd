#pragma once

#include "bstd_utils.h"
#include <stdarg.h>

ALWAYS_INLINE void bstd_memcpy(void *dest, const void *src, usize size);
ALWAYS_INLINE void bstd_memsep(const void *source, usize size, usize objsize,
                               usize count, ...);

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_MEMORY__
#endif

#ifdef __BSTD_MEMORY__

ALWAYS_INLINE void bstd_memcpy(void *dest, const void *src, usize size) {
  const u8 *_src = src;
  u8 *_dest = dest;
  for (usize i = 0; i < size; i++)
    _dest[i] = _src[i];
}
ALWAYS_INLINE void bstd_memsep(const void *src, usize objsize, usize size,
                               usize count, ...) {
  if (objsize == 0)
    return;
  va_list args;
  void *arrs[count];
  va_start(args, count);
  for (usize i = 0; i < count; i++)
    arrs[i] = va_arg(args, void *);
  for (usize i = 0; i < size; i++) {
    usize stride = i % count;
    if (!arrs[stride])
      continue;
    bstd_memcpy(arrs[stride], src + (i * objsize), objsize);
    arrs[stride] += objsize;
  }
}
#endif
