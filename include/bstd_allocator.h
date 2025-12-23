#pragma once

#ifdef __BSTD_ALLOCATOR_IMPLEMENTATION__
#define __BSTD_ERROR_IMPLEMENTATION__
#endif

#include "bstd_error.h"

typedef enum {
  AE_NOT_ENOUGH_SPACE,
} ALLOCATION_ERR;

bstd_error_union_def(ALLOCATION_ERR, void *) allocation_opt;

typedef struct _bstd_alloc_interface {
  allocation_opt (*alloc)(void *, unsigned);
  void (*dealloc)(void *, void *);
  void (*free_all)(void *);
  void *backing_allocator;
} bstd_alloc_interface;

typedef struct bstd_fixed_arena_allocator {
  char *arr;
  unsigned long long size;
  unsigned long long current;
  bstd_alloc_interface *previous_allocator;
} bstd_arena_allocator;

#define bstd_alloc(allocator_interface_ptr, size)                              \
  (allocator_interface_ptr->alloc(allocator_interface_ptr, size))
#define bstd_dealloc(allocator_interface_ptr)                                  \
  (allocator_interface_ptr->dealloc(allocator_interface_ptr))
#define bstd_free_all(allocator_interface_ptr)                                 \
  (allocator_interface_ptr->free_all(allocator_interface_ptr))

extern const bstd_alloc_interface *bstd_global_alloc;

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_ALLOCATOR_IMPLEMENTATION__
#endif

#ifdef __BSTD_ALLOCATOR_IMPLEMENTATION__

#ifndef __BSTD_EMBEDED__
#include <stdlib.h>

// bstd_global_alloc
allocation_opt _bstd_global_allocate(void *, unsigned size) {
  void *mem = malloc(size);
  if (!mem)
    return bstd_err(allocation_opt, AE_NOT_ENOUGH_SPACE);
  return bstd_val(allocation_opt, mem);
}
void _bstd_global_dealloc(void *, void *data) { free(data); }
#else
extern const bstd_alloc_interface *bstdbstd_global_alloc = 0;
#endif

#endif
