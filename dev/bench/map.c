#include <u/u.h>

void map(size_t N) {
  umap(size_t, int) t = nullptr;

  um_init(t);

  /* put */
  benchmark(.n = N, .msg = "um_put()") {
    um_put(t, T.i, T.i * 2);
  }

  /* len */
  benchmark(.n = N, .msg = "um_len()") {
    um_len(t);
  }

  /* at */
  benchmark(.n = N, .msg = "um_at()") {
    um_at(t, T.i);
  }

  /* at.re */
  benchmark(.n = N, .msg = "um_at.re()") {
    um_at(t, T.i, T.i * 3);
  }

  /* pop */
  benchmark(.n = N, .msg = "um_pop()") {
    um_pop(t, T.i);
  }

  um_cleanup(t);
}
