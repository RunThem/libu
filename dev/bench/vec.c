#include <u/core/vec.h>

void vec(size_t N) {
  u_vec_t(int) v = u_vec_new(int);

  /* push */
  benchmark(.n = N, .msg = "vec_push_back()") {
    u_vec_push_back(v, T.i);
  };

  benchmark(.n = N, .msg = "vec_push_front()") {
    u_vec_push_front(v, T.i);
  };

  benchmark(.n = N, .msg = "vec_push()") {
    u_vec_push(v, T.i, T.i);
  }

  /* len */
  benchmark(.n = N, .msg = "vec_len()") {
    u_vec_len(v);
  }

  /* at */
  benchmark(.n = N, .msg = "vec_at_back()") {
    (void)u_vec_at_back(v);
  }

  benchmark(.n = N, .msg = "vec_at_front()") {
    (void)u_vec_at_front(v);
  }

  benchmark(.n = N, .msg = "vec_at()") {
    (void)u_vec_at(v, T.i);
  }

  /* re */
  benchmark(.n = N, .msg = "vec_re_back()") {
    (void)u_vec_re_back(v, T.i * 2);
  }

  benchmark(.n = N, .msg = "vec_re_front()") {
    (void)u_vec_re_front(v, T.i * 2);
  }

  benchmark(.n = N, .msg = "vec_re()") {
    (void)u_vec_re(v, T.i, T.i * 2);
  }

  /* pop */
  benchmark(.n = N, .msg = "vec_pop_back()") {
    (void)u_vec_pop_back(v);
  }

  benchmark(.n = N, .msg = "vec_pop_front()") {
    (void)u_vec_pop_front(v);
  }

  benchmark(.n = N, .msg = "vec_pop()") {
    (void)u_vec_pop(v, T.i);
  }

  u_vec_cleanup(v);
}
