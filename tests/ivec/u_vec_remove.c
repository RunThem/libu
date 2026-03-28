/***************************************************************************************************
 * Test: u_vec_remove
 *
 * Coverage:
 * - Remove from beginning
 * - Remove from middle
 * - Remove from end
 * - Remove from single element vector
 * - Remove all elements one by one
 * - Remove for int type
 * - Remove for str_t type
 **************************************************************************************************/

test() {
  /* int type: remove from beginning */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto val = u_vec_remove(v, 0);
    mut_eq(0, val);
    mut_eq(N - 1, v->len);

    each(i, 0, N - 1, +1) {
      mut_eq(i + 1, u_vec_at(v, i));
    }
  }

  /* int type: remove from end */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto val = u_vec_remove(v, v->len - 1);
    mut_eq(N - 1, val);
    mut_eq(N - 1, v->len);

    each(i, 0, N - 1, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: remove from middle */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    auto val = u_vec_remove(v, N / 2);
    mut_eq(N / 2, val);
    mut_eq(N - 1, v->len);

    each(i, 0, N / 2, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
    each(i, N / 2, N - 1, +1) {
      mut_eq(i + 1, u_vec_at(v, i));
    }
  }

  /* int type: remove all from end */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      auto val = u_vec_remove(v, v->len - 1);
      mut_eq(N - 1 - i, val);
    }
    mut_eq(0, v->len);
  }

  /* int type: remove all from beginning */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      auto val = u_vec_remove(v, 0);
      mut_eq(i, val);
    }
    mut_eq(0, v->len);
  }

  /* int type: remove from single element vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    auto val = u_vec_remove(v, 0);
    mut_eq(42, val);
    mut_eq(0, v->len);
  }

  /* int type: remove alternating positions */
  {
    u_vec_t(int) v = make_vec(N_SMALL)->ref;

    each(i, 0, N_SMALL / 2, +1) {
      auto val = u_vec_remove(v, i);
      mut_eq(i * 2, val);
    }
    mut_eq(N_SMALL / 2, v->len);
  }

  /* int type: remove and verify remaining */
  {
    u_vec_t(int) v = make_vec(10)->ref;

    u_vec_remove(v, 5);
    u_vec_remove(v, 3);
    u_vec_remove(v, 1);

    mut_eq(7, v->len);
    mut_eq(0, u_vec_at(v, 0));
    mut_eq(2, u_vec_at(v, 1));
    mut_eq(4, u_vec_at(v, 2));
    mut_eq(6, u_vec_at(v, 3));
    mut_eq(7, u_vec_at(v, 4));
    mut_eq(8, u_vec_at(v, 5));
    mut_eq(9, u_vec_at(v, 6));
  }

  /* str_t type: remove from beginning */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    str_t val = u_vec_remove(v, 0);
    mut_str_eq("str_0", val);
    mut_eq(N - 1, v->len);
    mut_str_eq("str_1", u_vec_at(v, 0));

    str_free(val);
    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: remove from end */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    char buf[32] = {};
    snprintf(buf, sizeof(buf), "str_%d", N - 1);
    str_t val = u_vec_remove(v, v->len - 1);
    mut_str_eq(buf, val);
    mut_eq(N - 1, v->len);

    str_free(val);
    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: remove from middle */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    char buf[32] = {};
    snprintf(buf, sizeof(buf), "str_%d", N / 2);
    str_t val = u_vec_remove(v, N / 2);
    mut_str_eq(buf, val);
    mut_eq(N - 1, v->len);

    str_free(val);
    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: remove all from end */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    each(i, 0, N_SMALL, +1) {
      str_t val = u_vec_remove(v, v->len - 1);
      str_free(val);
    }
    mut_eq(0, v->len);
  }

  /* str_t type: remove all from beginning */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    each(i, 0, N_SMALL, +1) {
      str_t val = u_vec_remove(v, 0);
      str_free(val);
    }
    mut_eq(0, v->len);
  }

  /* str_t type: remove from single element */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    u_vec_insert_back(v, str_new("single"));

    str_t val = u_vec_remove(v, 0);
    mut_str_eq("single", val);
    mut_eq(0, v->len);

    str_free(val);
  }

  return 0;
}