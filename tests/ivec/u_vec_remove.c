test() {
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, N - 1, 0, -1) {
      auto it = u_vec_remove(v, v->len - 1);
      mut_eq(i, it);
    }

    mut_eq(0, v->len);
  }
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, N - 1, 0, -1) {
      auto it = u_vec_remove(v, i);
      mut_eq(i, it);
    }

    mut_eq(0, v->len);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      auto it = u_vec_remove(v, 0);
      mut_eq(i, it);
    }

    mut_eq(0, v->len);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto it = u_vec_remove(v, 0);
    mut_eq(0, it);

    mut_eq(N - 1, v->len);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto it = u_vec_remove(v, N - 1);
    mut_eq(N - 1, it);

    mut_eq(N - 1, v->len);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto it = u_vec_remove(v, N / 2 - 1);
    mut_eq(it, N / 2 - 1);

    mut_eq(N - 1, v->len);
  }

  return 0;
}