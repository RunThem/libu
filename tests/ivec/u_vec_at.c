/***************************************************************************************************
 * Test: u_vec_at
 *
 * Coverage:
 * - Access elements at various indices
 * - Modify elements via u_vec_at
 * - Access first/last element
 * - Access middle elements
 * - Access for int type
 * - Access for str_t type
 * - Access for person_t type
 **************************************************************************************************/

test() {
  /* int type: access all elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: access and verify order */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t prev = -1;
    u_vec_each (v, it) {
      mut_true(it > prev);
      prev = it;
    }
  }

  /* int type: modify elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      auto val = u_vec_at(v, i);
      u_vec_at(v, i, val * 2);
    }

    each(i, 0, N, +1) {
      mut_eq(i * 2, u_vec_at(v, i));
    }
  }

  /* int type: access first element */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    mut_eq(0, u_vec_at(v, 0));
  }

  /* int type: access last element */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    mut_eq(N - 1, u_vec_at(v, v->len - 1));
  }

  /* int type: access middle element */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    mut_eq(N / 2, u_vec_at(v, N / 2));
  }

  /* int type: access single element vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    mut_eq(42, u_vec_at(v, 0));
    mut_eq(1, v->len);
  }

  /* str_t type: access all elements */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    each(i, 0, N, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: modify elements */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    each(i, 0, N_SMALL, +1) {
      str_free(u_vec_at(v, i));
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "modified_%d", i);
      u_vec_at(v, i, str_new(buf));
    }

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "modified_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: access first/last */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    mut_str_eq("str_0", u_vec_at(v, 0));

    char buf[32] = {};
    snprintf(buf, sizeof(buf), "str_%d", N - 1);
    mut_str_eq(buf, u_vec_at(v, v->len - 1));

    u_vec_cleanup(v, str_free(it));
  }

  /* person_t type: access all elements */
  {
    u_vec_t(person_t) v = make_person_vec(N)->ref;

    each(i, 0, N, +1) {
      person_t p = u_vec_at(v, i);
      mut_eq(i, p.id);

      char buf[32] = {};
      snprintf(buf, sizeof(buf), "person_%d", i);
      mut_str_eq(buf, p.name);
    }

    u_vec_cleanup(v, person_free(&it));
  }

  /* person_t type: access and verify id order */
  {
    u_vec_t(person_t) v = make_person_vec(N)->ref;

    i32_t prev_id = -1;
    u_vec_each (v, it) {
      mut_true(it.id > prev_id);
      prev_id = it.id;
    }

    u_vec_cleanup(v, person_free(&it));
  }

  /* person_t type: access first/last */
  {
    u_vec_t(person_t) v = make_person_vec(N)->ref;

    person_t first = u_vec_at(v, 0);
    mut_eq(0, first.id);
    mut_str_eq("person_0", first.name);

    person_t last = u_vec_at(v, v->len - 1);
    mut_eq(N - 1, last.id);

    char buf[32] = {};
    snprintf(buf, sizeof(buf), "person_%d", N - 1);
    mut_str_eq(buf, last.name);

    u_vec_cleanup(v, person_free(&it));
  }

  return 0;
}