test() {
  {
    u_vec_t(int) v = u_vec_new(v);

    mut_false(u_vec_is_valid(v, 0));
    mut_false(u_vec_is_valid(v, 1));
  }

  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_true(u_vec_is_valid(v, 0));
    mut_true(u_vec_is_valid(v, 1));
    mut_true(u_vec_is_valid(v, N - 2));
    mut_true(u_vec_is_valid(v, N - 1));

    mut_false(u_vec_is_valid(v, N));
  }

  return 0;
}