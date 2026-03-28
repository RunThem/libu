/***************************************************************************************************
 * Test: u_vec_at_ref
 *
 * Coverage:
 * - Get const pointer to elements
 * - Access all elements via pointer
 * - Access first/last element pointer
 * - Access for int type
 * - Access for str_t type
 **************************************************************************************************/

test() {
  /* int type: get pointer to all elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      const int* p = u_vec_at_ref(v, i);
      mut_ptr_neq_nil(p);
      mut_eq(i, *p);
    }
  }

  /* int type: verify pointer values match at() */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      int val = u_vec_at(v, i);
      const int* p = u_vec_at_ref(v, i);
      mut_eq(val, *p);
    }
  }

  /* int type: pointer to first element */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    const int* p = u_vec_at_ref(v, 0);
    mut_ptr_neq_nil(p);
    mut_eq(0, *p);
  }

  /* int type: pointer to last element */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    const int* p = u_vec_at_ref(v, v->len - 1);
    mut_ptr_neq_nil(p);
    mut_eq(N - 1, *p);
  }

  /* int type: pointer to middle element */
  {
    u_vec_t(int) v = make_vec(N)->ref;
    const int* p = u_vec_at_ref(v, N / 2);
    mut_ptr_neq_nil(p);
    mut_eq(N / 2, *p);
  }

  /* int type: verify pointers are contiguous */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    const int* p0 = u_vec_at_ref(v, 0);
    const int* p1 = u_vec_at_ref(v, 1);

    mut_true((p1 - p0) == 1);
  }

  /* int type: single element */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 100);

    const int* p = u_vec_at_ref(v, 0);
    mut_ptr_neq_nil(p);
    mut_eq(100, *p);
  }

  /* str_t type: get pointer to all elements */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    each(i, 0, N, +1) {
      const str_t* p = u_vec_at_ref(v, i);
      mut_ptr_neq_nil(p);
      mut_ptr_neq_nil(*p);

      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      mut_str_eq(buf, *p);
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: pointer to first/last */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    const str_t* first = u_vec_at_ref(v, 0);
    mut_ptr_neq_nil(first);
    mut_str_eq("str_0", *first);

    const str_t* last = u_vec_at_ref(v, v->len - 1);
    mut_ptr_neq_nil(last);

    char buf[32] = {};
    snprintf(buf, sizeof(buf), "str_%d", N - 1);
    mut_str_eq(buf, *last);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: verify pointer values match at() */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    each(i, 0, N_SMALL, +1) {
      str_t val = u_vec_at(v, i);
      const str_t* p = u_vec_at_ref(v, i);
      mut_ptr_eq(val, *p);
    }

    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}