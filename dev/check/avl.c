#if 0
#include <mut.h>
#include <u/core/avl.h>

#define N 20

mut_test(avl_create) {
  u_avl_t(int, int) t = u_avl_new(int, int, fn_cmp(int));

  mut_assert(t != nullptr);
  mut_assert(0 == u_avl_len(t));

  u_avl_cleanup(t);

  mut_assert(t == nullptr);
}

mut_test(avl_interface) {
  u_avl_t(int, int) t = u_avl_new(int, int, fn_cmp(int));

  mut_assert(true == u_avl_empty(t));

  each(i, N) {
    u_avl_push(t, i, i * 2);
  }

  mut_assert(false == u_avl_empty(t));
  mut_assert(N == u_avl_len(t));

  each(i, N) {
    mut_assert(i * 2 == u_avl_at(t, i));
  }

  mut_assert(N == u_avl_len(t));

  each(i, N) {
    mut_assert(i * 2 == u_avl_pop(t, i));
  }

  mut_assert(true == u_avl_empty(t));
  mut_assert(0 == u_avl_len(t));

  u_avl_cleanup(t);

  t = u_avl_new(int, int, fn_cmp(int));

  mut_assert(0 == u_avl_len(t));

  each(i, N) {
    u_avl_re(t, i, i * 3);
  }

  mut_assert(0 == u_avl_len(t));
  mut_assert(true == u_avl_empty(t));

  each(i, N) {
    u_avl_push(t, i, i * 2);
  }

  mut_assert(N == u_avl_len(t));

  each(i, N) {
    u_avl_re(t, i, i * 3);
  }

  mut_assert(N == u_avl_len(t));

  each(i, N) {
    mut_assert(i * 3 == u_avl_at(t, i));
  }

  u_avl_for(t) {
    mut_assert(t->key * 3 == t->val);
  }

  u_avl_cleanup(t);
}

mut_group(avl) {

  mut_add_test(avl_create, "create avl instance.");

  mut_add_test(avl_interface, "avl interface.");
}

#endif
