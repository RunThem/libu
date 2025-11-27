test() {
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_back(v, i);

      mut_eq(i + 1, v->len);
    }

    each(i, 0, N, +1) {
      auto it = u_vec_at(v, i);

      mut_eq(i, it);
    }

    mut_eq(N, v->len);
  }

  return 0;
}
