#include <u/u.h>

test() {
  /* #[[tree<int, int>]] */
  u_tree_t(int, int) m = nullptr;

  /* new */
  m = u_tree_new(int, int, fn_cmp(int));
  mut_n(m);

  mut_e(0, u_tree_len(m));
  mut_t(u_tree_is_empty(m));

  mut_f(u_tree_is_exist(m, 0));
  mut_f(u_tree_is_exist(m, 1));
  mut_f(u_tree_is_exist(m, 2));

  u_tree_put(m, 1, -1);
  u_tree_put(m, 2, -2);
  u_tree_put(m, 3, -3);

  mut_e(3, u_tree_len(m));
  mut_f(u_tree_is_empty(m));

  mut_t(u_tree_is_exist(m, 1));
  mut_t(u_tree_is_exist(m, 2));
  mut_t(u_tree_is_exist(m, 3));

  mut_f(u_tree_is_exist(m, 4));
  mut_f(u_tree_is_exist(m, 5));
  mut_f(u_tree_is_exist(m, -1));
  mut_f(u_tree_is_exist(m, -2));

  u_tree_at(m, 1, -101);
  u_tree_at(m, 2, -102);
  u_tree_at(m, 3, -103);

  mut_e(3, u_tree_len(m));
  mut_f(u_tree_is_empty(m));

  mut_t(u_tree_is_exist(m, 1));
  mut_t(u_tree_is_exist(m, 2));
  mut_t(u_tree_is_exist(m, 3));

  mut_f(u_tree_is_exist(m, 4));
  mut_f(u_tree_is_exist(m, 5));
  mut_f(u_tree_is_exist(m, -1));
  mut_f(u_tree_is_exist(m, -2));

  mut_e(-101, u_tree_at(m, 1));
  mut_e(-102, u_tree_at(m, 2));
  mut_e(-103, u_tree_at(m, 3));

  u_tree_try(m, 1) {
    mut_e(-101, *it);
    *it = *it * 2;
  }

  mut_e(3, u_tree_len(m));
  mut_t(u_tree_is_exist(m, 1));
  mut_e(-202, u_tree_at(m, 1));

  u_tree_clear(m);

  mut_e(0, u_tree_len(m));
  mut_t(u_tree_is_empty(m));

  u_tree_cleanup(m);
  mut_nn(m);

  /* init */
  u_tree_init(m);
  mut_n(m);

  mut_e(0, u_tree_len(m));
  mut_t(u_tree_is_empty(m));

  mut_f(u_tree_is_exist(m, 0));
  mut_f(u_tree_is_exist(m, 1));
  mut_f(u_tree_is_exist(m, 2));

  u_tree_put(m, 1, -1);
  u_tree_put(m, 2, -2);
  u_tree_put(m, 3, -3);

  mut_e(3, u_tree_len(m));
  mut_f(u_tree_is_empty(m));

  mut_t(u_tree_is_exist(m, 1));
  mut_t(u_tree_is_exist(m, 2));
  mut_t(u_tree_is_exist(m, 3));

  mut_f(u_tree_is_exist(m, 4));
  mut_f(u_tree_is_exist(m, 5));
  mut_f(u_tree_is_exist(m, -1));
  mut_f(u_tree_is_exist(m, -2));

  u_tree_clear(m);

  mut_e(0, u_tree_len(m));
  mut_t(u_tree_is_empty(m));

  u_tree_cleanup(m);

  mut_nn(m);
}
