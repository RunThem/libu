#include <u/u.h>

void avl(size_t N) {
  uavl(size_t, int) a = nullptr;

  /* put */
  benchmark(.n = N, .msg = "ua_put()") {
    ua_put(a, T.i, T.i * 2);
  }

  /* len */
  benchmark(.n = N, .msg = "ua_len()") {
    ua_len(a);
  }

  /* at */
  benchmark(.n = N, .msg = "ua_at()") {
    ua_at(a, T.i);
  }

  /* re */
  benchmark(.n = N, .msg = "ua_at.re()") {
    ua_at(a, T.i, T.i * 3);
  }

  /* pop */
  benchmark(.n = N, .msg = "ua_pop()") {
    ua_pop(a, T.i);
  }

  ua_cleanup(a);
}
