#include <u/u.h>

typedef struct {
  int a;

  u_node_t next;
} st_t;

test() {
  /* #[[list<st_t>]] */
  u_list_t(st_t) l = nullptr;

  st_t items[] = {
      {.a = 0},
      {.a = 1},
      {.a = 2},
      {.a = 3},
      {.a = 4},
      {.a = 5},
  };

  l = u_list_new(st_t, next);
  mut_n(l);

  mut_e(0, u_list_len(l));
  mut_t(u_list_is_empty(l));

  /* 3 */
  u_list_put(l, &items[3]);

  mut_e(1, u_list_len(l));
  mut_e(&items[3], u_list_head(l));
  mut_e(&items[3], u_list_tail(l));
  mut_e(NULL, u_list_prev(l, &items[3]));
  mut_e(NULL, u_list_next(l, &items[3]));

  /* 3, 4 */
  u_list_put(l, &items[4]);

  mut_e(2, u_list_len(l));
  mut_e(&items[3], u_list_head(l));
  mut_e(&items[4], u_list_tail(l));
  mut_e(NULL, u_list_prev(l, &items[3]));
  mut_e(&items[4], u_list_next(l, &items[3]));
  mut_e(&items[3], u_list_prev(l, &items[4]));
  mut_e(NULL, u_list_next(l, &items[4]));

  /* 3, 4, 5 */
  u_list_put(l, &items[4], &items[5]);

  mut_e(3, u_list_len(l));
  mut_e(&items[3], u_list_head(l));
  mut_e(&items[5], u_list_tail(l));
  mut_e(NULL, u_list_prev(l, &items[3]));
  mut_e(&items[4], u_list_next(l, &items[3]));
  mut_e(&items[3], u_list_prev(l, &items[4]));
  mut_e(&items[5], u_list_next(l, &items[4]));
  mut_e(&items[4], u_list_prev(l, &items[5]));
  mut_e(NULL, u_list_next(l, &items[5]));

  /* 1, 3, 4, 5 */
  u_list_put(l, nullptr, &items[1]);

  mut_e(4, u_list_len(l));
  mut_e(&items[1], u_list_head(l));
  mut_e(&items[5], u_list_tail(l));
  mut_e(NULL, u_list_prev(l, &items[1]));
  mut_e(&items[3], u_list_next(l, &items[1]));
  mut_e(&items[1], u_list_prev(l, &items[3]));
  mut_e(&items[4], u_list_next(l, &items[3]));

  /* 1, 2, 3, 4, 5 */
  u_list_put(l, &items[1], &items[2]);

  mut_e(5, u_list_len(l));
  mut_e(&items[1], u_list_head(l));
  mut_e(&items[5], u_list_tail(l));
  mut_e(NULL, u_list_prev(l, &items[1]));
  mut_e(&items[2], u_list_next(l, &items[1]));
  mut_e(&items[1], u_list_prev(l, &items[2]));
  mut_e(&items[3], u_list_next(l, &items[2]));
  mut_e(&items[2], u_list_prev(l, &items[3]));
  mut_e(&items[4], u_list_next(l, &items[3]));

  /* 0, 1, 2, 3, 4, 5 */
  u_list_put(l, nullptr, &items[0]);

  mut_e(6, u_list_len(l));
  mut_e(&items[0], u_list_head(l));
  mut_e(&items[5], u_list_tail(l));
  mut_e(NULL, u_list_prev(l, &items[0]));
  mut_e(&items[1], u_list_next(l, &items[0]));
  mut_e(&items[0], u_list_prev(l, &items[1]));
  mut_e(&items[2], u_list_next(l, &items[1]));
  mut_e(&items[1], u_list_prev(l, &items[2]));
  mut_e(&items[3], u_list_next(l, &items[2]));
  mut_e(&items[2], u_list_prev(l, &items[3]));
  mut_e(&items[4], u_list_next(l, &items[3]));

  u_list_clear(l, it);

  mut_e(0, u_list_len(l));

  /* pop */
  /* 0, 1, 2, 3, 4, 5 */
  u_list_put(l, &items[0]);
  u_list_put(l, &items[1]);
  u_list_put(l, &items[2]);
  u_list_put(l, &items[3]);
  u_list_put(l, &items[4]);
  u_list_put(l, &items[5]);

  mut_e(6, u_list_len(l));

  /* 1, 2, 3, 4, 5 */
  mut_e(&items[0], u_list_pop(l));
  mut_e(5, u_list_len(l));
  mut_e(NULL, items[0].next.prev);
  mut_e(NULL, items[0].next.next);
  mut_e(NULL, items[0].next.ptr);
  mut_e(&items[1], u_list_head(l));
  mut_e(NULL, u_list_prev(l, &items[1]));
  mut_e(&items[2], u_list_next(l, &items[1]));

  /* 1, 3, 4, 5 */
  mut_e(&items[2], u_list_pop(l, &items[2]));
  mut_e(4, u_list_len(l));
  mut_e(NULL, items[2].next.prev);
  mut_e(NULL, items[2].next.next);
  mut_e(NULL, items[2].next.ptr);
  mut_e(&items[1], u_list_head(l));
  mut_e(NULL, u_list_prev(l, &items[1]));
  mut_e(&items[3], u_list_next(l, &items[1]));
  mut_e(&items[1], u_list_prev(l, &items[3]));

  /* 1, 3, 4 */
  mut_e(&items[5], u_list_pop(l, &items[5]));
  mut_e(3, u_list_len(l));
  mut_e(NULL, items[5].next.prev);
  mut_e(NULL, items[5].next.next);
  mut_e(NULL, items[5].next.ptr);
  mut_e(&items[1], u_list_head(l));
  mut_e(&items[4], u_list_tail(l));
  mut_e(&items[3], u_list_prev(l, &items[4]));
  mut_e(NULL, u_list_next(l, &items[4]));
  mut_e(&items[4], u_list_next(l, &items[3]));

  u_list_pop(l, &items[5]);
  mut_e(3, u_list_len(l));
  mut_e(&items[1], u_list_head(l));
  mut_e(&items[4], u_list_tail(l));

  u_list_pop(l);
  u_list_pop(l);
  u_list_pop(l);

  mut_e(0, u_list_len(l));
  mut_e(NULL, u_list_head(l));
  mut_e(NULL, u_list_tail(l));

  u_list_clear(l, it);

  mut_e(0, u_list_len(l));
  mut_e(NULL, u_list_head(l));
  mut_e(NULL, u_list_tail(l));

  /* for */
  /* 0, 1, 2, 3, 4, 5 */
  u_list_put(l, &items[0]);
  u_list_put(l, &items[1]);
  u_list_put(l, &items[2]);
  u_list_put(l, &items[3]);
  u_list_put(l, &items[4]);
  u_list_put(l, &items[5]);

  int i = 0;
  u_list_for (l, it) {
    mut_e(&items[i++], it);
  }

  i = 5;
  u_list_rfor (l, it) {
    mut_e(&items[i--], it);
  }
}
