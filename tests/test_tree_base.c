#include <u/u.h>

test() {
  /* #[[tree<int, int>]] */
  u_tree_t(int, int) t = nullptr;

  /* new */
  t = u_tree_new(int, int, fn_cmp(int));
  mut_n(t);

  mut_e(0, u_tree_len(t));
  mut_t(u_tree_is_empty(t));

  mut_f(u_tree_is_exist(t, 0));
  mut_f(u_tree_is_exist(t, 1));
  mut_f(u_tree_is_exist(t, 2));

  u_tree_put(t, 1, -1);
  u_tree_put(t, 2, -2);
  u_tree_put(t, 3, -3);

  mut_e(3, u_tree_len(t));
  mut_f(u_tree_is_empty(t));

  mut_e(1, u_tree_min(t).key);
  mut_e(-1, u_tree_min(t).val);

  mut_e(3, u_tree_max(t).key);
  mut_e(-3, u_tree_max(t).val);

  mut_t(u_tree_is_exist(t, 1));
  mut_t(u_tree_is_exist(t, 2));
  mut_t(u_tree_is_exist(t, 3));

  mut_f(u_tree_is_exist(t, 4));
  mut_f(u_tree_is_exist(t, 5));
  mut_f(u_tree_is_exist(t, -1));
  mut_f(u_tree_is_exist(t, -2));

  u_tree_at(t, 1, -101);
  u_tree_at(t, 2, -102);
  u_tree_at(t, 3, -103);

  mut_e(3, u_tree_len(t));
  mut_f(u_tree_is_empty(t));

  mut_t(u_tree_is_exist(t, 1));
  mut_t(u_tree_is_exist(t, 2));
  mut_t(u_tree_is_exist(t, 3));

  mut_f(u_tree_is_exist(t, 4));
  mut_f(u_tree_is_exist(t, 5));
  mut_f(u_tree_is_exist(t, -1));
  mut_f(u_tree_is_exist(t, -2));

  mut_e(-101, u_tree_at(t, 1));
  mut_e(-102, u_tree_at(t, 2));
  mut_e(-103, u_tree_at(t, 3));

  u_tree_try(t, 1) {
    mut_e(-101, *it);
    *it = *it * 2;
  }

  u_tree_try(t, 1'2841'2735) {
    mut_e(-101, *it);
    *it = *it * 2;
  }

  mut_e(3, u_tree_len(t));
  mut_t(u_tree_is_exist(t, 1));
  mut_e(-202, u_tree_at(t, 1));

  u_tree_clear(t);

  mut_e(0, u_tree_len(t));
  mut_t(u_tree_is_empty(t));

  u_tree_cleanup(t);
  mut_nn(t);

  /* init */
  u_tree_init(t, fn_cmp(int));
  mut_n(t);

  mut_e(0, u_tree_len(t));
  mut_t(u_tree_is_empty(t));

  mut_f(u_tree_is_exist(t, 0));
  mut_f(u_tree_is_exist(t, 1));
  mut_f(u_tree_is_exist(t, 2));

  u_tree_put(t, 1, -1);
  u_tree_put(t, 2, -2);
  u_tree_put(t, 3, -3);

  mut_e(3, u_tree_len(t));
  mut_f(u_tree_is_empty(t));

  mut_t(u_tree_is_exist(t, 1));
  mut_t(u_tree_is_exist(t, 2));
  mut_t(u_tree_is_exist(t, 3));

  mut_f(u_tree_is_exist(t, 4));
  mut_f(u_tree_is_exist(t, 5));
  mut_f(u_tree_is_exist(t, -1));
  mut_f(u_tree_is_exist(t, -2));

  u_tree_clear(t);

  mut_e(0, u_tree_len(t));
  mut_t(u_tree_is_empty(t));

  u_tree_cleanup(t);

  mut_nn(t);
}
