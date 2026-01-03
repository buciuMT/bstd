#pragma once

#include "bstd_error.h"
#include "bstd_utils.h"
#include <stdbool.h>

int bstd_cmp_str_cstr(const string a, const char *b);

bstd_optional_def(bool) optional_bool;
optional_bool bstd_parse_bool(const string str, bool permisive);

#ifdef __BSTD_IMPLEMENT_ALL__
#define __BSTD_STRING_IMPLEMENTATION__
#endif

#ifdef __BSTD_STRING_IMPLEMENTATION__
optional_bool bstd_parse_bool(const string str, bool permisive) {
  if (!bstd_cmp_str_cstr(str, "true"))
    return bstd_val(optional_bool, true);
  if (!bstd_cmp_str_cstr(str, "false"))
    return bstd_val(optional_bool, false);
  if (permisive) {
    if (!bstd_cmp_str_cstr(str, "1"))
      return bstd_val(optional_bool, true);
    if (!bstd_cmp_str_cstr(str, "0"))
      return bstd_val(optional_bool, false);
  }
  return bstd_none(optional_bool);
}

int bstd_cmp_str_cstr(const string a, const char *b) {
  for (usize i = 0; i < a.size; i++) {
    if (!b[i])
      return a.str[i];
    if (a.str[i] != b[i])
      return a.str[i] - b[i];
  }
  return 0;
}
#endif
