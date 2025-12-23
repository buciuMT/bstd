#define __BSTD_IMPLEMENT_ALL__
#include "bstd_error.h"

bstd_error_union_def(int, float) test_err;

test_err returns_error() { return bstd_err(test_err, 0); }
test_err returns_value() { return bstd_val(test_err, 0); }

test_err bstd_should_error() {
  bstd_try(returns_error());
  bstd_unreachable("try failed to trigger");
}

int main() { bstd_should_error(); }
