test() {
  {
    u_tree_t(int, int) t = {};
    mut_ptr_eq_nil(t);

    t = u_tree_new(t, fn_cmp(int));
    mut_ptr_neq_nil(t);
    mut_ptr_eq(t, t->ref);

    mut_eq(0, t->len);
  }

  {
    u_tree_t(int, int) t = {};
    mut_ptr_eq_nil(t);

    t = u_tree_new(t, fn_cmp(int));
    mut_ptr_neq_nil(t);
    mut_ptr_eq(t, t->ref);

    mut_eq(0, t->len);
  }

  return 0;
}