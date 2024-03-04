#include <u/u.h>

void avl(size_t N) {
  uavl(size_t, int) a = nullptr;

  /* put */
  benchmark(.n = N, .msg = "ut_put()") {
    ut_put(a, T.i, T.i * 2);
  }

  /* len */
  benchmark(.n = N, .msg = "ut_len()") {
    ut_len(a);
  }

  /* at */
  benchmark(.n = N, .msg = "ut_at()") {
    ut_at(a, T.i);
  }

  /* re */
  benchmark(.n = N, .msg = "ut_at.re()") {
    ut_at(a, T.i, T.i * 3);
  }

  /* pop */
  benchmark(.n = N, .msg = "ut_pop()") {
    ut_pop(a, T.i);
  }

  ut_cleanup(a);
}
