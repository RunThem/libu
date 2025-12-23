test() {

  {
    u_vec_t(int) v = u_vec_new(v);

    bool flag = true;

    u_vec_each_mut (v, it) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool flag = true;
    int i     = 0;

    u_vec_each_mut (v, it) {
      mut_ptr_neq_nil(it);

      flag = false;
      mut_eq(i, *it);

      i++;

      *it = *it * 2;
    }

    mut_eq(N, v->len);

    mut_false(flag);
    mut_eq(i, N);

    flag = true;
    i    = 0;
    u_vec_each_mut (v, it) {
      mut_ptr_neq_nil(it);

      flag = false;
      mut_eq(i * 2, *it);
      i++;
    }

    mut_eq(N, v->len);
  }

  return 0;
}