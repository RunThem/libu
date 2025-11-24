test() {
  u_vec_t(int) v = make_vec(N)->ref;

  {
    bool flag = false;
    u_vec_try_at(v, 0) {
      mut_eq(0, it);
      flag = true;
    }

    mut_true(flag);
  }

  {
    bool flag = false;
    u_vec_try_at(v, 1) {
      mut_eq(1, it);
      flag = true;
    }

    mut_true(flag);
  }

  {
    bool flag = false;
    u_vec_try_at(v, N - 1) {
      mut_eq(N - 1, it);
      flag = true;
    }

    mut_true(flag);
  }

  {
    bool flag = false;
    u_vec_try_at(v, N) {
      mut_eq(N, it);
      flag = true;
    }

    mut_false(flag);
  }

  {
    bool flag = false;
    u_vec_try_at(v, N, num) {
      mut_eq(N, num);
      flag = true;
    }

    mut_false(flag);
  }

  return 0;
}
