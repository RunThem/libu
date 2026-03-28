/***************************************************************************************************
 * Test: u_vec_resize
 *
 * Coverage:
 * - Resize empty vector
 * - Resize vector with elements
 * - Resize to larger capacity
 * - Resize preserves existing elements
 * - Resize for int type
 * - Resize for str_t type
 **************************************************************************************************/

test() {
  /* int type: resize empty vector */
  {
    u_vec_t(int) v = u_vec_new(v);
    mut_eq(0, v->len);
    mut_eq(16, v->cap);

    u_vec_resize(v, 32);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
    mut_eq(32, v->cap);
  }

  /* int type: resize preserves elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    mut_eq(N, v->len);

    i32_t old_cap = v->cap;
    u_vec_resize(v, old_cap * 2);
    mut_eq(N, v->len);
    mut_eq(old_cap * 2, v->cap);

    each(i, 0, N, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: resize multiple times */
  {
    u_vec_t(int) v = u_vec_new(v);
    mut_eq(16, v->cap);

    u_vec_resize(v, 32);
    mut_eq(32, v->cap);

    u_vec_resize(v, 64);
    mut_eq(64, v->cap);

    u_vec_resize(v, 128);
    mut_eq(128, v->cap);

    u_vec_resize(v, 256);
    mut_eq(256, v->cap);
  }

  /* int type: resize with large capacity */
  {
    u_vec_t(int) v = u_vec_new(v);
    mut_eq(16, v->cap);

    u_vec_resize(v, N_LARGE);
    mut_eq(N_LARGE, v->cap);
    mut_eq(0, v->len);
  }

  /* int type: resize after inserting elements */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N, v->len);

    i32_t old_cap = v->cap;
    u_vec_resize(v, old_cap * 4);
    mut_eq(N, v->len);
    mut_eq(old_cap * 4, v->cap);

    each(i, 0, N, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* str_t type: resize empty vector */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    mut_eq(0, v->len);
    mut_eq(16, v->cap);

    u_vec_resize(v, 64);
    mut_ptr_neq_nil(v);
    mut_eq(0, v->len);
    mut_eq(64, v->cap);
  }

  /* str_t type: resize preserves string elements */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;
    mut_eq(N, v->len);

    i32_t old_cap = v->cap;
    u_vec_resize(v, old_cap * 2);
    mut_eq(N, v->len);
    mut_eq(old_cap * 2, v->cap);

    each(i, 0, N, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: resize after inserting */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "test_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }
    mut_eq(N_SMALL, v->len);

    i32_t old_cap = v->cap;
    u_vec_resize(v, old_cap * 2);
    mut_eq(N_SMALL, v->len);
    mut_eq(old_cap * 2, v->cap);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "test_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}