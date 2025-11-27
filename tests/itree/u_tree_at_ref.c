test() {
  u_tree_t(int, int) t = make_tree(N)->ref;

  each(i, 0, N, +1) {
    auto ref = u_tree_at_ref(t, i);

    mut_ptr_neq_nil(ref);

    mut_eq(i, *ref);
  }

  mut_eq(N, t->len);
}
