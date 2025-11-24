test() {
  u_dict_t(int, int) t = make_dict(N)->ref;

  each(i, 0, N, +1) {
    auto mut = u_dict_at_mut(t, i);

    mut_ptr_neq_nil(mut);
    mut_eq(i, *mut);

    *mut = i * 2;

    mut_eq(i * 2, u_dict_at(t, i));
  }

  mut_eq(N, t->len);

  return 0;
}