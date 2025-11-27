test() {
  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool flag = true;

    u_vec_reach_if (v, it, it % 2 == 0) {
      flag = false;
    }

    mut_false(flag);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool flag = true;
    int i     = N-2;

    u_vec_reach_if (v, it, it % 2 == 0) {
      flag = false;
      mut_eq(i, it);

      i-=2;
    }

    mut_false(flag);
    mut_eq(i, -2);
  }

  return 0;
}