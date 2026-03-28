/***************************************************************************************************
 * Test: u_vec_clear
 *
 * Coverage:
 * - Clear empty vector
 * - Clear vector with int elements
 * - Clear vector with str_t elements (with cleanup callback)
 * - Clear vector with person_t elements (with cleanup callback)
 * - Verify len becomes 0 but cap unchanged
 * - Clear multiple times
 **************************************************************************************************/

test() {
  /* int type: clear empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
    mut_eq(16, v->cap);

    u_vec_clear(v);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
    mut_eq(16, v->cap);
  }

  /* int type: clear vector with elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    mut_ptr_neq_nil(v);
    mut_eq(N, v->len);
    i32_t old_cap = v->cap;

    u_vec_clear(v);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
    mut_eq(old_cap, v->cap);
  }

  /* int type: clear multiple times */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    i32_t old_cap = v->cap;

    u_vec_clear(v);
    mut_eq(0, v->len);
    mut_eq(old_cap, v->cap);

    u_vec_clear(v);
    mut_eq(0, v->len);
    mut_eq(old_cap, v->cap);

    u_vec_clear(v);
    mut_eq(0, v->len);
    mut_eq(old_cap, v->cap);
  }

  /* str_t type: clear empty vector */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);

    u_vec_clear(v);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
  }

  /* str_t type: clear vector with elements and free callback */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;
    mut_ptr_neq_nil(v);
    mut_eq(N, v->len);
    i32_t old_cap = v->cap;

    u_vec_clear(v, str_free(it));
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
    mut_eq(old_cap, v->cap);
  }

  /* str_t type: clear and reuse */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;
    i32_t old_cap = v->cap;

    u_vec_clear(v, str_free(it));
    mut_eq(0, v->len);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "new_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }
    mut_eq(N_SMALL, v->len);

    u_vec_clear(v, str_free(it));
    mut_eq(0, v->len);
    mut_eq(old_cap, v->cap);

    u_vec_cleanup(v);
  }

  /* person_t type: clear empty vector */
  {
    u_vec_t(person_t) v = u_vec_new(v);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);

    u_vec_clear(v);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
  }

  /* person_t type: clear vector with elements and free callback */
  {
    u_vec_t(person_t) v = make_person_vec(N)->ref;
    mut_ptr_neq_nil(v);
    mut_eq(N, v->len);
    i32_t old_cap = v->cap;

    u_vec_clear(v, person_free(&it));
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
    mut_eq(old_cap, v->cap);
  }

  /* person_t type: clear and reuse */
  {
    u_vec_t(person_t) v = make_person_vec(N)->ref;
    i32_t old_cap = v->cap;

    u_vec_clear(v, person_free(&it));
    mut_eq(0, v->len);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "new_person_%d", i);
      u_vec_insert_back(v, person_new(i, buf));
    }
    mut_eq(N_SMALL, v->len);

    u_vec_clear(v, person_free(&it));
    mut_eq(0, v->len);
    mut_eq(old_cap, v->cap);

    u_vec_cleanup(v);
  }

  return 0;
}