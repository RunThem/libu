/***************************************************************************************************
 * Test: u_vec_reach_ref (reverse iteration with const pointer)
 *
 * Coverage:
 * - Reverse iterate empty vector with pointer
 * - Reverse iterate single element with pointer
 * - Reverse iterate full vector with pointer
 * - Reverse iterate and verify order via pointer
 **************************************************************************************************/

test() {
  /* int type: reverse iterate empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    bool iterated = false;
    u_vec_reach_ref (v, it) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* int type: reverse iterate single element */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    i32_t count = 0;
    u_vec_reach_ref (v, it) {
      count++;
      mut_ptr_neq_nil(it);
      mut_eq(42, *it);
    }
    mut_eq(1, count);
  }

  /* int type: reverse iterate full vector */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_reach_ref (v, it) {
      count++;
      mut_ptr_neq_nil(it);
    }
    mut_eq(N, count);
  }

  /* int type: reverse iterate and verify order */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t expected = N - 1;
    u_vec_reach_ref (v, it) {
      mut_eq(expected, *it);
      expected--;
    }
    mut_eq(-1, expected);
  }

  /* int type: reverse iterate and sum */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t sum = 0;
    u_vec_reach_ref (v, it) {
      sum += *it;
    }
    mut_eq(N * (N - 1) / 2, sum);
  }

  /* int type: reverse iterate and verify pointer addresses */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    const int* prev = NULL;
    u_vec_reach_ref (v, it) {
      if (prev) {
        mut_true(it < prev);
      }
      prev = it;
    }
  }

  /* int type: reverse iterate small vector */
  {
    u_vec_t(int) v = make_vec(N_SMALL)->ref;

    i32_t count = 0;
    i32_t sum   = 0;
    u_vec_reach_ref (v, it) {
      count++;
      sum += *it;
    }
    mut_eq(N_SMALL, count);
    mut_eq(N_SMALL * (N_SMALL - 1) / 2, sum);
  }

  /* int type: reverse iterate with break */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_reach_ref (v, it) {
      count++;
      if (*it == 100) break;
    }
    mut_eq(N - 100, count);
  }

  /* int type: reverse iterate verify descending order */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t prev = N;
    u_vec_reach_ref (v, it) {
      mut_true(*it < prev);
      prev = *it;
    }
  }

  /* str_t type: reverse iterate empty vector */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    bool iterated = false;
    u_vec_reach_ref (v, it) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* str_t type: reverse iterate single element */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    u_vec_insert_back(v, str_new("single"));

    i32_t count = 0;
    u_vec_reach_ref (v, it) {
      count++;
      mut_ptr_neq_nil(it);
      mut_ptr_neq_nil(*it);
      mut_str_eq("single", *it);
    }
    mut_eq(1, count);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: reverse iterate full vector */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    i32_t count = 0;
    u_vec_reach_ref (v, it) {
      count++;
      mut_ptr_neq_nil(it);
      mut_ptr_neq_nil(*it);
    }
    mut_eq(N, count);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: reverse iterate and verify order */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    i32_t expected = N - 1;
    u_vec_reach_ref (v, it) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", expected);
      mut_str_eq(buf, *it);
      expected--;
    }
    mut_eq(-1, expected);

    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}