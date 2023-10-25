#include <u/core/vec.h>

void vec(void) {
  u_vec_t(int) v = u_vec_new(int);

  /* push */
  benchmark(.n = N10W, .msg = "push_back()") {
    u_vec_push_back(v, T.i);
  };

  benchmark(.n = N10W, .msg = "push_front()") {
    u_vec_push_front(v, T.i);
  };

  benchmark(.n = N10W, .msg = "push()") {
    u_vec_push(v, T.i, T.i);
  }

  /* len */
  benchmark(.n = N10W, .msg = "len") {
    u_vec_len(v);
  }

  /* at */
  benchmark(.n = N10W, .msg = "at_back()") {
    (void)u_vec_at_back(v);
  }

  benchmark(.n = N10W, .msg = "at_front()") {
    (void)u_vec_at_front(v);
  }

  benchmark(.n = N10W, .msg = "at()") {
    (void)u_vec_at(v, T.i);
  }

  /* re */
  benchmark(.n = N10W, .msg = "re()") {
    (void)u_vec_re_back(v, T.i * 2);
  }

  benchmark(.n = N10W, .msg = "at_front()") {
    (void)u_vec_re_front(v, T.i * 2);
  }

  benchmark(.n = N10W, .msg = "at()") {
    (void)u_vec_re(v, T.i, T.i * 2);
  }

  /* pop */
  benchmark(.n = N10W, .msg = "pop_back()") {
    (void)u_vec_pop_back(v);
  }

  benchmark(.n = N10W, .msg = "pop_front()") {
    (void)u_vec_pop_front(v);
  }

  benchmark(.n = N10W, .msg = "pop()") {
    (void)u_vec_pop(v, T.i);
  }

  u_vec_cleanup(v);
}
