test() {
  {
    u_vec_t(int) v = u_vec_new(v);

    bool flag = true;

    u_vec_each (v, it) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool flag = true;
    int i     = 0;

    u_vec_each (v, it) {
      flag = false;
      mut_eq(i, it);

      i++;
    }

    mut_false(flag);
    mut_eq(i, N);
  }

  return 0;
}