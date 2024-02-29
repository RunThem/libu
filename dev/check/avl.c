#include <mut.h>
#include <u/u.h>

#define N 20

mut_test(avl_create) {
  uavl(size_t, int) t = nullptr;

  ua_init(t, fn_cmp(int));

  mut_assert(t != nullptr);
  mut_assert(0 == ua_len(t));

  ua_cleanup(t);

  mut_assert(t == nullptr);
}

mut_test(avl_interface) {
  uavl(size_t, int) t = nullptr;

  ua_init(t, fn_cmp(int));

  mut_assert(true == ua_empty(t));

  each(i, N) {
    ua_put(t, i, i * 2);
  }

  mut_assert(false == ua_empty(t));
  mut_assert(N == ua_len(t));

  each(i, N) {
    mut_assert(i * 2 == ua_at(t, i));
  }

  mut_assert(N == ua_len(t));

  each(i, N) {
    mut_assert(i * 2 == ua_pop(t, i));
  }

  mut_assert(true == ua_empty(t));
  mut_assert(0 == ua_len(t));

  ua_cleanup(t);

  ua_init(t, fn_cmp(int));

  mut_assert(0 == ua_len(t));

  each(i, N) {
    ua_at(t, i, i * 3);
  }

  mut_assert(0 == ua_len(t));
  mut_assert(true == ua_empty(t));

  each(i, N) {
    ua_put(t, i, i * 2);
  }

  mut_assert(N == ua_len(t));

  each(i, N) {
    ua_at(t, i, i * 3);
  }

  mut_assert(N == ua_len(t));

  each(i, N) {
    mut_assert(i * 3 == ua_at(t, i));
  }

  size_t k;
  int v;
  ua_each(t, k, v) {
    mut_assert(k * 3 == v);
  }

  ua_cleanup(t);
}

mut_test(avl_iterator) {
  uavl(int, char) t = nullptr;
  ua_init(t, fn_cmp(int));

  mut_assert(true == ua_empty(t));

  each(i, N) {
    ua_put(t, (int)i, 'a' + i);
  }

  int k  = 0;
  char v = 0;
  ua_each(t, k, v) {
    mut_assert(k + 'a' == +v);
  }

  ua_reach(t, k, v) {
    mut_assert(k + 'a' == +v);
  }

  ua_foreach(t, int, k, char, v, {
    ;
    mut_assert(k + 'a' == +v);
  });

  ua_foreach(t, int, k, char, v, {
    ;
    mut_assert(k + 'a' == +v);
  });

  ua_cleanup(t);
}

mut_group(avl) {

  mut_add_test(avl_create, "create avl instance.");

  mut_add_test(avl_interface, "avl interface.");

  mut_add_test(avl_iterator, "avl iterator.");
}
