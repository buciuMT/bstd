#pragma once

#include "bstd_allocator.h"
#include "bstd_error.h"
#include "bstd_string.h"
#include "bstd_utils.h"
#include <stdbool.h>

#ifndef BSTD_MAX_ARGS
#define BSTD_MAX_ARGS 128
#endif

typedef enum {
  BS_ARG_NULL,
  BS_ARG_BOOL,
  BS_ARG_FLAG,
  BS_ARG_STRING,
} bstd_args_type;

typedef struct {
  bstd_args_type type;
  bool expected;
  char shortform;
  string longform;
  string description;
  void((*nullable callback)(const char *));
  void *nullable data;
} bstd_arg_element;

typedef enum {
  BS_E_AR_NO_MEMORY,
  BS_E_AR_INVALID_ARGUMENT,
  BS_E_AR_EXPECTED_BOOL,
  BS_E_AR_EXPECTED_STRING,
} arg_parse_err_type;

typedef struct {
  arg_parse_err_type type;
  int position;
} bstd_arg_parse_err;

bstd_error_union_def(bstd_arg_parse_err, int) bstd_arg_parse_ret;

bstd_arg_parse_ret bstd_arg_element_run(const bstd_arg_element *nonnull elm,
                                        int count, const char **arg) {
  switch (elm->type) {
  case BS_ARG_NULL:
    return bstd_val(bstd_arg_parse_ret, 0);
    break;
  case BS_ARG_BOOL:
    if (count < 1)
      return bstd_err(
          bstd_arg_parse_ret,
          ((bstd_arg_parse_err){.type = BS_E_AR_EXPECTED_BOOL, .position = 0}));
    *((bool *)elm->data) = bstd_or_ret(
        bstd_parse_bool(SLIT(arg[0]), true),
        bstd_err(bstd_arg_parse_ret,
                 ((bstd_arg_parse_err){.type = BS_E_AR_EXPECTED_BOOL,
                                       .position = 0})));
    return bstd_val(bstd_arg_parse_ret, 1);
    break;
  case BS_ARG_FLAG:
    *((bool *)elm->data) = true;
    return bstd_val(bstd_arg_parse_ret, 0);
    break;
  case BS_ARG_STRING:
    if (count < 1)
      return bstd_err(bstd_arg_parse_ret,
                      ((bstd_arg_parse_err){.type = BS_E_AR_EXPECTED_STRING,
                                            .position = 0}));
    *((string *)elm->data) = SLIT(arg[0]);
    return bstd_val(bstd_arg_parse_ret, 1);
    break;
  }
}

typedef struct {
  bstd_arg_element args[BSTD_MAX_ARGS];
  usize count;
} bstd_args;

typedef struct _bstd_args_unparsed {
  usize count;
  int *positions;
} bstd_args_unparsed;

bstd_error_union_def(bstd_arg_parse_err,
                     bstd_args_unparsed) bstd_args_parse_ret;

bstd_args_parse_ret bstd_args_parse(const bstd_args *sarg, int count,
                                    const char **arg,
                                    bstd_alloc_interface *nullable alloc) {
  int unparsed[max(count, BSTD_MAX_ARGS)];
  int unparsed_cnt = 0;
  for (int i = 0; i < count; i++) {
    if (!arg[i][0])
      continue;
    if (arg[i][0] != '-') {
      unparsed[unparsed_cnt++] = i;
      continue;
    }
    if (arg[i][1] == '-') {
      for (usize a = 0; a < sarg->count; a++)
        if (!bstd_cmp_str_cstr(sarg->args[a].longform, arg[i] + 2)) {
          int offset = bstd_or_ret(
              bstd_arg_element_run(sarg->args + a, count - i - 1, arg + i + 1),
              bstd_err(
                  bstd_args_parse_ret,
                  ((bstd_arg_parse_err){.type = err.type,
                                        .position = err.position + i + 1})));
          i += offset;
          goto next_argument;
        }
    } else {
      for (usize a = 0; a < sarg->count; a++)
        if (sarg->args[a].shortform && (sarg->args[a].shortform == arg[i][2])) {
          int offset = bstd_or_ret(
              bstd_arg_element_run(sarg->args + a, count - i - 1, arg + i + 1),
              bstd_err(
                  bstd_args_parse_ret,
                  ((bstd_arg_parse_err){.type = err.type,
                                        .position = err.position + i + 1})));
          i += offset;
          goto next_argument;
        }
    }
    unparsed[unparsed_cnt++] = i;
  next_argument:;
  }
  if (!alloc)
    return bstd_val(bstd_args_parse_ret, ((bstd_args_unparsed){.count = 0}));
  int *positions =
      bstd_or_ret(bstd_alloc(alloc, sizeof(unparsed[0]) * unparsed_cnt),
                  bstd_err(bstd_args_parse_ret, ((bstd_arg_parse_err){
                                                    .type = BS_E_AR_NO_MEMORY,
                                                })));
  for (int i = 0; i < unparsed_cnt; i++)
    positions[i] = unparsed[i];

  return bstd_val(
      bstd_args_parse_ret,
      ((bstd_args_unparsed){.count = unparsed_cnt, .positions = positions}));
}

typedef struct _bstd_a_menu {
  struct _bstd_a_menu *next;
  struct _bstd_a_menu *children;
  bstd_args *args;
} bstd_a_menu;

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_ARGS_IMPLEMENTATION__
#endif
