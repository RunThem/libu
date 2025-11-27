test() {
  u_tree_t(int, int) t = make_tree(N)->ref;

  each(i, 0, N, +1) {
    auto mut = u_tree_at_mut(t, i);

    mut_ptr_neq_nil(mut);
    mut_eq(i, *mut);

    *mut = i * 2;

    mut_eq(i * 2, u_tree_at(t, i));
  }

  mut_eq(N, t->len);

  return 0;
}