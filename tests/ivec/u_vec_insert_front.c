/***************************************************************************************************
 * Test: u_vec_insert_front
 *
 * Coverage:
 * - Insert at front of empty vector
 * - Insert at front multiple times
 * - Insert at front for int type
 * - Insert at front for str_t type
 * - Verify order after multiple front inserts
 **************************************************************************************************/

test() {
  /* int type: insert front into empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    mut_eq(0, v->len);

    u_vec_insert_front(v, 42);
    mut_eq(1, v->len);
    mut_eq(42, u_vec_at(v, 0));
  }

  /* int type: insert front multiple times */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_front(v, i);
    }
    mut_eq(N, v->len);

    each(i, 0, N, +1) {
      mut_eq(N - 1 - i, u_vec_at(v, i));
    }
  }

  /* int type: insert front preserves reverse order */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert_front(v, 1);
    u_vec_insert_front(v, 2);
    u_vec_insert_front(v, 3);

    mut_eq(3, v->len);
    mut_eq(3, u_vec_at(v, 0));
    mut_eq(2, u_vec_at(v, 1));
    mut_eq(1, u_vec_at(v, 2));
  }

  /* int type: insert front on existing vector */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_insert_front(v, -1);
    mut_eq(N + 1, v->len);
    mut_eq(-1, u_vec_at(v, 0));

    each(i, 1, N + 1, +1) {
      mut_eq(i - 1, u_vec_at(v, i));
    }
  }

  /* int type: insert front small vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_front(v, i);
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      mut_eq(N_SMALL - 1 - i, u_vec_at(v, i));
    }
  }

  /* int type: insert front then verify all */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, 100, +1) {
      u_vec_insert_front(v, i);
    }

    i32_t sum = 0;
    u_vec_each (v, it) {
      sum += it;
    }
    mut_eq(4950, sum);
  }

  /* str_t type: insert front into empty vector */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    u_vec_insert_front(v, str_new("first"));
    mut_eq(1, v->len);
    mut_str_eq("first", u_vec_at(v, 0));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert front multiple times */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert_front(v, str_new(buf));
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", N_SMALL - 1 - i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert front on existing vector */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    u_vec_insert_front(v, str_new("new_front"));
    mut_eq(N + 1, v->len);
    mut_str_eq("new_front", u_vec_at(v, 0));
    mut_str_eq("str_0", u_vec_at(v, 1));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert front preserves reverse order */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    u_vec_insert_front(v, str_new("a"));
    u_vec_insert_front(v, str_new("b"));
    u_vec_insert_front(v, str_new("c"));

    mut_eq(3, v->len);
    mut_str_eq("c", u_vec_at(v, 0));
    mut_str_eq("b", u_vec_at(v, 1));
    mut_str_eq("a", u_vec_at(v, 2));

    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}