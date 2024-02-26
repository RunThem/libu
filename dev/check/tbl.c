#include <mut.h>
#include <u/u.h>

#define N 26

mut_test(tbl_create) {
  utbl(int, char) m = nullptr;

  ut_init(m);

  mut_assert(m != nullptr);
  mut_assert(0 == ut_len(m));

  ut_cleanup(m);

  mut_assert(m == nullptr);
}

mut_test(tbl_interface) {
  utbl(int, char) m = nullptr;

  ut_init(m);

  mut_assert(true == ut_empty(m));

  each(i, N) {
    ut_put(m, (int)i, 'a' + i);
  }

  mut_assert(false == ut_empty(m));
  mut_assert(N == ut_len(m));

  each(i, N) {
    mut_assert('a' + i == ut_at(m, (int)i));
  }

  mut_assert(N == ut_len(m));

  each(i, N) {
    mut_assert('a' + i == ut_pop(m, (int)i));
  }

  mut_assert(true == ut_empty(m));
  mut_assert(0 == ut_len(m));

  ut_cleanup(m);

  ut_init(m);

  mut_assert(0 == ut_len(m));

  each(i, N) {
    ut_at(m, (int)i, 'a' + i);
  }

  mut_assert(0 == ut_len(m));

  ut_cleanup(m);

  ut_init(m);

  each(i, N) {
    ut_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    ut_pop(m, (int)i);
  }

  each(i, N) {
    ut_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    ut_pop(m, (int)i);
  }

  each(i, N) {
    ut_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    ut_pop(m, (int)i);
  }

  each(i, N) {
    ut_put(m, (int)i, 'a' + i);
  }

  ut_put(m, 1000, '0');

  mut_assert(N + 1 == ut_len(m));

  ut_cleanup(m);
}

mut_group(tbl) {

  mut_add_test(tbl_create, "create tbl instance.");

  mut_add_test(tbl_interface, "tbl interface.");
}
