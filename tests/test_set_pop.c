#include <u/u.h>

typedef struct {
  int a;
  int b;
} st_t;

fn_compe_def(st_t, x.a == y.a, x.a > y.a);

test() {
  /* #[[set<st_t>]] */
  u_set_t(st_t) s = nullptr;

  st_t items[N1W] = {};
  u_each (i, N1W) {
    items[i].a = i;
    items[i].b = i + 100;
  }

  s = u_set_new(st_t, fn_cmp(st_t));

  u_each (i, N1W) {
    u_set_put(s, items[i]);
  }

  mut_e(N1W, u_set_len(s));

  u_each (i, N1W) {
    mut_e(i + 100, u_set_pop(s, me(st_t, .a = i)).b);
  }

  mut_e(0, u_set_pop(s, me(st_t, .a = -1)).b);
  mut_e(0, u_set_pop(s, me(st_t, .a = N1W)).b);

  u_set_cleanup(s);
}
