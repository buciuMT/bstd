#include <assert.h>
#include <stdio.h>
#define __BSTD_IMPLEMENT_ALL__

#include "bstd_core.h"
#include "bstd_image.h"
#include "bstd_math.h"
#include "bstd_stream.h"
#include "bstd_utils.h"

int test_rgb_ycbcr_roundtrip() {

  u8 rgb_input[9] = {255, 128, 64,   // Red=255, Green=128, Blue=64
                     0,   255, 0,    // Pure green
                     128, 128, 128}; // Gray
  u8 ycbcr_temp[9];
  u8 rgb_output[9];

  bstd_convert_u8_RGB_YCbCr(rgb_input, ycbcr_temp, 0, 3);

  bstd_convert_u8_YCbCr_RGB(ycbcr_temp, rgb_output, 0, 3);

  int success = 1, tolerance = 2;
  for (int i = 0; i < 9; i++) {
    int diff = BSTD_ABS((int)rgb_input[i] - (int)rgb_output[i]);

    if (diff > tolerance) {
      printf(
          "Mismatch at index %d: input=%d, output=%d, diff=%d (tolerance=%d)\n",
          i, rgb_input[i], rgb_output[i], diff, tolerance);
      success = 0;
    }
  }

  return success;
}

int test_specific_values() {

  u8 rgb[3] = {255, 0, 0};
  u8 ycbcr[3];
  u8 rgb_back[3];

  bstd_convert_u8_RGB_YCbCr(rgb, ycbcr, 0, 1);

  bstd_convert_u8_YCbCr_RGB(ycbcr, rgb_back, 0, 1);

  int success = 1, tolerance = 4;
  for (int i = 0; i < 3; i++) {
    int diff = BSTD_ABS((int)rgb[i] - (int)rgb_back[i]);

    if (diff > tolerance) {
      printf("Roundtrip mismatch at index %d: original=%d, converted back=%d, "
             "diff=%d (tolerance=%d)\n",
             i, rgb[i], rgb_back[i], diff, tolerance);
      success = 0;
    }
  }

  return success;
}

int main() {
  int result1 = test_rgb_ycbcr_roundtrip();
  int result2 = test_specific_values();

  if (result1 && result2) {
    return 0;
  } else {
    return 1;
  }
}
