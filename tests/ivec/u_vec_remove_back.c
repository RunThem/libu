/***************************************************************************************************
 * Test: u_vec_remove_back
 *
 * Coverage:
 * - Remove back from single element vector
 * - Remove back multiple times
 * - Remove back until empty
 * - Remove back for int type
 * - Remove back for str_t type
 **************************************************************************************************/

test() {
  /* int type: remove back from single element */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    auto val = u_vec_remove_back(v);
    mut_eq(42, val);
    mut_eq(0, v->len);
  }

  /* int type: remove back multiple times */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      auto val = u_vec_remove_back(v);
      mut_eq(N - 1 - i, val);
      mut_eq(N - 1 - i, v->len);
    }
    mut_eq(0, v->len);
  }

  /* int type: remove back preserves order */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 2);
    u_vec_insert_back(v, 3);

    mut_eq(3, u_vec_remove_back(v));
    mut_eq(2, v->len);
    mut_eq(1, u_vec_at(v, 0));
    mut_eq(2, u_vec_at(v, 1));

    mut_eq(2, u_vec_remove_back(v));
    mut_eq(1, v->len);
    mut_eq(1, u_vec_at(v, 0));

    mut_eq(1, u_vec_remove_back(v));
    mut_eq(0, v->len);
  }

  /* int type: remove back small vector */
  {
    u_vec_t(int) v = make_vec(N_SMALL)->ref;

    each(i, 0, N_SMALL, +1) {
      auto val = u_vec_remove_back(v);
      mut_eq(N_SMALL - 1 - i, val);
    }
    mut_eq(0, v->len);
  }

  /* int type: remove back and verify remaining */
  {
    u_vec_t(int) v = make_vec(10)->ref;

    u_vec_remove_back(v);
    mut_eq(9, v->len);

    each(i, 0, 9, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: remove back then insert back */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N / 2, +1) {
      u_vec_remove_back(v);
    }
    mut_eq(N / 2, v->len);

    each(i, 0, N / 2, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N, v->len);
  }

  /* int type: remove back reverse order */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t expected = N - 1;
    each(i, 0, N, +1) {
      auto val = u_vec_remove_back(v);
      mut_eq(expected, val);
      expected--;
    }
  }

  /* str_t type: remove back from single element */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    u_vec_insert_back(v, str_new("single"));

    str_t val = u_vec_remove_back(v);
    mut_str_eq("single", val);
    mut_eq(0, v->len);

    str_free(val);
  }

  /* str_t type: remove back multiple times */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    each(i, 0, N_SMALL, +1) {
      str_t val = u_vec_remove_back(v);
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", N_SMALL - 1 - i);
      mut_str_eq(buf, val);
      str_free(val);
    }
    mut_eq(0, v->len);
  }

  /* str_t type: remove back preserves order */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    u_vec_insert_back(v, str_new("a"));
    u_vec_insert_back(v, str_new("b"));
    u_vec_insert_back(v, str_new("c"));

    str_t val1 = u_vec_remove_back(v);
    mut_str_eq("c", val1);
    mut_eq(2, v->len);
    mut_str_eq("a", u_vec_at(v, 0));
    mut_str_eq("b", u_vec_at(v, 1));

    str_t val2 = u_vec_remove_back(v);
    mut_str_eq("b", val2);
    mut_eq(1, v->len);
    mut_str_eq("a", u_vec_at(v, 0));

    str_t val3 = u_vec_remove_back(v);
    mut_str_eq("a", val3);
    mut_eq(0, v->len);

    str_free(val1);
    str_free(val2);
    str_free(val3);
  }

  /* str_t type: remove back and verify remaining */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    char buf[32] = {};
    snprintf(buf, sizeof(buf), "str_%d", N - 1);
    str_t val = u_vec_remove_back(v);
    mut_str_eq(buf, val);
    mut_eq(N - 1, v->len);

    snprintf(buf, sizeof(buf), "str_%d", N - 2);
    mut_str_eq(buf, u_vec_at(v, v->len - 1));

    str_free(val);
    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}