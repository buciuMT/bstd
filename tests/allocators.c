#include <stdio.h>

#define __BSTD_IMPLEMENT_ALL__

#include "bstd_allocator.h"
#include "bstd_core.h"
#include "bstd_utils.h"

int main() {
  u8 buffer[512];
  bstd_fixed_arena_allocator arena = {.size = 512, .arr = buffer};
  bstd_alloc_interface interf =
      bstd_interface_from_fixed_arena_allocator(&arena);

  void *ptr = bstd_expect(bstd_alloc(&interf, BSTD_ALIGMENT * 2 - 2),
                          "FAILED to allocate at `" __FILE__ "`");
  if ((usize)ptr != (usize)buffer)
    bstd_panic("Wrong offset");
  if (arena.current != BSTD_ALIGMENT * 2)
    bstd_panic("Wrong aligment calculation: expected %d, found %ld",
               BSTD_ALIGMENT, arena.current);
  bstd_free_all(&interf);
  if (arena.current != 0)
    bstd_panic("Not able to reset");
  return 0;
}
