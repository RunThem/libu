#include <mut.h>
#include <u/core/vec.h>

#define N 32

mut_test(vec_create) {
  u_vec_t(int) v = u_vec_new(int);

  mut_assert(v != nullptr);

  mut_assert(16 == u_vec_cap(v));
  mut_assert(0 == u_vec_len(v));

  u_vec_cleanup(v);

  mut_assert(v == nullptr);

  /*
   * < default cap
   * */
  v = u_vec_new(int, 10);

  mut_assert(v != nullptr);

  mut_assert(16 == u_vec_cap(v));
  mut_assert(0 == u_vec_len(v));

  u_vec_cleanup(v);

  mut_assert(v == nullptr);

  /*
   * > default cap
   * */
  v = u_vec_new(int, 100);

  mut_assert(v != nullptr);

  mut_assert(100 == u_vec_cap(v));
  mut_assert(0 == u_vec_len(v));

  u_vec_cleanup(v);

  mut_assert(v == nullptr);
}

mut_test(vec_interface) {
  /*
   * push back
   * */
  u_vec_t(int) v = u_vec_new(int);

  mut_assert(true == u_vec_empty(v));

  /* 31, 30, ..., 1, 0 */
  each(i, N) {
    u_vec_push_front(v, i);
  }

  /* <- 0, 1, ..., 30, 31 */
  each(i, N) {
    u_vec_push_back(v, i);
  }

  /* <- 0, 1, ..., 30, 31 -> */
  each(i, N) {
    u_vec_push(v, N + i, i);
  }

  /* <- 31, 30, ..., 1, 0 -> */
  each(i, N) {
    u_vec_push(v, N * 2, i);
  }

  /* [       0       ]  [       2       ]  [        3      ]  [       1       ]
   * 31, 30, ..., 1, 0, 0, 1, ..., 30, 31, 31, 30, ..., 1, 0, 0, 1, ..., 30, 31
   * */

  mut_assert(false == u_vec_empty(v));
  mut_assert(N * 4 == u_vec_len(v));

  mut_assert(N - 1 == u_vec_at_front(v));
  mut_assert(N - 1 == u_vec_at_back(v));
  printf("%d", u_vec_at_back(v));

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
    u_vec_pop_front(v);
  }

  mut_assert(N * 2 == u_vec_len(v));

  each(i, N) {
    mut_assert(i == u_vec_at(v, i));
  }

  each(i, N) {
    u_vec_pop_back(v);
  }

  mut_assert(N == u_vec_len(v));

  each(i, N) {
    mut_assert(i == u_vec_at(v, i));
  }

  u_vec_clear(v);

  mut_assert(0 == u_vec_len(v));

  each(i, 4) {
    u_vec_push_back(v, i);
  }

  u_vec_re_front(v, 3);
  u_vec_re_back(v, 0);
  u_vec_re(v, 1, 2);
  u_vec_re(v, 2, 1);

  mut_assert(4 == u_vec_len(v));

  each(i, 4) {
    mut_assert(4 - 1 - i == u_vec_at(v, i));
  }

  u_vec_cleanup(v);
}

mut_test(vec_iterator) {
  u_vec_t(int) v = u_vec_new(int);

  mut_assert(true == u_vec_empty(v));

  /* 0, 1, ..., 30, 31 */
  each(i, N) {
    u_vec_push_back(v, i);
  }

  u_vec_for(v, i) {
    mut_assert(i == v->item);
  }

  u_vec_rfor(v, i) {
    mut_assert(i == v->item);
  }

  u_vec_cleanup(v);
}

mut_group(vec) {
  mut_add_test(vec_create, "create vec instance.");

  mut_add_test(vec_interface, "vec interface.");

  mut_add_test(vec_iterator, "vec iterator.");
}
