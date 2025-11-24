test() {
  u_vec_t(int) v = make_vec(N)->ref;

  each(i, 0, N, +1) {
    auto ref = u_vec_at_ref(v, i);

    mut_ptr_neq_nil(ref);

    mut_eq(i, *ref);
  }

  mut_eq(N, v->len);

  return 0;
}