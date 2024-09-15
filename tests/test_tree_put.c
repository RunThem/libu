#include <u/u.h>

test() {
  /* #[[tree<int, int>]] */
  u_tree_t(int, int) t = nullptr;

  t = u_tree_new(int, int, fn_cmp(int));

  u_each (i, N1W) {
    u_tree_put(t, i, -i);
  }

  mut_e(N1W, u_tree_len(t));

  u_each (i, N1W) {
    mut_e(-i, u_tree_at(t, i));
  }

  mut_e(0, u_tree_at(t, -1));
  mut_e(0, u_tree_at(t, N1W));

  u_tree_cleanup(t);
}
