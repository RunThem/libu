#include <u/u.h>

test() {
  /* #[[heap<int>]] */
  u_heap_t(int) h = nullptr;

  /* new min */
  h = u_heap_new(int, U_ORDER_ASCEND, fn_cmp(int));
  mut_n(h);

  mut_e(0, u_heap_len(h));
  mut_t(u_heap_is_empty(h));

  u_heap_put(h, 5);
  u_heap_put(h, 3);
  u_heap_put(h, 6);
  u_heap_put(h, 4);
  u_heap_put(h, 1);
  u_heap_put(h, 2);

  mut_e(6, u_heap_len(h));
  mut_f(u_heap_is_empty(h));

  mut_e(1, u_heap_at(h));
  mut_e(6, u_heap_len(h));
  mut_e(1, u_heap_pop(h));
  mut_e(5, u_heap_len(h));

  mut_e(2, u_heap_at(h));
  mut_e(5, u_heap_len(h));
  mut_e(2, u_heap_pop(h));
  mut_e(4, u_heap_len(h));

  mut_e(3, u_heap_at(h));
  mut_e(3, u_heap_pop(h));

  mut_e(4, u_heap_at(h));
  mut_e(4, u_heap_pop(h));
  mut_e(2, u_heap_len(h));

  mut_e(5, u_heap_at(h));
  mut_e(5, u_heap_pop(h));

  mut_e(6, u_heap_at(h));
  mut_e(6, u_heap_pop(h));

  mut_e(0, u_heap_len(h));
  mut_t(u_heap_is_empty(h));

  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));

  u_heap_put(h, 4);
  mut_e(1, u_heap_len(h));

  u_heap_clear(h);
  mut_e(0, u_heap_len(h));

  u_heap_cleanup(h);

  /* init min */
  u_heap_init(h, true, fn_cmp(int));
  mut_n(h);

  mut_e(0, u_heap_len(h));
  mut_t(u_heap_is_empty(h));

  u_heap_put(h, 5);
  u_heap_put(h, 3);
  u_heap_put(h, 6);
  u_heap_put(h, 4);
  u_heap_put(h, 1);
  u_heap_put(h, 2);

  mut_e(6, u_heap_len(h));
  mut_f(u_heap_is_empty(h));

  mut_e(1, u_heap_at(h));
  mut_e(6, u_heap_len(h));
  mut_e(1, u_heap_pop(h));
  mut_e(5, u_heap_len(h));

  mut_e(2, u_heap_at(h));
  mut_e(5, u_heap_len(h));
  mut_e(2, u_heap_pop(h));
  mut_e(4, u_heap_len(h));

  mut_e(3, u_heap_at(h));
  mut_e(3, u_heap_pop(h));

  mut_e(4, u_heap_at(h));
  mut_e(4, u_heap_pop(h));
  mut_e(2, u_heap_len(h));

  mut_e(5, u_heap_at(h));
  mut_e(5, u_heap_pop(h));

  mut_e(6, u_heap_at(h));
  mut_e(6, u_heap_pop(h));

  mut_e(0, u_heap_len(h));
  mut_t(u_heap_is_empty(h));

  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));

  u_heap_put(h, 4);
  mut_e(1, u_heap_len(h));

  u_heap_clear(h);
  mut_e(0, u_heap_len(h));

  u_heap_cleanup(h);

  /* new max */
  h = u_heap_new(int, U_ORDER_DESCEND, fn_cmp(int));
  mut_n(h);

  mut_e(0, u_heap_len(h));
  mut_t(u_heap_is_empty(h));

  u_heap_put(h, 5);
  u_heap_put(h, 3);
  u_heap_put(h, 6);
  u_heap_put(h, 4);
  u_heap_put(h, 1);
  u_heap_put(h, 2);

  mut_e(6, u_heap_len(h));
  mut_f(u_heap_is_empty(h));

  mut_e(6, u_heap_at(h));
  mut_e(6, u_heap_len(h));
  mut_e(6, u_heap_pop(h));
  mut_e(5, u_heap_len(h));

  mut_e(5, u_heap_at(h));
  mut_e(5, u_heap_len(h));
  mut_e(5, u_heap_pop(h));
  mut_e(4, u_heap_len(h));

  mut_e(4, u_heap_at(h));
  mut_e(4, u_heap_pop(h));

  mut_e(3, u_heap_at(h));
  mut_e(3, u_heap_pop(h));
  mut_e(2, u_heap_len(h));

  mut_e(2, u_heap_at(h));
  mut_e(2, u_heap_pop(h));

  mut_e(1, u_heap_at(h));
  mut_e(1, u_heap_pop(h));

  mut_e(0, u_heap_len(h));
  mut_t(u_heap_is_empty(h));

  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));

  u_heap_put(h, 4);
  mut_e(1, u_heap_len(h));

  u_heap_clear(h);
  mut_e(0, u_heap_len(h));

  u_heap_cleanup(h);

  /* init max */
  u_heap_init(h, U_ORDER_DESCEND, fn_cmp(int));
  mut_n(h);

  mut_e(0, u_heap_len(h));
  mut_t(u_heap_is_empty(h));

  u_heap_put(h, 5);
  u_heap_put(h, 3);
  u_heap_put(h, 6);
  u_heap_put(h, 4);
  u_heap_put(h, 1);
  u_heap_put(h, 2);

  mut_e(6, u_heap_len(h));
  mut_f(u_heap_is_empty(h));

  mut_e(6, u_heap_at(h));
  mut_e(6, u_heap_len(h));
  mut_e(6, u_heap_pop(h));
  mut_e(5, u_heap_len(h));

  mut_e(5, u_heap_at(h));
  mut_e(5, u_heap_len(h));
  mut_e(5, u_heap_pop(h));
  mut_e(4, u_heap_len(h));

  mut_e(4, u_heap_at(h));
  mut_e(4, u_heap_pop(h));

  mut_e(3, u_heap_at(h));
  mut_e(3, u_heap_pop(h));
  mut_e(2, u_heap_len(h));

  mut_e(2, u_heap_at(h));
  mut_e(2, u_heap_pop(h));

  mut_e(1, u_heap_at(h));
  mut_e(1, u_heap_pop(h));

  mut_e(0, u_heap_len(h));
  mut_t(u_heap_is_empty(h));

  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_at(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));

  u_heap_put(h, 4);
  mut_e(1, u_heap_len(h));

  u_heap_clear(h);
  mut_e(0, u_heap_len(h));

  u_heap_cleanup(h);
}
