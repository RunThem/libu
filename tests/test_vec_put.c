#include <u/u.h>

test() {
  /* #[[vec<int>]] */
  u_vec_t(int) v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, i);
  }

  mut_e(N1W, u_vec_len(v));

  u_each (i, N1W) {
    mut_e(i, u_vec_at(v, i));
  }

  mut_e(0, u_vec_at(v, N1W));

  u_vec_cleanup(v);

  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, -1, i);
  }

  mut_e(N1W, u_vec_len(v));

  u_each (i, N1W) {
    mut_e(i, u_vec_at(v, i));
  }

  mut_e(0, u_vec_at(v, N1W));

  u_vec_cleanup(v);
}
