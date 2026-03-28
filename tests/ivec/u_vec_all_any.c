/***************************************************************************************************
 * Test: u_vec_all_if / u_vec_all_if_ref / u_vec_any_if / u_vec_any_if_ref
 *
 * Coverage:
 * - all_if on empty vector
 * - all_if when all elements match
 * - all_if when some elements don't match
 * - any_if on empty vector
 * - any_if when at least one matches
 * - any_if when none matches
 **************************************************************************************************/

test() {
  /* int type: all_if on empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    mut_true(u_vec_all_if(v, it > 0));
  }

  /* int type: all_if when all elements match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_true(u_vec_all_if(v, it >= 0));
    mut_true(u_vec_all_if(v, it < N));
  }

  /* int type: all_if when some elements don't match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_false(u_vec_all_if(v, it > N / 2));
    mut_false(u_vec_all_if(v, it % 2 == 0));
  }

  /* int type: all_if when none match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_false(u_vec_all_if(v, it > N * 10));
    mut_false(u_vec_all_if(v, it < 0));
  }

  /* int type: all_if with complex condition */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_true(u_vec_all_if(v, it >= 0 && it < N));
    mut_false(u_vec_all_if(v, it % 3 == 0 && it % 5 == 0));
  }

  /* int type: all_if_ref on empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    mut_true(u_vec_all_if_ref(v, *it > 0));
  }

  /* int type: all_if_ref when all elements match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_true(u_vec_all_if_ref(v, *it >= 0));
    mut_true(u_vec_all_if_ref(v, *it < N));
  }

  /* int type: all_if_ref when some elements don't match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_false(u_vec_all_if_ref(v, *it > N / 2));
  }

  /* int type: any_if on empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    mut_false(u_vec_any_if(v, it > 0));
    mut_false(u_vec_any_if(v, it == 0));
  }

  /* int type: any_if when at least one matches */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_true(u_vec_any_if(v, it == 0));
    mut_true(u_vec_any_if(v, it == N - 1));
    mut_true(u_vec_any_if(v, it == N / 2));
    mut_true(u_vec_any_if(v, it >= 0));
  }

  /* int type: any_if when none match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_false(u_vec_any_if(v, it > N * 10));
    mut_false(u_vec_any_if(v, it < 0));
    mut_false(u_vec_any_if(v, it == N));
  }

  /* int type: any_if with complex condition */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_true(u_vec_any_if(v, it % 100 == 50));
    mut_false(u_vec_any_if(v, it % N == N));
  }

  /* int type: any_if_ref on empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    mut_false(u_vec_any_if_ref(v, *it > 0));
  }

  /* int type: any_if_ref when at least one matches */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_true(u_vec_any_if_ref(v, *it == 0));
    mut_true(u_vec_any_if_ref(v, *it == N - 1));
    mut_true(u_vec_any_if_ref(v, *it >= 0));
  }

  /* int type: any_if_ref when none match */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    mut_false(u_vec_any_if_ref(v, *it > N * 10));
    mut_false(u_vec_any_if_ref(v, *it < 0));
  }

  /* int type: single element vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    mut_true(u_vec_all_if(v, it == 42));
    mut_false(u_vec_all_if(v, it == 0));

    mut_true(u_vec_any_if(v, it == 42));
    mut_false(u_vec_any_if(v, it == 0));
  }

  /* int type: small vector */
  {
    u_vec_t(int) v = make_vec(N_SMALL)->ref;

    mut_true(u_vec_all_if(v, it >= 0 && it < N_SMALL));
    mut_false(u_vec_all_if(v, it > N_SMALL / 2));

    mut_true(u_vec_any_if(v, it == 0));
    mut_true(u_vec_any_if(v, it == N_SMALL - 1));
    mut_false(u_vec_any_if(v, it >= N_SMALL));
  }

  /* int type: after modification */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_each_mut (v, it) {
      if (*it % 2 == 0) *it = -1;
    }

    mut_false(u_vec_all_if(v, it >= 0));
    mut_true(u_vec_any_if(v, it == -1));
    mut_true(u_vec_any_if(v, it >= 0));
  }

  return 0;
}