test() {
  {
    u_vec_t(int) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(i32_t);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);

    mut_eq(16, v->cap);
    mut_eq(0, v->len);
  }

  {
    u_vec_t(int) v = {};
    mut_ptr_eq_nil(v);

    v = u_vec_new(i32_t, 64);
    mut_ptr_neq_nil(v);
    mut_ptr_eq(v, v->ref);

    mut_eq(64, v->cap);
    mut_eq(0, v->len);
  }

  return 0;
}
