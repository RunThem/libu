

#include <mut.h>
#define uvec_def (int)
#include <u/u.h>

#define N 10

mut_test(vec_create) {
  uvec(int) v = nullptr;

  uv_init(v);

  mut_assert(v != nullptr);

  mut_assert(16 == uv_cap(v));
  mut_assert(0 == uv_len(v));

  uv_cleanup(v);

  mut_assert(v == nullptr);
}

mut_test(vec_interface) {
  /*
   * push back
   * */
  uvec(int) v = nullptr;
  uv_init(v);

  mut_assert(true == uv_is_empty(v));

  /* 31, 30, ..., 1, 0 */
  each(i, N) {
    uv_put(v, 0, i);
  }

  /* <- 0, 1, ..., 30, 31 */
  each(i, N) {
    uv_put(v, -1, i);
  }

  /* <- 0, 1, ..., 30, 31 -> */
  each(i, N) {
    uv_put(v, N + i, i);
  }

  /* <- 31, 30, ..., 1, 0 -> */
  each(i, N) {
    uv_put(v, N * 2, i);
  }

  /* [       0       ]  [       2       ]  [        3      ]  [       1       ]
   * 31, 30, ..., 1, 0, 0, 1, ..., 30, 31, 31, 30, ..., 1, 0, 0, 1, ..., 30, 31
   * */

  mut_assert(false == uv_is_empty(v));
  mut_assert(N * 4 == uv_len(v));

  mut_assert(N - 1 == uv_at(v, 0));
  mut_assert(N - 1 == uv_at(v, -1));

  each(i, N) {
    mut_assert(N - 1 - i == uv_at(v, i));
  }

  each(i, N) {
    uv_pop(v, N * 2 + i);
  }

  mut_assert(N * 3 == uv_len(v));

  each(i, N) {
    mut_assert(N - 1 - i == uv_at(v, i));
  }

  each(i, N) {
    uv_pop(v, 0ul);
  }

  mut_assert(N * 2 == uv_len(v));

  each(i, N) {
    mut_assert(i == uv_at(v, i));
  }

  each(i, N) {
    uv_pop(v, -1);
  }

  mut_assert(N == uv_len(v));

  each(i, N) {
    mut_assert(i == uv_at(v, i));
  }

  uv_clear(v);

  mut_assert(0 == uv_len(v));

  /* 0, 1, 2, 3 */
  each(i, 4) {
    uv_put(v, -1, i);
  }

  /* 3, 2, 1, 0 */
  uv_at(v, 0, 3);
  uv_at(v, -1, 0);
  uv_at(v, 1, 2);
  uv_at(v, 2, 1);

  mut_assert(4 == uv_len(v));

  each(i, 4) {
    mut_assert(4 - 1 - i == uv_at(v, i));
  }

  uv_cleanup(v);
}

mut_test(vec_iterator) {
  uvec(int) v = nullptr;
  uv_init(v);

  mut_assert(true == uv_is_empty(v));

  /* 0, 1, ..., 30, 31 */
  each(i, N) {
    uv_put(v, -1, i);
  }

  uv_for_all(v, i, it) {
    mut_assert(i == it);
  }

  uv_rfor_all(v, i, it) {
    mut_assert(i == it);
  }

  uv_cleanup(v);
}

mut_group(vec) {
  mut_add_test(vec_create, "create vec instance.");

  mut_add_test(vec_interface, "vec interface.");

  mut_add_test(vec_iterator, "vec iterator.");
}
