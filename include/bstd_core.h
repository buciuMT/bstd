#pragma once

#include "bstd_utils.h"
#include <stdarg.h>

int bstd_err_printf(const char *fmt, ...);
int bstd_printf(const char *fmt, ...);
ALWAYS_INLINE int bstd_err_vprintf(const char *fmt, va_list va);
ALWAYS_INLINE int bstd_vprintf(const char *fmt, va_list va);
int bstd_io_flush();
NO_RETURN int bstd_exit(int code);
NO_RETURN int bstd_panic(const char *msg, ...);

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_CORE_IMPLEMENTATION__
#endif

#ifdef __BSTD_CORE_IMPLEMENTATION__
NO_RETURN int bstd_panic(const char *msg, ...) {
  va_list args;
  va_start(args, msg);
  bstd_err_vprintf(msg, args);
  va_end(args);
  bstd_err_printf("\n");
  bstd_exit(-1);
}

int bstd_err_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int res = bstd_err_vprintf(fmt, args);
  va_end(args);
  return res;
}

int bstd_printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  int res = bstd_vprintf(fmt, args);
  va_end(args);
  return res;
}

#ifndef __BSTD_EMBEDED__
#include <stdio.h>
#include <stdlib.h>

NO_RETURN int bstd_exit(int code) { exit(code); }
int bstd_vprintf(const char *fmt, va_list va) {
  return vfprintf(stdout, fmt, va);
}
int bstd_err_vprintf(const char *fmt, va_list va) {
  return vfprintf(stderr, fmt, va);
}
#else

#endif
#endif
