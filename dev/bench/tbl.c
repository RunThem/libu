#include <u/u.h>

void tbl(size_t N) {
  utbl(size_t, int) t = nullptr;

  ut_init(t);

  /* put */
  benchmark(.n = N, .msg = "ut_put()") {
    ut_put(t, T.i, T.i * 2);
  }

  /* len */
  benchmark(.n = N, .msg = "ut_len()") {
    ut_len(t);
  }

  /* at */
  benchmark(.n = N, .msg = "ut_at()") {
    ut_at(t, T.i);
  }

  /* at.re */
  benchmark(.n = N, .msg = "ut_at.re()") {
    ut_at(t, T.i, T.i * 3);
  }

  /* pop */
  benchmark(.n = N, .msg = "ut_pop()") {
    ut_pop(t, T.i);
  }

  ut_cleanup(t);
}
