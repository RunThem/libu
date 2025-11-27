test() {
  u_vec_t(int) v = make_vec(N)->ref;

  each(i, 0, N, +1) {
    auto mut = u_vec_at_mut(v, i);

    mut_ptr_neq_nil(mut);
    mut_eq(i, *mut);

    *mut = i * 2;

    mut_eq(i * 2, u_vec_at(v, i));
  }

  mut_eq(N, v->len);

  return 0;
}