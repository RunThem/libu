#include <u/u.h>

#undef N1W
#define N1W 10

test() {
  /* #[[vec<int>]] */
  u_vec_t(int) v = u_vec_new(int);

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
    mut_e(0, u_vec_at(v, i));
  }

  u_each (i, N1W) {
    mut_e(i, u_vec_at(v, i + N1W));
  }

  u_each (i, N1W) {
    mut_e(N1W, u_vec_at(v, i + 2 * N1W));
  }

  u_each (i, N1W) {
    mut_e(N1W - i - 1, u_vec_at(v, i + 3 * N1W));
  }

  u_each (i, N1W) {
    mut_e(0, u_vec_at(v, i + 4 * N1W));
  }

  mut_e(0, u_vec_at(v, 5 * N1W));

  u_vec_cleanup(v);
}
