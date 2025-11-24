test() {
  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool flag = true;

    u_vec_reach_if_mut(v, it, *it % 2 == 0) {
      flag = false;
    }

    mut_false(flag);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool flag = true;
    int i     = N - 2;

    u_vec_reach_if_mut(v, it, *it % 2 == 0) {
      flag = false;
      mut_eq(i, *it);

      i -= 2;

      *it = 0;
    }

    mut_false(flag);
    mut_eq(i, -2);

    flag = true;
    i    = N - 1;

    u_vec_reach_if_mut(v, it, i-- % 2 == 0) {
      flag = false;
      mut_eq(0, *it);
    }

    mut_false(flag);
    mut_eq(i, -1);

    flag = true;
    i    = N-1;

    u_vec_reach_if_mut(v, it, i-- % 2 != 0) {
      flag = false;
      mut_eq(i + 1, *it);
    }

    mut_false(flag);
    mut_eq(i, -1);
  }

  return 0;
}