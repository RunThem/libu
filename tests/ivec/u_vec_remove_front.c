test() {

  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      auto it = u_vec_remove_front(v);
      mut_eq(i, it);
    }

    mut_eq(0, v->len);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto it = u_vec_remove_front(v);
    mut_eq(0, it);

    mut_eq(N - 1, v->len);
  }

  return 0;
}
