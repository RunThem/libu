test() {
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, N - 1, 0, -1) {
      auto it = u_vec_remove_back(v);
      mut_eq(i, it);
    }

    mut_eq(0, v->len);
  }

  return 0;
}
