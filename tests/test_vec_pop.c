#include <u/u.h>

test() {
  /* #[[vec<int>]] */
  u_vec_t(int) v = nullptr;

  /* 1 */
  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, -1, i);
  }

  mut_e(N1W, u_vec_len(v));

  u_each (i, N1W) {
    mut_e(i, u_vec_pop(v, 0));
  }

  mut_e(0, u_vec_pop(v, 0));
  mut_e(0, u_vec_pop(v, N1W));

  mut_e(0, u_vec_len(v));

  u_vec_cleanup(v);

  /* 2 */
  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, i);
  }

  mut_e(N1W, u_vec_len(v));

  u_each (i, N1W) {
    mut_e(i, u_vec_pop(v, 0));
  }

  mut_e(0, u_vec_pop(v, N1W));
  mut_e(0, u_vec_pop(v, N1W));

  mut_e(0, u_vec_len(v));

  u_vec_cleanup(v);

  /* 3 */
  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, 0, i);
  }

  mut_e(N1W, u_vec_len(v));

  u_each (i, N1W) {
    mut_e(N1W - i - 1, u_vec_pop(v, 0));
  }

  mut_e(0, u_vec_pop(v, N1W));
  mut_e(0, u_vec_pop(v, -1));

  mut_e(0, u_vec_len(v));

  u_vec_cleanup(v);

  /* 4 */
  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, -(i + 1), i);
  }

  mut_e(N1W, u_vec_len(v));

  u_each (i, N1W) {
    mut_e(N1W - i - 1, u_vec_pop(v, 0));
  }

  mut_e(0, u_vec_pop(v, N1W));
  mut_e(0, u_vec_pop(v, -1));

  mut_e(0, u_vec_len(v));

  u_vec_cleanup(v);

  /* 5 */
  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, i);
  }

  u_each (i, N1W) {
    u_vec_put(v, N1W - i - 1);
  }

  u_each (i, N1W) {
    u_vec_put(v, N1W + i, N1W);
  }

  u_each (i, N1W) {
    u_vec_put(v, 0, 0);
  }

  u_each (i, N1W) {
    u_vec_put(v, -1, 0);
  }

  mut_e(5 * N1W, u_vec_len(v));

  u_each (i, N1W) {
    mut_e(0, u_vec_pop(v, 0));
  }

  u_each (i, N1W) {
    mut_e(i, u_vec_pop(v, 0));
  }

  u_each (i, N1W) {
    mut_e(N1W, u_vec_pop(v, 0));
  }

  u_each (i, N1W) {
    mut_e(N1W - i - 1, u_vec_pop(v, 0));
  }

  u_each (i, N1W) {
    mut_e(0, u_vec_pop(v, 0));
  }

  mut_e(0, u_vec_pop(v, 5 * N1W));
  mut_e(0, u_vec_pop(v, 0));

  mut_e(0, u_vec_len(v));

  u_vec_cleanup(v);
}
