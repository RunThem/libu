test() {
  {
    u_dict_t(int, int) t = u_dict_new(t);

    bool flag = true;

    u_dict_each_ref (t, it) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    bool flag = true;
    int i     = 0;

    u_dict_each_ref (t, it) {
      flag = false;
      mut_eq(it->key, it->val);

      i++;
    }

    mut_false(flag);
    mut_eq(i, N);
  }

  return 0;
}
