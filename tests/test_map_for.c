#include <u/u.h>

test() {
  /* #[[map<int, int>]] */
  u_map_t(int, int) m = nullptr;

  m = u_map_new(int, int);

  u_each (i, N1W) {
    u_map_put(m, i, -i);
  }

  u_map_for (m, k, v) {
    mut_e(k, -v);
  }

  mut_e(N1W, u_map_len(m));

  u_map_cleanup(m);
}
