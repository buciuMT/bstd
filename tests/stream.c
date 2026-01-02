#include "bstd_utils.h"
#define __BSTD_IMPLEMENT_ALL__
#include "bstd_core.h"
#include "bstd_error.h"
#include "bstd_stream.h"

const string str = SLIT("Hello guys");
#define BUFFSIZ 512

int main() {
  u8 buff[BUFFSIZ];
  bstd_sink sink = bstd_expect(
      bstd_sink_file_open("tmp/sink.txt", false, true, buff, BUFFSIZ),
      "something went wrong opening");
  if (bstd_sink_write(&sink, str.str, 1, str.size))
    bstd_panic("should not even write");
  for (usize i = 0; i < BUFFSIZ / str.size; i++)
    if (bstd_sink_write(&sink, str.str, 1, str.size))
      bstd_panic("error on write");
  bstd_sink_flush(&sink);
  bstd_sink_close(&sink);
}
