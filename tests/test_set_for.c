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

  u_set_for (s, it) {
    mut_e(it.a + 100, it.b);
  }

  u_set_rfor (s, it) {
    mut_e(it.a + 100, it.b);
  }

  u_set_cleanup(s);
}
