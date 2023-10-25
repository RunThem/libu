#include <u/core/avl.h>

void avl(size_t N) {
  u_avl_t(int, int) t = u_avl_new(int, int, fn_cmp_use(int));

  /* push */
  benchmark(.n = N, .msg = "avl_push()") {
    u_avl_push(t, T.i, T.i * 2);
  }

  /* len */
  benchmark(.n = N, .msg = "avl_len()") {
    u_avl_len(t);
  }

  /* at */
  benchmark(.n = N, .msg = "avl_at()") {
    u_avl_at(t, T.i);
  }

  /* re */
  benchmark(.n = N, .msg = "avl_re()") {
    u_avl_re(t, T.i, T.i * 3);
  }

  /* pop */
  benchmark(.n = N, .msg = "avl_pop()") {
    u_avl_pop(t, T.i);
  }

  u_avl_cleanup(t);
}
