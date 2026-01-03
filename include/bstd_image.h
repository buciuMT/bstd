#pragma once

#include "bstd_math.h"
#include "bstd_memory.h"
#include "bstd_stream.h"
#include "bstd_utils.h"

ALWAYS_INLINE void bstd_convert_shift_u8_i8(const u8 *in, i8 *out, usize cnt,
                                            isize by);
ALWAYS_INLINE void bstd_convert_u8_RGB_YCbCr(const u8 *in, u8 *out,
                                             usize padding, usize count);
ALWAYS_INLINE void bstd_convert_u8_YCbCr_RGB(const u8 *in, u8 *out,
                                             usize padding, usize count);

ALWAYS_INLINE void bstd_separate_channels(const u8 *src, usize size, u8 *r,
                                          u8 *g, u8 *b);

bstd_stream_errs bstd_dump_ppm(bstd_sink *sink, const u8 *buffer, usize height,
                               usize width, bool rgb);

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

    // https://www.w3.org/Graphics/JPEG/jfif3.pdf
    u8 y = BSTD_CLAMP(BSTD_ROUND(16 + (0.299 * r) + (0.587 * g) + (0.114 * b)),
                      0, 255);
    u8 cb = BSTD_CLAMP(
        BSTD_ROUND(128 + (-0.168736 * r) - (0.331264 * g) + (0.5 * b)), 0, 255);
    u8 cr = BSTD_CLAMP(
        BSTD_ROUND(128 + (0.5 * r) - (0.418688 * g) - (0.081312 * b)), 0, 255);

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

    // JPEG standard conversion formulas
    u8 r = BSTD_CLAMP(BSTD_ROUND((y - 16) + 1.402 * (cr - 128)), 0, 255);
    u8 g = BSTD_CLAMP(
        BSTD_ROUND((y - 16) - 0.34414 * (cb - 128) - 0.71414 * (cr - 128)), 0,
        255);
    u8 b = BSTD_CLAMP(BSTD_ROUND((y - 16) + 1.772 * (cb - 128)), 0, 255);

    out[pos] = r;
    out[pos + 1] = g;
    out[pos + 2] = b;
  }
}

ALWAYS_INLINE void bstd_separate_channels(const u8 *src, usize size, u8 *r,
                                          u8 *g, u8 *b) {
  bstd_memsep(src, size * 3, 1, 3, r, g, b);
}

bstd_stream_errs bstd_dump_ppm(bstd_sink *sink, const u8 *buffer, usize height,
                               usize width, bool rgb) {
  bstd_stream_errs err = bstd_sink_write(sink, rgb ? "P6\n" : "P2\n", 1, 3);
  if (err)
    return err;
  const int b_size = 16;
  int size = b_size - 1;
  char cb[b_size];
  cb[b_size - 1] = ' ';

  usize tmp = height;
  while (tmp && size)
    cb[--size] = '0' + tmp % 10, tmp /= 10;

  if (bstd_sink_write(sink, cb + size, 1, b_size - size))
    return err;

  cb[b_size - 1] = '\n';
  size = b_size - 1;

  tmp = width;
  while (tmp && size)
    cb[--size] = '0' + tmp % 10, tmp /= 10;

  if (bstd_sink_write(sink, cb + size, 1, b_size - size))
    return err;

  err = bstd_sink_write(sink, buffer, rgb ? 3 : 1, height * width);
  if (err)
    return err;
  return BS_S_OK;
}
#endif
