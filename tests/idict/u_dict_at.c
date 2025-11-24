test() {
  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    each(i, 0, N, +1) {
      mut_eq(i, u_dict_at(t, i));
    }
  }

  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    each(i, 0, N, +1) {
      auto it = u_dict_at(t, i);

      u_dict_at(t, i, it * 2);

      mut_eq(it * 2, u_dict_at(t, i));
    }
  }

  return 0;
}
