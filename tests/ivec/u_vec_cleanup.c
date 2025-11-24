test() {
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_cleanup(v);
    mut_ptr_eq_nil(v);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    int cnt = 0;
    u_vec_cleanup(v, {
      mut_eq(cnt, it);

      cnt++;
    });

    mut_eq(N, cnt);
    mut_ptr_eq_nil(v);
  }

  return 0;
}
