test() {
  {
    u_dict_t(int, int) t = u_dict_new(int, int);

    mut_false(u_dict_is_valid(t, 0));
    mut_false(u_dict_is_valid(t, 1));
  }

  {
    u_dict_t(int, int) t = make_dict(N)->ref;

    mut_true(u_dict_is_valid(t, 0));
    mut_true(u_dict_is_valid(t, 1));
    mut_true(u_dict_is_valid(t, N - 2));
    mut_true(u_dict_is_valid(t, N - 1));

    mut_false(u_dict_is_valid(t, N));
  }
}
