#include <u/core/map.h>

void map(size_t N) {
  u_map_t(int, int) m = u_map_new(int, int, fn_eq(int), U_MAP_INT_HASH_FN);

  /* push */
  benchmark(.n = N, .msg = "map_push()") {
    u_map_push(m, T.i, T.i * 2);
  }

  /* len */
  benchmark(.n = N, .msg = "map_len()") {
    u_map_len(m);
  }

  /* at */
  benchmark(.n = N, .msg = "map_at()") {
    u_map_at(m, T.i);
  }

  /* re */
  benchmark(.n = N, .msg = "map_re()") {
    u_map_re(m, T.i, T.i * 3);
  }

  /* pop */
  benchmark(.n = N, .msg = "map_pop()") {
    u_map_pop(m, T.i);
  }

  u_map_cleanup(m);
}
