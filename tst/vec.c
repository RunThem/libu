#include <mut.h>

#define u_vec_defs u_defs(vec, int)
#include <u/u.h>

#define N 10

mut_test(vec_create) {
  u_vec_t(int) v = nullptr;

  u_vec_init(v);

  mut_assert(v != nullptr);

  mut_assert(16 == u_vec_cap(v));
  mut_assert(0 == u_vec_len(v));

  u_vec_cleanup(v);

  mut_assert(v == nullptr);
}

mut_test(vec_interface) {
  /*
   * push back
   * */
  u_vec_t(int) v = nullptr;
  u_vec_init(v);

  mut_assert(true == u_vec_is_empty(v));

  /* 31, 30, ..., 1, 0 */
  each(i, N) {
    u_vec_put(v, 0, i);
  }

  /* <- 0, 1, ..., 30, 31 */
  each(i, N) {
    u_vec_put(v, -1, i);
  }

  /* <- 0, 1, ..., 30, 31 -> */
  each(i, N) {
    u_vec_put(v, N + i, i);
  }

  /* <- 31, 30, ..., 1, 0 -> */
  each(i, N) {
    u_vec_put(v, N * 2, i);
  }

  /* [       0       ]  [       2       ]  [        3      ]  [       1       ]
   * 31, 30, ..., 1, 0, 0, 1, ..., 30, 31, 31, 30, ..., 1, 0, 0, 1, ..., 30, 31
   * */

  mut_assert(false == u_vec_is_empty(v));
  mut_assert(N * 4 == u_vec_len(v));

  mut_assert(N - 1 == u_vec_at(v, 0));
  mut_assert(N - 1 == u_vec_at(v, -1));

  each(i, N) {
    mut_assert(N - 1 - i == u_vec_at(v, i));
  }

  each(i, N) {
    u_vec_pop(v, N * 2 + i);
  }

  mut_assert(N * 3 == u_vec_len(v));

  each(i, N) {
    mut_assert(N - 1 - i == u_vec_at(v, i));
  }

  each(i, N) {
    u_vec_pop(v, 0ul);
  }

  mut_assert(N * 2 == u_vec_len(v));

  each(i, N) {
    mut_assert(i == u_vec_at(v, i));
  }

  each(i, N) {
    u_vec_pop(v, -1);
  }

  mut_assert(N == u_vec_len(v));

  each(i, N) {
    mut_assert(i == u_vec_at(v, i));
  }

  u_vec_clear(v);

  mut_assert(0 == u_vec_len(v));

  /* 0, 1, 2, 3 */
  each(i, 4) {
    u_vec_put(v, -1, i);
  }

  /* 3, 2, 1, 0 */
  u_vec_at(v, 0, 3);
  u_vec_at(v, -1, 0);
  u_vec_at(v, 1, 2);
  u_vec_at(v, 2, 1);

  mut_assert(4 == u_vec_len(v));

  each(i, 4) {
    mut_assert(4 - 1 - i == u_vec_at(v, i));
  }

  u_vec_cleanup(v);
}

mut_test(vec_iterator) {
  u_vec_t(int) v = nullptr;
  u_vec_init(v);

  mut_assert(true == u_vec_is_empty(v));

  /* 0, 1, ..., 30, 31 */
  each(i, N) {
    u_vec_put(v, -1, i);
  }

  u_vec_for(v, i, it) {
    mut_assert(i == it);
  }

  u_vec_rfor(v, i, it) {
    mut_assert(i == it);
  }

  u_vec_cleanup(v);
}

mut_group(vec) {
  mut_add_test(vec_create, "create vec instance.");

  mut_add_test(vec_interface, "vec interface.");

  mut_add_test(vec_iterator, "vec iterator.");
}
