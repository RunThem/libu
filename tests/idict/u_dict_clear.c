test() {
  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    u_dict_clear(t);
    mut_ptr_neq_nil(t);
    mut_eq(0, t->len);
  }

  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    int cnt = 0;

    u_dict_clear(t, {
      mut_eq(it.key, it.val);

      cnt++;
    });

    mut_eq(N, cnt);
    mut_eq(0, t->len);

    mut_ptr_neq_nil(t);
  }

  return 0;
}
