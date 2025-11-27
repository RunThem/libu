test() {
  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    u_tree_clear(t);
    mut_ptr_neq_nil(t);
    mut_eq(0, t->len);
  }

  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    int cnt = 0;

    u_tree_clear(t, {
      mut_eq(cnt, it.key);

      cnt++;
    });

    mut_eq(N, cnt);
    mut_eq(0, t->len);

    mut_ptr_neq_nil(t);
  }

  return 0;
}
