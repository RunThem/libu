#include <u/u.h>

test() {
  /* #[[vec<int>]] */
  u_vec_t(int) v = nullptr;

  /* 1 */
  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, -1, i);
  }

  int _i = 0;
  u_vec_for (v, i, it) {
    mut_e(_i, it);
    mut_e(i, _i++);
  }

  mut_e(N1W, u_vec_len(v));

  u_vec_cleanup(v);

  /* 2 */
  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, -1, i);
  }

  _i = N1W - 1;
  u_vec_for (v, i, it, U_ORDER_DESCEND) {
    mut_e(_i, it);
    mut_e(i, _i--);
  }

  mut_e(N1W, u_vec_len(v));

  u_vec_cleanup(v);
}
