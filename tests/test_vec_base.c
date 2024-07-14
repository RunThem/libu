#include <u/u.h>

test() {
  /* #[[vec<int>]] */
  u_vec_t(int) v = nullptr;

  /* new */
  v = u_vec_new(int);
  mut_n(v);

  mut_e(0, u_vec_len(v));
  mut_t(u_vec_is_empty(v));

  mut_f(u_vec_is_exist(v, 0));
  mut_f(u_vec_is_exist(v, 1));

  u_vec_put(v, 1);
  u_vec_put(v, 2);

  mut_e(2, u_vec_len(v));
  mut_f(u_vec_is_empty(v));

  mut_t(u_vec_is_exist(v, 0));
  mut_t(u_vec_is_exist(v, 1));
  mut_f(u_vec_is_exist(v, 3));
  mut_f(u_vec_is_exist(v, 4));

  u_vec_at(v, 0, -1);
  u_vec_at(v, 1, -2);

  mut_e(2, u_vec_len(v));
  mut_f(u_vec_is_empty(v));

  mut_t(u_vec_is_exist(v, 0));
  mut_t(u_vec_is_exist(v, 1));
  mut_f(u_vec_is_exist(v, 3));
  mut_f(u_vec_is_exist(v, 4));

  mut_e(-1, u_vec_at(v, 0));
  mut_e(-2, u_vec_at(v, 1));

  u_vec_try(v, 0) {
    *it = 100;
  }

  mut_e(2, u_vec_len(v));
  mut_t(u_vec_is_exist(v, 0));
  mut_e(100, u_vec_at(v, 0));

  u_vec_clear(v);

  mut_e(0, u_vec_len(v));
  mut_t(u_vec_is_empty(v));

  u_vec_cleanup(v);
  mut_nn(v);

  /* init */
  u_vec_init(v);
  mut_n(v);

  mut_e(0, u_vec_len(v));
  mut_t(u_vec_is_empty(v));

  mut_f(u_vec_is_exist(v, 0));
  mut_f(u_vec_is_exist(v, 1));

  u_vec_put(v, 1);
  u_vec_put(v, 2);

  mut_e(2, u_vec_len(v));
  mut_f(u_vec_is_empty(v));

  mut_t(u_vec_is_exist(v, 0));
  mut_t(u_vec_is_exist(v, 1));
  mut_f(u_vec_is_exist(v, 3));
  mut_f(u_vec_is_exist(v, 4));

  u_vec_clear(v);

  mut_e(0, u_vec_len(v));
  mut_t(u_vec_is_empty(v));

  u_vec_cleanup(v);
  mut_nn(v);
}
