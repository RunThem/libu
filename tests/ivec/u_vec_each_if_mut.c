test() {

  {
    u_vec_t(int) v = u_vec_new(v);

    bool flag = true;

    u_vec_each_if_mut (v, it, *it % 2 == 0) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool flag = true;
    int i     = 0;
    int cnt   = 0;

    u_vec_each_if_mut (v, it, *it % 2 == 0) {
      mut_ptr_neq_nil(it);

      flag = false;
      mut_eq(i, *it);

      i += 2;
      cnt++;

      *it = 0;
    }

    mut_false(flag);
    mut_eq(i, N);
    mut_eq(cnt, N / 2);

    i   = 0;
    cnt = 0;

    u_vec_each_if_mut (v, it, i++ % 2 == 0) {
      mut_ptr_neq_nil(it);
      cnt++;

      mut_eq(0, *it);
    }

    mut_eq(cnt, N / 2);

    i = 0;

    u_vec_each_if_mut (v, it, i % 2 != 0) {
      mut_ptr_neq_nil(it);
      cnt++;

      mut_eq(i, *it);
      i++;
    }

    mut_eq(cnt, N / 2);
  }

  return 0;
}