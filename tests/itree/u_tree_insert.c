test() {
  {
    u_tree_t(int, int) t = u_tree_new(t, fn_cmp(int));

    each(i, 0, N, +1) {
      u_tree_insert(t, i, i);

      mut_eq(i + 1, t->len);
    }

    each(i, 0, N, +1) {
      auto it = u_tree_at(t, i);

      mut_eq(i, it);
    }

    mut_eq(N, t->len);
  }

  return 0;
}
