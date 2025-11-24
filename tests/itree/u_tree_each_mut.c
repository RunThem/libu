test() {
  {
    u_tree_t(int, int) t = u_tree_new(int, int, fn_cmp(int));

    bool flag = true;

    u_tree_each_mut(t, it) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    bool flag = true;
    int i     = 0;

    u_tree_each_mut(t, it) {
      mut_ptr_neq_nil(it);

      flag = false;
      mut_eq(i, it->key);
      mut_eq(i, it->val);

      i++;

      // it->key = it->key * 2;
      it->val = it->val * 2;
    }

    mut_eq(N, t->len);

    mut_false(flag);
    mut_eq(i, N);

    flag = true;
    i    = 0;

    u_tree_each_mut(t, it) {
      mut_ptr_neq_nil(it);

      flag = false;

      mut_eq(i, it->key);
      mut_eq(i * 2, it->val);

      i++;
    }

    mut_eq(N, t->len);
  }

  return 0;
}
