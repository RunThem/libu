#include <mut.h>
#include <u/core/map.h>

#define N 26

mut_test(map_create) {
  u_map_t(int, char) m = u_map_new(int, char, fn_eq(int), U_MAP_INT_HASH_FN);

  mut_assert(m != nullptr);
  mut_assert(0 == u_map_len(m));

  u_map_cleanup(m);

  mut_assert(m == nullptr);
}

mut_test(map_interface) {
  u_map_t(int, char) m = u_map_new(int, char, fn_eq(int), U_MAP_INT_HASH_FN);

  mut_assert(true == u_map_empty(m));

  each(i, N) {
    u_map_push(m, i, 'a' + i);
  }

  mut_assert(false == u_map_empty(m));
  mut_assert(N == u_map_len(m));

  each(i, N) {
    mut_assert('a' + i == u_map_at(m, i));
  }

  mut_assert(N == u_map_len(m));

  each(i, N) {
    mut_assert('a' + i == u_map_pop(m, i));
  }

  mut_assert(true == u_map_empty(m));
  mut_assert(0 == u_map_len(m));

  u_map_cleanup(m);

  m = u_map_new(int, char, fn_eq(int), U_MAP_INT_HASH_FN);

  mut_assert(0 == u_map_len(m));

  each(i, N) {
    u_map_re(m, i, 'a' + i);
  }

  mut_assert(0 == u_map_len(m));

  u_map_cleanup(m);

  m = u_map_new(int, char, fn_eq(int), U_MAP_INT_HASH_FN);

  each(i, N) {
    u_map_push(m, i, 'a' + i);
  }

  each(i, N) {
    u_map_pop(m, i);
  }

  each(i, N) {
    u_map_push(m, i, 'a' + i);
  }

  each(i, N) {
    u_map_pop(m, i);
  }

  each(i, N) {
    u_map_push(m, i, 'a' + i);
  }

  each(i, N) {
    u_map_pop(m, i);
  }

  each(i, N) {
    u_map_push(m, i, 'a' + i);
  }

  u_map_push(m, 1000, '0');

  mut_assert(N + 1 == u_map_len(m));

  u_map_cleanup(m);
}

mut_group(map) {

  mut_add_test(map_create, "create map instance.");

  mut_add_test(map_interface, "map interface.");
}
