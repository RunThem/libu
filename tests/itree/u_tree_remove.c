test() {
  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    each(i, 0, N, +1) {
      auto it = u_tree_remove(t, i);

      mut_eq(i, it);
    }

    mut_eq(0, t->len);
  }

  return 0;
}
