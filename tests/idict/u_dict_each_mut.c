test() {
  {
    u_dict_t(int, int) t = u_dict_new(t);

    bool flag = true;

    u_dict_each_mut(t, it) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    bool flag = true;
    int i     = 0;

    u_dict_each_mut(t, it) {
      mut_ptr_neq_nil(it);

      flag = false;
      mut_eq(it->key, it->val);

      i++;

      // it->key = it->key * 2;
      it->val = it->val * 2;
    }

    mut_eq(N, t->len);

    mut_false(flag);
    mut_eq(i, N);

    flag = true;

    u_dict_each_mut(t, it) {
      mut_ptr_neq_nil(it);

      flag = false;

      mut_eq(it->key * 2, it->val);
    }

    mut_eq(N, t->len);
  }

  return 0;
}
