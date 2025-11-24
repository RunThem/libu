test() {
  {
    u_vec_t(int) v = u_vec_new(int);

    each(i, 0, N, +1) {
      u_vec_insert(v, v->len, i);

      mut_eq(i + 1, v->len);
    }

    each(i, 0, N, +1) {
      auto it = u_vec_at(v, i);

      mut_eq(i, it);
    }

    mut_eq(N, v->len);
  }

  {
    u_vec_t(int) v = u_vec_new(int);

    each(i, 0, N, +1) {
      u_vec_insert(v, 0, i);

      mut_eq(i + 1, v->len);
    }

    each(i, N - 1, -1, -1) {
    }
  }

  return 0;
}
