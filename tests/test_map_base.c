#include <u/u.h>

test() {
  /* #[[map<int, int>]] */
  u_map_t(int, int) m = nullptr;

  /* new */
  m = u_map_new(int, int);
  mut_n(m);

  mut_e(0, u_map_len(m));
  mut_t(u_map_is_empty(m));

  mut_f(u_map_is_exist(m, 0));
  mut_f(u_map_is_exist(m, 1));
  mut_f(u_map_is_exist(m, 2));

  u_map_put(m, 1, -1);
  u_map_put(m, 2, -2);
  u_map_put(m, 3, -3);

  mut_e(3, u_map_len(m));
  mut_f(u_map_is_empty(m));

  mut_t(u_map_is_exist(m, 1));
  mut_t(u_map_is_exist(m, 2));
  mut_t(u_map_is_exist(m, 3));

  mut_f(u_map_is_exist(m, 4));
  mut_f(u_map_is_exist(m, 5));
  mut_f(u_map_is_exist(m, -1));
  mut_f(u_map_is_exist(m, -2));

  u_map_at(m, 1, -101);
  u_map_at(m, 2, -102);
  u_map_at(m, 3, -103);

  mut_e(3, u_map_len(m));
  mut_f(u_map_is_empty(m));

  mut_t(u_map_is_exist(m, 1));
  mut_t(u_map_is_exist(m, 2));
  mut_t(u_map_is_exist(m, 3));

  mut_f(u_map_is_exist(m, 4));
  mut_f(u_map_is_exist(m, 5));
  mut_f(u_map_is_exist(m, -1));
  mut_f(u_map_is_exist(m, -2));

  mut_e(-101, u_map_at(m, 1));
  mut_e(-102, u_map_at(m, 2));
  mut_e(-103, u_map_at(m, 3));

  u_map_try(m, 1) {
    mut_e(-101, *it);
    *it = *it * 2;
  }

  mut_e(3, u_map_len(m));
  mut_t(u_map_is_exist(m, 1));
  mut_e(-202, u_map_at(m, 1));

  u_map_clear(m);

  mut_e(0, u_map_len(m));
  mut_t(u_map_is_empty(m));

  u_map_cleanup(m);
  mut_nn(m);

  /* init */
  u_map_init(m);
  mut_n(m);

  mut_e(0, u_map_len(m));
  mut_t(u_map_is_empty(m));

  mut_f(u_map_is_exist(m, 0));
  mut_f(u_map_is_exist(m, 1));
  mut_f(u_map_is_exist(m, 2));

  u_map_put(m, 1, -1);
  u_map_put(m, 2, -2);
  u_map_put(m, 3, -3);

  mut_e(3, u_map_len(m));
  mut_f(u_map_is_empty(m));

  mut_t(u_map_is_exist(m, 1));
  mut_t(u_map_is_exist(m, 2));
  mut_t(u_map_is_exist(m, 3));

  mut_f(u_map_is_exist(m, 4));
  mut_f(u_map_is_exist(m, 5));
  mut_f(u_map_is_exist(m, -1));
  mut_f(u_map_is_exist(m, -2));

  u_map_clear(m);

  mut_e(0, u_map_len(m));
  mut_t(u_map_is_empty(m));

  u_map_cleanup(m);
  mut_nn(m);
}
