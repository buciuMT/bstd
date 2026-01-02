#pragma once

#include "bstd_math.h"
#include "bstd_utils.h"

ALWAYS_INLINE void bstd_convert_shift_u8_i8(const u8 *in, i8 *out, usize cnt,
                                            isize by);
ALWAYS_INLINE void bstd_convert_u8_RGB_YCbCr(const u8 *in, u8 *out,
                                             usize padding, usize count);
ALWAYS_INLINE void bstd_convert_u8_YCbCr_RGB(const u8 *in, u8 *out,
                                             usize padding, usize count);

#ifdef __BSTD_IMPLEMENT_ALL__

#define __BSTD_IMAGE__
#endif

#ifdef __BSTD_IMAGE__

void bstd_convert_shift_u8_i8(const u8 *in, i8 *out, usize cnt, isize by) {
  for (usize i = 0; i < cnt; i++) {
    isize tmp = in[i];
    tmp += by;
    out[i] = tmp;
  }
}

ALWAYS_INLINE void bstd_convert_u8_RGB_YCbCr(const u8 *in, u8 *out,
                                             usize padding, usize count) {
  for (usize cnt = 0; cnt < count; cnt++) {
    const usize pos = (padding + 3) * cnt;
    isize r = in[pos];
    isize g = in[pos + 1];
    isize b = in[pos + 2];

    // https://en.wikipedia.org/wiki/YCbCr

    u8 y = BSTD_ROUND(16 + (0.299 * r) + (0.587 * g) + (0.114 * b));
    u8 cb = BSTD_ROUND(128 + (-0.168736 * r) - (0.331274 * g) + (0.5 * b));
    u8 cr = BSTD_ROUND(128 + (0.5 * r) - (0.4583 * g) - (0.0417 * b));

    out[pos] = y;
    out[pos + 1] = cb;
    out[pos + 2] = cr;
  }
}

ALWAYS_INLINE void bstd_convert_u8_YCbCr_RGB(const u8 *in, u8 *out,
                                             usize padding, usize count) {
  for (usize cnt = 0; cnt < count; cnt++) {
    const usize pos = (padding + 3) * cnt;
    isize y = in[pos];
    isize cb = in[pos + 1];
    isize cr = in[pos + 2];

    // https://en.wikipedia.org/wiki/YCbCr

    u8 r = BSTD_CLAMP(BSTD_ROUND((y - 16) + 1.402 * (cr - 128)), 0, 255);
    u8 g = BSTD_CLAMP(
        BSTD_ROUND((y - 16) - 0.344136 * (cb - 128) - 0.714136 * (cr - 128)), 0,
        255);
    u8 b = BSTD_CLAMP(BSTD_ROUND((y - 16) + 1.772 * (cb - 128)), 0, 255);

    out[pos] = r;
    out[pos + 1] = g;
    out[pos + 2] = b;
  }
}
#endif
