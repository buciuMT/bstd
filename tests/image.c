
#include <assert.h>
#include <stdio.h>
#include <string.h>
#define __BSTD_IMPLEMENT_ALL__

#include "bstd_core.h"
#include "bstd_image.h"
#include "bstd_stream.h"
#include "bstd_utils.h"

string header = SLIT("P6 64 64 255\n\n\n");

char scratch[128];

int main() {
  FILE *f = fopen("extra/img_triangle.ppm", "r");
  usize width, heigth;
  fscanf(f, " %s", scratch);
  fscanf(f, " %ld", &width);
  fscanf(f, " %ld", &heigth);
  const usize total = width * heigth;
  u8 img_rgb[3 * total];
  u8 img_yCbCr[3 * total];

  for (usize i = 0, tmp; i < 3 * total; i++)
    fscanf(f, " %ld", &tmp), img_rgb[i] = tmp;

  bstd_convert_u8_RGB_YCbCr(img_rgb, img_yCbCr, 0, total);

  bstd_sink sink =
      bstd_expect(bstd_sink_file_open("tmp/ycbr.ppm", false, true, 0, 0),
                  "unable to open file");

  if (bstd_sink_write(&sink, header.str, 1, header.size))
    bstd_panic("unable to write");

  if (bstd_sink_write(&sink, img_yCbCr, 3, total))
    bstd_panic("unable to write");

  if (bstd_sink_flush(&sink))
    bstd_panic("unable to write");

  bstd_sink_close(&sink);

  // bstd_convert_shift_u8_i8(img_yCbCr, (i8 *)img_rgb, 3 * total, -128);
  bstd_convert_u8_YCbCr_RGB(img_yCbCr, img_rgb, 0, total);

  sink = bstd_expect(bstd_sink_file_open("tmp/rgb.ppm", false, true, 0, 0),
                     "unable to open file");

  if (bstd_sink_write(&sink, header.str, 1, header.size))
    bstd_panic("unable to write");

  if (bstd_sink_write(&sink, img_rgb, 3, total))
    bstd_panic("unable to write");

  if (bstd_sink_flush(&sink))
    bstd_panic("unable to write");

  bstd_sink_close(&sink);
}
