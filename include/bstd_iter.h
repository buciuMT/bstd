#pragma once

#define bstd_next(obj)                                                         \
  ({                                                                           \
    typeof(obj) tmp = obj;                                                     \
    tmp.next(&tmp);                                                            \
  })

#ifdef BSTD_SHORT

#define a(obj, fnc, ...) fnc((obj), __VA_ARGS__)

#endif
