#include <mut.h>

#define u_map_defs u_defs(map, (int, char))
#include <u/u.h>

#define N 26

mut_test(map_create) {
  u_map_t(int, char) m = nullptr;

  u_map_init(m);

  mut_assert(m != nullptr);
  mut_assert(0 == u_map_len(m));

  u_map_cleanup(m);

  mut_assert(m == nullptr);
}

mut_test(map_interface) {
  u_map_t(int, char) m = nullptr;

  u_map_init(m);

  mut_assert(true == u_map_is_empty(m));

  each(i, N) {
    u_map_put(m, (int)i, 'a' + i);
  }

  mut_assert(false == u_map_is_empty(m));
  mut_assert(N == u_map_len(m));

  each(i, N) {
    mut_assert('a' + i == u_map_at(m, (int)i));
  }

  mut_assert(N == u_map_len(m));

  each(i, N) {
    mut_assert('a' + i == u_map_pop(m, (int)i));
  }

  mut_assert(true == u_map_is_empty(m));
  mut_assert(0 == u_map_len(m));

  u_map_cleanup(m);

  u_map_init(m);

  mut_assert(0 == u_map_len(m));

  each(i, N) {
    u_map_at(m, (int)i, 'a' + i);
  }

  mut_assert(0 == u_map_len(m));

  u_map_cleanup(m);

  u_map_init(m);

  each(i, N) {
    u_map_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    u_map_pop(m, (int)i);
  }

  each(i, N) {
    u_map_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    u_map_pop(m, (int)i);
  }

  each(i, N) {
    u_map_put(m, (int)i, 'a' + i);
  }

  each(i, N) {
    u_map_pop(m, (int)i);
  }

  each(i, N) {
    u_map_put(m, (int)i, 'a' + i);
  }

  u_map_put(m, 1000, '0');

  mut_assert(N + 1 == u_map_len(m));

  u_map_cleanup(m);
}

mut_test(map_iterator) {
  u_map_t(int, char) m = nullptr;
  u_map_init(m);

  mut_assert(true == u_map_is_empty(m));

  each(i, N) {
    u_map_put(m, (int)i, 'a' + i);
  }

  u_map_for(m, k, v) {
    mut_assert(k + 'a' == +v);
  }

  u_map_cleanup(m);
}

mut_group(map) {

  mut_add_test(map_create, "create map instance.");

  mut_add_test(map_interface, "map interface.");

  mut_add_test(map_iterator, "map iterator.");
}
