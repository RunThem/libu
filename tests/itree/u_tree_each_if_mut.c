test() {
  {
    u_tree_t(int, int) t = u_tree_new(t, fn_cmp(int));

    bool flag = true;
    u_tree_each_if_mut(t, it, it->key % 2 == 0) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    bool flag = true;
    int i     = 0;
    int cnt   = 0;

    u_tree_each_if_mut(t, it, it->key % 2 == 0) {
      mut_ptr_neq_nil(it);

      flag = false;
      mut_eq(i, it->key);
      mut_eq(i, it->val);

      i += 2;
      cnt++;

      // it->key = 0;
      it->val = 0;
    }

    mut_false(flag);
    mut_eq(i, N);
    mut_eq(cnt, N / 2);

    i   = 0;
    cnt = 0;

    u_tree_each_if_mut(t, it, i++ % 2 == 0) {
      mut_ptr_neq_nil(it);
      cnt++;

      mut_eq(0, it->val);
    }

    mut_eq(cnt, N / 2);

    i = 0;

    u_tree_each_if_mut(t, it, i % 2 != 0) {
      mut_ptr_neq_nil(it);

      cnt++;

      mut_eq(i, it->val);

      i++;
    }

    mut_eq(cnt, N / 2);
  }

  return 0;
}
