#include <u/u.h>

typedef struct {
  int a;
  int b;
} st_t;

fn_compe_def(st_t, x.a == y.a, x.a > y.a);

test() {
  /* #[[set<st_t>]] */
  u_set_t(st_t) s = nullptr;

  /* new */
  s = u_set_new(st_t, fn_cmp(st_t));
  mut_n(s);

  mut_e(0, u_set_len(s));
  mut_t(u_set_is_empty(s));

  mut_f(u_set_is_exist(s, me(st_t, .a = 0)));
  mut_f(u_set_is_exist(s, me(st_t, .a = 1)));
  mut_f(u_set_is_exist(s, me(st_t, .a = 2)));

  u_set_put(s, me(st_t, .a = 0, .b = 100));
  u_set_put(s, me(st_t, .a = 1, .b = 101));
  u_set_put(s, me(st_t, .a = 2, .b = 102));

  mut_e(3, u_set_len(s));
  mut_f(u_set_is_empty(s));

  mut_t(u_set_is_exist(s, me(st_t, .a = 0)));
  mut_t(u_set_is_exist(s, me(st_t, .a = 1)));
  mut_t(u_set_is_exist(s, me(st_t, .a = 2)));

  mut_f(u_set_is_exist(s, me(st_t, .a = 3)));
  mut_f(u_set_is_exist(s, me(st_t, .a = 4)));
  mut_f(u_set_is_exist(s, me(st_t, .a = -1)));
  mut_f(u_set_is_exist(s, me(st_t, .a = -2)));

  u_set_at(s, me(st_t, .a = 0), me(st_t, .a = 0, .b = 200));
  u_set_at(s, me(st_t, .a = 1), me(st_t, .a = 1, .b = 201));
  u_set_at(s, me(st_t, .a = 2), me(st_t, .a = 2, .b = 202));

  mut_e(3, u_set_len(s));
  mut_f(u_set_is_empty(s));

  mut_t(u_set_is_exist(s, me(st_t, .a = 0)));
  mut_t(u_set_is_exist(s, me(st_t, .a = 1)));
  mut_t(u_set_is_exist(s, me(st_t, .a = 2)));

  mut_f(u_set_is_exist(s, me(st_t, .a = 3)));
  mut_f(u_set_is_exist(s, me(st_t, .a = 4)));
  mut_f(u_set_is_exist(s, me(st_t, .a = -1)));
  mut_f(u_set_is_exist(s, me(st_t, .a = -2)));

  mut_e(200, u_set_at(s, me(st_t, .a = 0)).b);
  mut_e(201, u_set_at(s, me(st_t, .a = 1)).b);
  mut_e(202, u_set_at(s, me(st_t, .a = 2)).b);

  u_set_try(s, me(st_t, .a = 0)) {
    mut_e(200, it->b);
    it->b = 2 * it->b;
  }

  mut_e(3, u_set_len(s));
  mut_t(u_set_is_exist(s, me(st_t, .a = 0)));
  mut_e(400, u_set_at(s, me(st_t, .a = 0)).b);

  u_set_clear(s);

  mut_e(0, u_set_len(s));
  mut_t(u_set_is_empty(s));

  u_set_cleanup(s);
  mut_nn(s);

  /* init */
  u_set_init(s, fn_cmp(st_t));
  mut_n(s);

  mut_e(0, u_set_len(s));
  mut_t(u_set_is_empty(s));

  mut_f(u_set_is_exist(s, me(st_t, .a = 0)));
  mut_f(u_set_is_exist(s, me(st_t, .a = 1)));
  mut_f(u_set_is_exist(s, me(st_t, .a = 2)));

  u_set_put(s, me(st_t, .a = 0, .b = 100));
  u_set_put(s, me(st_t, .a = 1, .b = 101));
  u_set_put(s, me(st_t, .a = 2, .b = 102));

  mut_e(3, u_set_len(s));
  mut_f(u_set_is_empty(s));

  mut_t(u_set_is_exist(s, me(st_t, .a = 0)));
  mut_t(u_set_is_exist(s, me(st_t, .a = 1)));
  mut_t(u_set_is_exist(s, me(st_t, .a = 2)));

  mut_f(u_set_is_exist(s, me(st_t, .a = 3)));
  mut_f(u_set_is_exist(s, me(st_t, .a = 4)));
  mut_f(u_set_is_exist(s, me(st_t, .a = -1)));
  mut_f(u_set_is_exist(s, me(st_t, .a = -2)));

  u_set_at(s, me(st_t, .a = 0), me(st_t, .a = 0, .b = 200));
  u_set_at(s, me(st_t, .a = 1), me(st_t, .a = 1, .b = 201));
  u_set_at(s, me(st_t, .a = 2), me(st_t, .a = 2, .b = 202));

  mut_e(3, u_set_len(s));
  mut_f(u_set_is_empty(s));

  mut_t(u_set_is_exist(s, me(st_t, .a = 0)));
  mut_t(u_set_is_exist(s, me(st_t, .a = 1)));
  mut_t(u_set_is_exist(s, me(st_t, .a = 2)));

  mut_f(u_set_is_exist(s, me(st_t, .a = 3)));
  mut_f(u_set_is_exist(s, me(st_t, .a = 4)));
  mut_f(u_set_is_exist(s, me(st_t, .a = -1)));
  mut_f(u_set_is_exist(s, me(st_t, .a = -2)));

  mut_e(200, u_set_at(s, me(st_t, .a = 0)).b);
  mut_e(201, u_set_at(s, me(st_t, .a = 1)).b);
  mut_e(202, u_set_at(s, me(st_t, .a = 2)).b);

  u_set_try(s, me(st_t, .a = 0)) {
    mut_e(200, it->b);
    it->b = 2 * it->b;
  }

  mut_e(3, u_set_len(s));
  mut_t(u_set_is_exist(s, me(st_t, .a = 0)));
  mut_e(400, u_set_at(s, me(st_t, .a = 0)).b);

  u_set_clear(s);

  mut_e(0, u_set_len(s));
  mut_t(u_set_is_empty(s));

  u_set_cleanup(s);
  mut_nn(s);
}
