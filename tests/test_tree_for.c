#include <u/u.h>

test() {
  /* #[[tree<int, int>]] */
  u_tree_t(int, int) m = nullptr;

  m = u_tree_new(int, int, fn_cmp(int));

  u_each (i, N1W) {
    u_tree_put(m, i, -i);
  }

  u_tree_for (m, k, v) {
    mut_e(k, -v);
  }

  u_tree_rfor (m, k, v) {
    mut_e(k, -v);
  }

  mut_e(N1W, u_tree_len(m));

  u_tree_cleanup(m);
}
