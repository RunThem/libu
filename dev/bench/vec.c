#include <u/u.h>

void vec(size_t N) {
  uvec(int) v = nullptr;

  uv_init(v);

  /* put */
  benchmark(.n = N, .msg = "uv_put(-1)") {
    uv_put(v, -1, T.i);
  };

  benchmark(.n = N, .msg = "uv_put(0)") {
    uv_put(v, 0, T.i);
  };

  benchmark(.n = N, .msg = "uv_put(...)") {
    uv_put(v, T.i, T.i);
  }

  /* len */
  benchmark(.n = N, .msg = "uv_len()") {
    uv_len(v);
  }

  /* cap */
  benchmark(.n = N, .msg = "uv_cap()") {
    uv_cap(v);
  }

  /* at */
  benchmark(.n = N, .msg = "uv_at(-1)") {
    (void)uv_at(v, -1);
  }

  benchmark(.n = N, .msg = "uv_at(0)") {
    (void)uv_at(v, 0);
  }

  benchmark(.n = N, .msg = "uv_at(...)") {
    (void)uv_at(v, T.i);
  }

  /* at.re */
  benchmark(.n = N, .msg = "uv_at.re(-1)") {
    (void)uv_at(v, -1, T.i * 2);
  }

  benchmark(.n = N, .msg = "uv_at.re(0)") {
    (void)uv_at(v, 0, T.i * 2);
  }

  benchmark(.n = N, .msg = "uv_at.re(...)") {
    (void)uv_at(v, T.i, T.i * 2);
  }

  /* pop */
  benchmark(.n = N, .msg = "uv_pop(-1)") {
    (void)uv_pop(v, -1);
  }

  benchmark(.n = N, .msg = "uv_pop(0)") {
    (void)uv_pop(v, 0);
  }

  benchmark(.n = N, .msg = "uv_pop(...)") {
    (void)uv_pop(v, T.i);
  }

  uv_cleanup(v);
}
