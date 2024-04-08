#include "u/iavl.h"

#include <mut.h>

#define uavl_def ((size_t, int), (int, char))
#include <u/u.h>

#define N 20

mut_test(avl_create) {
  uavl(size_t, int) t = nullptr;

  ut_init(t, fn_cmp(int));

  mut_assert(t != nullptr);
  mut_assert(0 == ut_len(t));

  ut_cleanup(t);

  mut_assert(t == nullptr);
}

mut_test(avl_interface) {
  uavl(size_t, int) t = nullptr;

  ut_init(t, fn_cmp(int));

  mut_assert(true == ut_is_empty(t));

  each(i, N) {
    ut_put(t, i, i * 2);
  }

  mut_assert(false == ut_is_empty(t));
  mut_assert(N == ut_len(t));

  each(i, N) {
    mut_assert(i * 2 == ut_at(t, i));
  }

  mut_assert(N == ut_len(t));

  each(i, N) {
    mut_assert(i * 2 == ut_pop(t, i));
  }

  mut_assert(true == ut_is_empty(t));
  mut_assert(0 == ut_len(t));

  ut_cleanup(t);

  ut_init(t, fn_cmp(int));

  mut_assert(0 == ut_len(t));

  each(i, N) {
    ut_at(t, i, i * 3);
  }

  mut_assert(0 == ut_len(t));
  mut_assert(true == ut_is_empty(t));

  each(i, N) {
    ut_put(t, i, i * 2);
  }

  mut_assert(N == ut_len(t));

  each(i, N) {
    ut_at(t, i, i * 3);
  }

  mut_assert(N == ut_len(t));

  each(i, N) {
    mut_assert(i * 3 == ut_at(t, i));
  }

  ut_for_all(t, k, v) {
    mut_assert(k * 3 == v);
  }

  ut_cleanup(t);
}

mut_test(avl_iterator) {
  uavl(int, char) t = nullptr;
  ut_init(t, fn_cmp(int));

  mut_assert(true == ut_is_empty(t));

  each(i, N) {
    ut_put(t, (int)i, 'a' + i);
  }

  ut_for_all(t, k, v) {
    mut_assert(k + 'a' == +v);
  }

  ut_rfor_all(t, k, v) {
    mut_assert(k + 'a' == +v);
  }

  ut_cleanup(t);
}

mut_group(avl) {

  mut_add_test(avl_create, "create avl instance.");

  mut_add_test(avl_interface, "avl interface.");

  mut_add_test(avl_iterator, "avl iterator.");
}
