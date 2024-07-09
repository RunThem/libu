#include <u/u.h>

test() {
  /* #[[vec<int>]] */
  u_vec_t(int) v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, i);
  }

  u_each (i, N1W) {
    mut_e(N1W - i - 1, u_vec_pop(v));
  }

  mut_e(0, u_vec_len(v));

  u_vec_cleanup(v);

  v = u_vec_new(int);

  u_each (i, N1W) {
    u_vec_put(v, i);
  }

  u_each (i, N1W) {
    mut_e(i, u_vec_pop(v, 0));
  }

  mut_e(0, u_vec_len(v));

  u_vec_cleanup(v);
}
