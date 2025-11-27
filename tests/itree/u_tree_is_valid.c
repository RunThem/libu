test() {
  {
    u_tree_t(int, int) t = u_tree_new(t, fn_cmp(int));

    mut_false(u_tree_is_valid(t, 0));
    mut_false(u_tree_is_valid(t, 1));
  }

  {
    u_tree_t(int, int) t = make_tree(N)->ref;

    mut_true(u_tree_is_valid(t, 0));
    mut_true(u_tree_is_valid(t, 1));
    mut_true(u_tree_is_valid(t, N - 2));
    mut_true(u_tree_is_valid(t, N - 1));

    mut_false(u_tree_is_valid(t, N));
  }
}