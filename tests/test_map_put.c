#include <u/u.h>

test() {
  /* #[[map<int, int>]] */
  u_map_t(int, int) m = nullptr;

  m = u_map_new(int, int);

  u_each (i, N1W) {
    u_map_put(m, i, -i);
  }

  mut_e(N1W, u_map_len(m));

  u_each (i, N1W) {
    mut_e(-i, u_map_at(m, i));
  }

  mut_e(0, u_map_at(m, -1));
  mut_e(0, u_map_at(m, N1W));

  u_map_cleanup(m);
}
