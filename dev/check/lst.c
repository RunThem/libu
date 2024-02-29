#include <mut.h>
#include <u/u.h>

mut_test(lst_create) {
  ulst(int) v = nullptr;

  ul_init(v);

  mut_assert(v != nullptr);

  mut_assert(0 == ul_len(v));

  ul_cleanup(v);

  mut_assert(v == nullptr);
}

mut_test(lst_interface) {
  /*
   * push back
   * */
  ulst(int) l = nullptr;
  ul_init(l);

  mut_assert(true == ul_empty(l));

  int a1 = 1;
  int a2 = 2;
  int a3 = 3;
  int a4 = 4;
  int a5 = 5;

  /* 3 */
  ul_put(l, nullptr, &a3);

  /* 3, 5 */
  ul_put(l, ul_last(l), &a5);

  /* 1, 3, 5 */
  ul_put(l, nullptr, &a1);

  /* 1, 3, 4, 5 */
  ul_put(l, &a3, &a4);

  /* 1, 2, 3, 4, 5 */
  ul_put(l, ul_first(l), &a2);

  mut_assert(false == ul_empty(l));
  mut_assert(5 == ul_len(l));

  ul_pop(l, &a3);

  mut_assert(4 == ul_len(l));

  ul_pop(l, &a1);

  mut_assert(&a2 == ul_first(l));
  mut_assert(&a5 == ul_last(l));
  mut_assert(&a4 == ul_next(l, &a2));
  mut_assert(&a4 == ul_prev(l, &a5));

  ul_cleanup(l);
}

mut_test(lst_iterator) {
  ulst(int) l = nullptr;
  ul_init(l);

  mut_assert(true == ul_empty(l));

  int a1 = 1;
  int a2 = 2;
  int a3 = 3;
  int a4 = 4;
  int a5 = 5;

  ul_put(l, ul_last(l), &a1);
  ul_put(l, ul_last(l), &a2);
  ul_put(l, ul_last(l), &a3);
  ul_put(l, ul_last(l), &a4);
  ul_put(l, ul_last(l), &a5);

  size_t i = 1;
  int* it  = 0;
  ul_each(l, it) {
    mut_assert(*it == i++);
  }

  i = 5;
  ul_reach(l, it) {
    mut_assert(*it == i--);
  }

  i = 1;
  ul_foreach(l, int, it, {
    ;
    mut_assert(*it == i++);
  });

  i = 5;
  ul_rforeach(l, int, it, {
    ;
    mut_assert(*it == i--);
  });

  i = 1;
  for (auto it = ul_first(l); it; it = ul_next(l, it)) {
    mut_assert(*it == i++);
  }

  i = 5;
  for (auto it = ul_last(l); it; it = ul_prev(l, it)) {
    mut_assert(*it == i--);
  }

  ul_cleanup(l);
}

mut_group(lst) {
  mut_add_test(lst_create, "create lst instance.");

  mut_add_test(lst_interface, "lst interface.");

  mut_add_test(lst_iterator, "lst iterator.");
}
