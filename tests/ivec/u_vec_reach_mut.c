/***************************************************************************************************
 * Test: u_vec_reach_mut (reverse iteration with mutable pointer)
 *
 * Coverage:
 * - Reverse iterate empty vector with mutable pointer
 * - Reverse iterate single element with mutable pointer
 * - Reverse iterate full vector with mutable pointer
 * - Modify elements during reverse iteration
 **************************************************************************************************/

test() {
  /* int type: reverse iterate empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    bool iterated = false;
    u_vec_reach_mut (v, it) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* int type: reverse iterate single element */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    i32_t count = 0;
    u_vec_reach_mut (v, it) {
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
    u_vec_reach_mut (v, it) {
      count++;
      mut_ptr_neq_nil(it);
    }
    mut_eq(N, count);
  }

  /* int type: reverse iterate and verify order */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t expected = N - 1;
    u_vec_reach_mut (v, it) {
      mut_eq(expected, *it);
      expected--;
    }
    mut_eq(-1, expected);
  }

  /* int type: modify all elements during reverse iteration */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_reach_mut (v, it) {
      *it = *it * 2;
    }

    each(i, 0, N, +1) {
      mut_eq(i * 2, u_vec_at(v, i));
    }
  }

  /* int type: modify specific elements during reverse iteration */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_reach_mut (v, it) {
      if (*it % 2 == 0) {
        *it = -1;
      }
    }

    each(i, 0, N, +1) {
      if (i % 2 == 0) {
        mut_eq(-1, u_vec_at(v, i));
      } else {
        mut_eq(i, u_vec_at(v, i));
      }
    }
  }

  /* int type: reverse iterate and sum */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t sum = 0;
    u_vec_reach_mut (v, it) {
      sum += *it;
    }
    mut_eq(N * (N - 1) / 2, sum);
  }

  /* int type: reverse iterate and verify pointer addresses */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    int* prev = NULL;
    u_vec_reach_mut (v, it) {
      if (prev) {
        mut_true(it < prev);
      }
      prev = it;
    }
  }

  /* int type: reverse iterate small vector */
  {
    u_vec_t(int) v = make_vec(N_SMALL)->ref;

    i32_t idx = N_SMALL - 1;
    u_vec_reach_mut (v, it) {
      *it += idx;
      idx--;
    }

    each(i, 0, N_SMALL, +1) {
      mut_eq(2 * i, u_vec_at(v, i));
    }
  }

  /* int type: reverse iterate with break */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_reach_mut (v, it) {
      count++;
      if (*it == 100) break;
    }
    mut_eq(N - 100, count);
  }

  /* str_t type: reverse iterate empty vector */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    bool iterated = false;
    u_vec_reach_mut (v, it) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* str_t type: reverse iterate single element */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    u_vec_insert_back(v, str_new("single"));

    i32_t count = 0;
    u_vec_reach_mut (v, it) {
      count++;
      mut_ptr_neq_nil(it);
      mut_ptr_neq_nil(*it);
      mut_str_eq("single", *it);
    }
    mut_eq(1, count);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: modify all elements during reverse iteration */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    i32_t idx = N_SMALL - 1;
    u_vec_reach_mut (v, it) {
      str_free(*it);
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "modified_%d", idx);
      *it = str_new(buf);
      idx--;
    }

    idx = 0;
    u_vec_each (v, it) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "modified_%d", idx);
      mut_str_eq(buf, it);
      idx++;
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: reverse iterate full vector */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    i32_t count = 0;
    u_vec_reach_mut (v, it) {
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
    u_vec_reach_mut (v, it) {
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