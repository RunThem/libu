test() {
  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    each(i, 0, N, +1) {
      auto it = u_dict_remove(t, i);

      mut_eq(i, it);
    }

    mut_eq(0, t->len);
  }

  return 0;
}
