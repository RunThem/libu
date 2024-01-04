#include <u/core/ivec.h>

int vec_test() {
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

  return 0;
}
