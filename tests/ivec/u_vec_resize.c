test() {
  u_vec_t(int) v = u_vec_new(int);

  mut_eq(0, v->len);
  mut_eq(16, v->cap);

  u_vec_resize(v, 16 * 2);
  mut_ptr_neq_nil(v);

  mut_eq(0, v->len);
  mut_eq(16 * 2, v->cap);

  return 0;
}
