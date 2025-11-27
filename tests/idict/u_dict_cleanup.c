test() {
  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    u_dict_cleanup(t);
    mut_ptr_eq_nil(t);
  }

  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    int cnt = 0;
    u_dict_cleanup(t, {
      mut_eq(it.key, it.val);

      cnt++;
    });

    mut_eq(N, cnt);
    mut_ptr_eq_nil(t);
  }

  return 0;
}
