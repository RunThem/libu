#include <u/u.h>

test() {
  /* #[[map<int, int>]] */
  u_map_t(int, int) m = nullptr;

  /* 1 */
  m = u_map_new(int, int);

  u_each (i, N1W) {
    u_map_put(m, i, -i);
  }

  mut_e(N1W, u_map_len(m));

  u_each (i, N1W) {
    mut_e(-i, u_map_pop(m, i));
  }

  mut_e(0, u_map_pop(m, 0));
  mut_e(0, u_map_pop(m, 1));
  mut_e(0, u_map_pop(m, 2));
  mut_e(0, u_map_pop(m, N1W));
  mut_e(0, u_map_pop(m, N1W - 1));

  mut_e(0, u_map_len(m));

  u_map_cleanup(m);
}
