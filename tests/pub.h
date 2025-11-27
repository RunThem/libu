#include "mut.h"
#include "u/u.h"

u_vec_t(int) make_vec(i32_t n) {
  u_vec_t(int) v = u_vec_new(v, n);

  each(i, 0, n, +1) {
    u_vec_insert_back(v, i);
  }

  return v->ref;
}

u_tree_t(int, int) make_tree(i32_t n) {
  u_tree_t(int, int) t = u_tree_new(t, fn_cmp(int));

  each(i, 0, n, +1) {
    u_tree_insert(t, i, i);
  }

  return t->ref;
}

u_dict_t(int, int) make_dict(i32_t n) {
  u_dict_t(int, int) d = u_dict_new(d);

  each(i, 0, n, +1) {
    u_dict_insert(d, i, i);
  }

  return d->ref;
}
