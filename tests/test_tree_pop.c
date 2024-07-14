#include <u/u.h>

test() {
  /* #[[tree<int, int>]] */
  u_tree_t(int, int) m = nullptr;

  /* 1 */
  m = u_tree_new(int, int, fn_cmp(int));

  u_each (i, N1W) {
    u_tree_put(m, i, -i);
  }

  mut_e(N1W, u_tree_len(m));

  u_each (i, N1W) {
    mut_e(-i, u_tree_pop(m, i));
  }

  mut_e(0, u_tree_pop(m, 0));
  mut_e(0, u_tree_pop(m, 1));
  mut_e(0, u_tree_pop(m, 2));
  mut_e(0, u_tree_pop(m, N1W));
  mut_e(0, u_tree_pop(m, N1W - 1));

  mut_e(0, u_tree_len(m));

  u_tree_cleanup(m);
}
