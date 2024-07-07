#include <mut.h>
#include <u/u.h>

#define N 20

mut_test(avl_create) {
  u_tree_t(size_t, int) t = nullptr; /* #[[tree<size_t, int>]] */

  u_tree_init(t, fn_cmp(int));

  mut_assert(t != nullptr);
  mut_assert(0 == u_tree_len(t));

  u_tree_cleanup(t);

  mut_assert(t == nullptr);
}

mut_test(avl_interface) {
  u_tree_t(size_t, int) t = nullptr;

  u_tree_init(t, fn_cmp(int));

  mut_assert(true == u_tree_is_empty(t));

  u_each (i, N) {
    u_tree_put(t, i, i * 2);
  }

  mut_assert(false == u_tree_is_empty(t));
  mut_assert(N == u_tree_len(t));

  u_each (i, N) {
    mut_assert(i * 2 == u_tree_at(t, i));
  }

  mut_assert(N == u_tree_len(t));

  u_each (i, N) {
    mut_assert(i * 2 == u_tree_pop(t, i));
  }

  mut_assert(true == u_tree_is_empty(t));
  mut_assert(0 == u_tree_len(t));

  u_tree_cleanup(t);

  u_tree_init(t, fn_cmp(int));

  mut_assert(0 == u_tree_len(t));

  u_each (i, N) {
    u_tree_at(t, i, i * 3);
  }

  mut_assert(0 == u_tree_len(t));
  mut_assert(true == u_tree_is_empty(t));

  u_each (i, N) {
    u_tree_put(t, i, i * 2);
  }

  mut_assert(N == u_tree_len(t));

  u_each (i, N) {
    u_tree_at(t, i, i * 3);
  }

  mut_assert(N == u_tree_len(t));

  u_each (i, N) {
    mut_assert(i * 3 == u_tree_at(t, i));
  }

  u_tree_for (t, k, v) {
    mut_assert(k * 3 == v);
  }

  u_tree_cleanup(t);
}

mut_test(avl_iterator) {
  u_tree_t(int, char) t = nullptr; /* #[[tree<int, char>]] */
  u_tree_init(t, fn_cmp(int));

  mut_assert(true == u_tree_is_empty(t));

  u_each (i, N) {
    u_tree_put(t, (int)i, 'a' + i);
  }

  u_tree_for (t, k, v) {
    mut_assert(k + 'a' == +v);
  }

  u_tree_rfor(t, k, v) {
    mut_assert(k + 'a' == +v);
  }

  u_tree_cleanup(t);
}

mut_group(avl) {

  mut_add_test(avl_create, "create avl instance.");

  mut_add_test(avl_interface, "avl interface.");

  mut_add_test(avl_iterator, "avl iterator.");
}
