#pragma once

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_ALLOCATOR_IMPLEMENTATION__
#endif

#include "bstd_utils.h"

#include "bstd_error.h"

#ifndef BSTD_ALIGMENT
#define BSTD_ALIGMENT 4
#endif

#define bstd_alloc_with_aligment(size)                                         \
  (((BSTD_ALIGMENT - 1) + (size)) & ~(BSTD_ALIGMENT - 1))

typedef enum {
  AE_NOT_ENOUGH_SPACE,
} ALLOCATION_ERR;

bstd_error_union_def(ALLOCATION_ERR, void *) allocation_opt;

typedef struct _bstd_alloc_interface {
  allocation_opt (*const alloc)(void *, usize);
  void (*const dealloc)(void *, void *);
  void (*const free_all)(void *);
  void *backing_allocator;
} bstd_alloc_interface;

#define bstd_alloc(allocator_interface_ptr, size)                              \
  ((allocator_interface_ptr)                                                   \
       ->alloc((allocator_interface_ptr)->backing_allocator, size))
#define bstd_dealloc(allocator_interface_ptr)                                  \
  ((allocator_interface_ptr)                                                   \
       ->dealloc((allocator_interface_ptr)->backing_allocator))
#define bstd_free_all(allocator_interface_ptr)                                 \
  ((allocator_interface_ptr)                                                   \
       ->free_all((allocator_interface_ptr)->backing_allocator))

#define _bstd_alloc_interface_new_def(function_name, alloc_function,           \
                                      dealloc_function, free_function)         \
  bstd_alloc_interface function_name(void *backing) {                          \
    return (bstd_alloc_interface){.backing_allocator = backing,                \
                                  .alloc = alloc_function,                     \
                                  .dealloc = dealloc_function,                 \
                                  .free_all = free_function};                  \
  }

typedef struct {
  usize size;
  usize current;
  u8 *arr;
} bstd_fixed_arena_allocator;

allocation_opt bstd_fixed_arena_allocator_alloc(void *nonnull alloc,
                                                usize size);
void bstd_fixed_arena_allocator_dealloc(void *nonnull alloc, void *ptr);
void bstd_fixed_arena_allocator_free_all(void *nonnull alloc);
bstd_alloc_interface
bstd_interface_from_fixed_arena_allocator(void *nonnull alloc);

extern const bstd_alloc_interface *bstd_global_alloc;

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_ALLOCATOR_IMPLEMENTATION__
#endif

#ifdef __BSTD_ALLOCATOR_IMPLEMENTATION__

allocation_opt bstd_fixed_arena_allocator_alloc(void *nonnull alloc,
                                                usize size) {
  bstd_fixed_arena_allocator *a = (bstd_fixed_arena_allocator *)alloc;
  const usize s = bstd_alloc_with_aligment(size);
  if (a->current + s >= a->size)
    return bstd_err(allocation_opt, AE_NOT_ENOUGH_SPACE);
  void *ret = a->arr + a->current;
  a->current += s;
  return bstd_val(allocation_opt, ret);
}
void bstd_fixed_arena_allocator_dealloc(void *nonnull alloc, void *ptr) {}
void bstd_fixed_arena_allocator_free_all(void *nonnull alloc) {

  bstd_fixed_arena_allocator *a = (bstd_fixed_arena_allocator *)alloc;
  a->current = 0;
}
_bstd_alloc_interface_new_def(bstd_interface_from_fixed_arena_allocator,
                              bstd_fixed_arena_allocator_alloc,
                              bstd_fixed_arena_allocator_dealloc,
                              bstd_fixed_arena_allocator_free_all);

#ifndef __BSTD_EMBEDED__
#include <stdlib.h>

allocation_opt _bstd_global_allocate(void *_, unsigned size) {
  void *mem = malloc(size);
  if (!mem)
    return bstd_err(allocation_opt, AE_NOT_ENOUGH_SPACE);
  return bstd_val(allocation_opt, mem);
}
void _bstd_global_dealloc(void *_, void *data) { free(data); }
#else
extern const bstd_alloc_interface *bstdbstd_global_alloc = 0;
#endif

#endif
