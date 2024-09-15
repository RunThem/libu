#include <u/u.h>

test() {
  /* #[[tree<int, int>]] */
  u_tree_t(int, int) t = nullptr;

  t = u_tree_new(int, int, fn_cmp(int));

  u_each (i, N1W) {
    u_tree_put(t, i, -i);
  }

  u_tree_for (t, k, v) {
    mut_e(k, -v);
  }

  u_tree_for (t, k, v, U_ORDER_DESCEND) {
    mut_e(k, -v);
  }

  mut_e(N1W, u_tree_len(t));

  u_tree_cleanup(t);
}
