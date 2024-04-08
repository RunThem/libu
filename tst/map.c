#include <mut.h>

#define umap_def ((int, char))
#include <u/u.h>

#define N 26

mut_test(map_create) {
  umap(int, char) m = nullptr;

  um_init(m);

  mut_assert(m != nullptr);
  mut_assert(0 == um_len(m));

  um_cleanup(m);

  mut_assert(m == nullptr);
}

mut_test(map_interface) {
  umap(int, char) m = nullptr;

  um_init(m);

  mut_assert(true == um_is_empty(m));

  each(i, N) {
    um_put(m, (int)i, 'a' + i);
  }

  mut_assert(false == um_is_empty(m));
  mut_assert(N == um_len(m));

  each(i, N) {
    mut_assert('a' + i == um_at(m, (int)i));
  }

  mut_assert(N == um_len(m));

  each(i, N) {
    mut_assert('a' + i == um_pop(m, (int)i));
  }

  mut_assert(true == um_is_empty(m));
  mut_assert(0 == um_len(m));

  um_cleanup(m);

  um_init(m);

  mut_assert(0 == um_len(m));

  each(i, N) {
    um_at(m, (int)i, 'a' + i);
  }

  mut_assert(0 == um_len(m));

  um_cleanup(m);

  um_init(m);

  each(i, N) {
    um_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    um_pop(m, (int)i);
  }

  each(i, N) {
    um_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    um_pop(m, (int)i);
  }

  each(i, N) {
    um_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    um_pop(m, (int)i);
  }

  each(i, N) {
    um_put(m, (int)i, 'a' + i);
  }

  um_put(m, 1000, '0');

  mut_assert(N + 1 == um_len(m));

  um_cleanup(m);
}

mut_test(map_iterator) {
  umap(int, char) m = nullptr;
  um_init(m);

  mut_assert(true == um_is_empty(m));

  each(i, N) {
    um_put(m, (int)i, 'a' + i);
  }

  um_for_all(m, k, v) {
    mut_assert(k + 'a' == +v);
  }

  um_rfor_all(m, k, v) {
    mut_assert(k + 'a' == +v);
  }

  um_cleanup(m);
}

mut_group(map) {

  mut_add_test(map_create, "create map instance.");

  mut_add_test(map_interface, "map interface.");

  mut_add_test(map_iterator, "map iterator.");
}
