test() {
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      auto it = u_vec_at(v, i);

      u_vec_at(v, i, it * 2);

      mut_eq(it * 2, u_vec_at(v, i));
    }

    mut_eq(N, v->len);
  }

  return 0;
}
