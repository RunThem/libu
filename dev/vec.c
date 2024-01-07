#include <u/core/itbl.h>
#include <u/core/ivec.h>

int vec_test() {
#if 0
  u_vec(int) v = u_vec_new(int);

  u_vec_put(v, 0, 10);

  each(i, 10) {
    u_vec_put(v, 0, i);
  }

  infln("len size is %zu", u_vec_len(v));

  u_vec_for(v, i) {
    infln("[%zu] is %d", i, v.item);
  }

  each(i, 10) {
    (void)u_vec_pop(v, 0);
  }

  u_vec_re(v, 0, 5);

  infln("len size is %zu", u_vec_len(v));

  u_vec_for(v, i) {
    infln("[%zu] is %d", i, v.item);
  }

  infln("first is %d", u_vec_at(v, 0));

  u_vec_clear(v);
  u_vec_cleanup(v);
#endif

  extern int tbl_test();
  tbl_test();

  return 0;
}

int tbl_test() {
  u_tbl(int, char*) m = u_tbl_new(int, char*, fn_eq(int));

  u_tbl_put(m, 1, "1");
  u_tbl_put(m, 2, "3");
  u_tbl_put(m, 3, "4");

  u_tbl_for(m) {
    infln("[%d] = %s", m.key, m.val);
  }

  infln("[3] is %s", u_tbl_at(m, 3));

  u_tbl_clear(m);

  u_tbl_cleanup(m);

  return 0;
}
