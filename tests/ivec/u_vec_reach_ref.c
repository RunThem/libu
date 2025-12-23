test() {
  {
    u_vec_t(int) v = u_vec_new(v);

    bool flag = true;

    u_vec_reach_ref (v, it) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool flag = true;
    int i     = N - 1;

    u_vec_reach_ref (v, it) {
      mut_ptr_neq_nil(it);
      flag = false;
      mut_eq(i, *it);

      i--;
    }

    mut_false(flag);
    mut_eq(i, -1);
  }

  return 0;
}