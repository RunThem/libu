test() {
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_clear(v);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    int cnt = 0;
    u_vec_clear(v, {
      mut_eq(cnt, it);

      cnt++;
    });

    mut_eq(N, cnt);
    mut_eq(0, v->len);
    mut_ptr_neq_nil(v);
  }

  return 0;
}
