#include <mut.h>

#define u_list_defs u_defs(list, int)
#include <u/u.h>

mut_test(lst_create) {
  u_list_t(int) v = nullptr;

  u_list_init(v);

  mut_assert(v != nullptr);

  mut_assert(0 == u_list_len(v));

  u_list_cleanup(v);

  mut_assert(v == nullptr);
}

mut_test(lst_interface) {
  /*
   * push back
   * */
  u_list_t(int) l = nullptr;

  u_list_init(l);

  mut_assert(true == u_list_is_empty(l));

  int a1 = 1;
  int a2 = 2;
  int a3 = 3;
  int a4 = 4;
  int a5 = 5;

  /* 3 */
  u_list_put(l, nullptr, &a3);

  /* 3, 5 */
  u_list_put(l, u_list_last(l), &a5);

  /* 1, 3, 5 */
  u_list_put(l, nullptr, &a1);

  /* 1, 3, 4, 5 */
  u_list_put(l, &a3, &a4);

  /* 1, 2, 3, 4, 5 */
  u_list_put(l, u_list_first(l), &a2);

  mut_assert(false == u_list_is_empty(l));
  mut_assert(5 == u_list_len(l));

  u_list_pop(l, &a3);

  mut_assert(4 == u_list_len(l));

  u_list_pop(l, &a1);

  mut_assert(&a2 == u_list_first(l));
  mut_assert(&a5 == u_list_last(l));
  mut_assert(&a4 == u_list_next(l, &a2));
  mut_assert(&a4 == u_list_prev(l, &a5));
  mut_assert(nullptr == u_list_next(l, &a5));
  mut_assert(nullptr == u_list_prev(l, &a2));

  u_list_cleanup(l);
}

mut_test(lst_iterator) {
  u_list_t(int) l = nullptr;

  u_list_init(l);

  mut_assert(true == u_list_is_empty(l));

  int a1 = 1;
  int a2 = 2;
  int a3 = 3;
  int a4 = 4;
  int a5 = 5;

  u_list_put(l, u_list_last(l), &a1);
  u_list_put(l, u_list_last(l), &a2);
  u_list_put(l, u_list_last(l), &a3);
  u_list_put(l, u_list_last(l), &a4);
  u_list_put(l, u_list_last(l), &a5);

  size_t i = 1;
  u_list_for_all(l, it) {
    mut_assert(*it == i++);
  }

  i = 5;
  u_list_rfor_all(l, it) {
    mut_assert(*it == i--);
  }

  i = 1;
  for (auto it = u_list_first(l); it; it = u_list_next(l, it)) {
    mut_assert(*it == i++);
  }

  i = 5;
  for (auto it = u_list_last(l); it; it = u_list_prev(l, it)) {
    mut_assert(*it == i--);
  }

  u_list_cleanup(l);
}

mut_group(lst) {
  mut_add_test(lst_create, "create lst instance.");

  mut_add_test(lst_interface, "lst interface.");

  mut_add_test(lst_iterator, "lst iterator.");
}
