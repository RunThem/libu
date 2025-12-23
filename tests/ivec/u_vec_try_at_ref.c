test() {
  u_vec_t(int) v = make_vec(N)->ref;

  {
    bool flag = false;
    u_vec_try_at_ref (v, 0) {
      mut_ptr_neq_nil(it);
      mut_eq(0, *it);
      flag = true;
    }

    mut_true(flag);
  }

  {
    bool flag = false;
    u_vec_try_at_ref (v, 1) {
      mut_ptr_neq_nil(it);
      mut_eq(1, *it);
      flag = true;
    }

    mut_true(flag);
  }

  {
    bool flag = false;
    u_vec_try_at_ref (v, N - 1) {
      mut_ptr_neq_nil(it);
      mut_eq(N - 1, *it);
      flag = true;
    }

    mut_true(flag);
  }

  {
    bool flag = false;
    u_vec_try_at_ref (v, N) {
      mut_ptr_neq_nil(it);
      mut_eq(N, *it);
      flag = true;
    }

    mut_false(flag);
  }

  {
    bool flag = false;
    u_vec_try_at_ref (v, N, ptr) {
      mut_ptr_neq_nil(ptr);
      mut_eq(N, *ptr);
      flag = true;
    }

    mut_false(flag);
  }

  return 0;
}
