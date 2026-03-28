/***************************************************************************************************
 * Test: u_vec_insert_back
 *
 * Coverage:
 * - Insert at back of empty vector
 * - Insert at back multiple times
 * - Insert at back for int type
 * - Insert at back for str_t type
 * - Insert at back for person_t type
 * - Verify order after multiple back inserts
 **************************************************************************************************/

test() {
  /* int type: insert back into empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    mut_eq(0, v->len);

    u_vec_insert_back(v, 42);
    mut_eq(1, v->len);
    mut_eq(42, u_vec_at(v, 0));
  }

  /* int type: insert back multiple times */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_back(v, i);
      mut_eq(i + 1, v->len);
    }

    each(i, 0, N, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: insert back preserves order */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert_back(v, 1);
    u_vec_insert_back(v, 2);
    u_vec_insert_back(v, 3);

    mut_eq(3, v->len);
    mut_eq(1, u_vec_at(v, 0));
    mut_eq(2, u_vec_at(v, 1));
    mut_eq(3, u_vec_at(v, 2));
  }

  /* int type: insert back on existing vector */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_insert_back(v, -1);
    mut_eq(N + 1, v->len);
    mut_eq(-1, u_vec_at(v, N));
  }

  /* int type: insert back small vector */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: insert back and verify sum */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, 100, +1) {
      u_vec_insert_back(v, i);
    }

    i32_t sum = 0;
    u_vec_each (v, it) {
      sum += it;
    }
    mut_eq(4950, sum);
  }

  /* int type: insert back triggers capacity expansion */
  {
    u_vec_t(int) v = u_vec_new(v);
    i32_t initial_cap = v->cap;

    each(i, 0, initial_cap + 10, +1) {
      u_vec_insert_back(v, i);
    }

    mut_true(v->cap > initial_cap);
    mut_eq(initial_cap + 10, v->len);
  }

  /* str_t type: insert back into empty vector */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    u_vec_insert_back(v, str_new("first"));
    mut_eq(1, v->len);
    mut_str_eq("first", u_vec_at(v, 0));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert back multiple times */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    each(i, 0, N, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }
    mut_eq(N, v->len);

    each(i, 0, N, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert back on existing vector */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    u_vec_insert_back(v, str_new("new_back"));
    mut_eq(N + 1, v->len);
    mut_str_eq("new_back", u_vec_at(v, N));

    u_vec_cleanup(v, str_free(it));
  }

  /* person_t type: insert back into empty vector */
  {
    u_vec_t(person_t) v = u_vec_new(v);

    u_vec_insert_back(v, person_new(1, "first"));
    mut_eq(1, v->len);

    person_t p = u_vec_at(v, 0);
    mut_eq(1, p.id);
    mut_str_eq("first", p.name);

    u_vec_cleanup(v, person_free(&it));
  }

  /* person_t type: insert back multiple times */
  {
    u_vec_t(person_t) v = u_vec_new(v);

    each(i, 0, N, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "person_%d", i);
      u_vec_insert_back(v, person_new(i, buf));
    }
    mut_eq(N, v->len);

    each(i, 0, N, +1) {
      person_t p = u_vec_at(v, i);
      mut_eq(i, p.id);

      char buf[32] = {};
      snprintf(buf, sizeof(buf), "person_%d", i);
      mut_str_eq(buf, p.name);
    }

    u_vec_cleanup(v, person_free(&it));
  }

  return 0;
}