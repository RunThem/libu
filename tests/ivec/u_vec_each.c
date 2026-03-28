/***************************************************************************************************
 * Test: u_vec_each
 *
 * Coverage:
 * - Iterate empty vector
 * - Iterate single element vector
 * - Iterate full vector
 * - Iterate and verify order
 * - Iterate and sum elements
 * - Iterate for int type
 * - Iterate for str_t type
 **************************************************************************************************/

test() {
  /* int type: iterate empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    bool iterated = false;
    u_vec_each (v, it) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* int type: iterate single element */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    i32_t count = 0;
    u_vec_each (v, it) {
      count++;
      mut_eq(42, it);
    }
    mut_eq(1, count);
  }

  /* int type: iterate full vector */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each (v, it) {
      count++;
    }
    mut_eq(N, count);
  }

  /* int type: iterate and verify order */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t idx = 0;
    u_vec_each (v, it) {
      mut_eq(idx, it);
      idx++;
    }
    mut_eq(N, idx);
  }

  /* int type: iterate and sum */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t sum = 0;
    u_vec_each (v, it) {
      sum += it;
    }
    mut_eq(N * (N - 1) / 2, sum);
  }

  /* int type: iterate and find max */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t max = -1;
    u_vec_each (v, it) {
      if (it > max) max = it;
    }
    mut_eq(N - 1, max);
  }

  /* int type: iterate and find min */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t min = N;
    u_vec_each (v, it) {
      if (it < min) min = it;
    }
    mut_eq(0, min);
  }

  /* int type: iterate small vector */
  {
    u_vec_t(int) v = make_vec(N_SMALL)->ref;

    i32_t count = 0;
    i32_t sum   = 0;
    u_vec_each (v, it) {
      count++;
      sum += it;
    }
    mut_eq(N_SMALL, count);
    mut_eq(N_SMALL * (N_SMALL - 1) / 2, sum);
  }

  /* int type: iterate with break */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each (v, it) {
      count++;
      if (it == 100) break;
    }
    mut_eq(101, count);
  }

  /* str_t type: iterate empty vector */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    bool iterated = false;
    u_vec_each (v, it) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* str_t type: iterate single element */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    u_vec_insert_back(v, str_new("single"));

    i32_t count = 0;
    u_vec_each (v, it) {
      count++;
      mut_str_eq("single", it);
    }
    mut_eq(1, count);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: iterate full vector */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    i32_t count = 0;
    u_vec_each (v, it) {
      count++;
      mut_ptr_neq_nil(it);
    }
    mut_eq(N, count);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: iterate and verify order */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    i32_t idx = 0;
    u_vec_each (v, it) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", idx);
      mut_str_eq(buf, it);
      idx++;
    }
    mut_eq(N, idx);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: iterate and verify content */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    i32_t count = 0;
    u_vec_each (v, it) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", count);
      mut_str_eq(buf, it);
      count++;
    }
    mut_eq(N_SMALL, count);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: iterate with break */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    i32_t count = 0;
    u_vec_each (v, it) {
      count++;
      if (count == 10) break;
    }
    mut_eq(10, count);

    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}