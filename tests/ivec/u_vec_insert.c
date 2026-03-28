/***************************************************************************************************
 * Test: u_vec_insert
 *
 * Coverage:
 * - Insert at beginning
 * - Insert at middle
 * - Insert at end (same as insert_back)
 * - Insert into empty vector
 * - Insert multiple elements
 * - Insert for int type
 * - Insert for str_t type
 **************************************************************************************************/

test() {
  /* int type: insert at beginning */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert(v, 0, 10);
    mut_eq(1, v->len);
    mut_eq(10, u_vec_at(v, 0));

    u_vec_insert(v, 0, 20);
    mut_eq(2, v->len);
    mut_eq(20, u_vec_at(v, 0));
    mut_eq(10, u_vec_at(v, 1));

    u_vec_insert(v, 0, 30);
    mut_eq(3, v->len);
    mut_eq(30, u_vec_at(v, 0));
    mut_eq(20, u_vec_at(v, 1));
    mut_eq(10, u_vec_at(v, 2));
  }

  /* int type: insert at middle */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_insert(v, N / 2, -1);
    mut_eq(N + 1, v->len);
    mut_eq(-1, u_vec_at(v, N / 2));

    each(i, 0, N / 2, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
    each(i, N / 2 + 1, N + 1, +1) {
      mut_eq(i - 1, u_vec_at(v, i));
    }
  }

  /* int type: insert at end */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_insert(v, v->len, -1);
    mut_eq(N + 1, v->len);
    mut_eq(-1, u_vec_at(v, N));
  }

  /* int type: insert into empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    mut_eq(0, v->len);

    u_vec_insert(v, 0, 42);
    mut_eq(1, v->len);
    mut_eq(42, u_vec_at(v, 0));
  }

  /* int type: insert multiple at various positions */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert(v, 0, i);
    }
    mut_eq(N, v->len);

    each(i, 0, N, +1) {
      mut_eq(N - 1 - i, u_vec_at(v, i));
    }
  }

  /* int type: insert preserves order */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert(v, 0, 1);
    u_vec_insert(v, 1, 2);
    u_vec_insert(v, 2, 3);
    u_vec_insert(v, 3, 4);

    mut_eq(4, v->len);
    mut_eq(1, u_vec_at(v, 0));
    mut_eq(2, u_vec_at(v, 1));
    mut_eq(3, u_vec_at(v, 2));
    mut_eq(4, u_vec_at(v, 3));
  }

  /* int type: insert at every position */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert(v, i, i);
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* str_t type: insert at beginning */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    u_vec_insert(v, 0, str_new("first"));
    mut_eq(1, v->len);
    mut_str_eq("first", u_vec_at(v, 0));

    u_vec_insert(v, 0, str_new("second"));
    mut_eq(2, v->len);
    mut_str_eq("second", u_vec_at(v, 0));
    mut_str_eq("first", u_vec_at(v, 1));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert at middle */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    u_vec_insert(v, N / 2, str_new("middle"));
    mut_eq(N + 1, v->len);
    mut_str_eq("middle", u_vec_at(v, N / 2));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert at end */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    u_vec_insert(v, v->len, str_new("end"));
    mut_eq(N + 1, v->len);
    mut_str_eq("end", u_vec_at(v, N));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert multiple */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert(v, i, str_new(buf));
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}