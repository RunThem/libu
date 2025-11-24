test() {
  u_dict_t(int, int) t = make_dict(N)->ref;

  each(i, 0, N, +1) {
    auto ref = u_dict_at_ref(t, i);

    mut_ptr_neq_nil(ref);

    mut_eq(i, *ref);
  }

  mut_eq(N, t->len);
}
