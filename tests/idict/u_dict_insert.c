test() {
  {
    u_dict_t(int, int) t = u_dict_new(int, int);

    each(i, 0, N, +1) {
      u_dict_insert(t, i, i);

      mut_eq(i + 1, t->len);
    }

    each(i, 0, N, +1) {
      auto it = u_dict_at(t, i);

      mut_eq(i, it);
    }

    mut_eq(N, t->len);
  }

  return 0;
}
