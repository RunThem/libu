test() {
  {
    u_tree_t(int, int) t = u_tree_new(int, int, fn_cmp(int));

    bool flag = true;

    u_tree_each_if (t, it, it.key % 2 == 0) {
      flag = false;
    }

    mut_true(flag);
  }

  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    bool flag = true;
    int i     = 0;
    int cnt   = 0;

    u_tree_each_if (t, it, it.key % 2 == 0) {
      flag = false;

      mut_eq(i, it.key);

      i += 2;
      cnt++;
    }

    mut_false(flag);
    mut_eq(i, N);
    mut_eq(cnt, N / 2);
  }

  return 0;
}
