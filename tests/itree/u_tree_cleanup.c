test() {
  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    u_tree_cleanup(t);
    mut_ptr_eq_nil(t);
  }

  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    int cnt = 0;
    u_tree_cleanup(t, {
      mut_eq(cnt, it.key);

      cnt++;
    });

    mut_eq(N, cnt);
    mut_ptr_eq_nil(t);
  }

  return 0;
}
