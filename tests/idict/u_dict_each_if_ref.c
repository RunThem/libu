test() {
  {
    u_dict_t(int, int) t = u_dict_new(int, int);

    bool flag = true;
    u_dict_each_if_ref(t, it, it->key % 2 == 0) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    bool flag = true;
    int cnt   = 0;

    u_dict_each_if_ref(t, it, it->key % 2 == 0) {
      flag = false;

      mut_eq(it->key, it->val);

      cnt++;
    }

    mut_false(flag);
    mut_eq(cnt, N / 2);
  }

  return 0;
}
