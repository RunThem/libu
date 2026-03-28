/***************************************************************************************************
 * Test: u_vec_at_mut
 *
 * Coverage:
 * - Get mutable pointer to elements
 * - Modify elements via pointer
 * - Access first/last element mutable pointer
 * - Access for int type
 * - Access for str_t type
 **************************************************************************************************/

test() {
  /* int type: get mutable pointer to all elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      int* p = u_vec_at_mut(v, i);
      mut_ptr_neq_nil(p);
      mut_eq(i, *p);
    }
  }

  /* int type: modify via pointer */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    each(i, 0, N, +1) {
      int* p = u_vec_at_mut(v, i);
      *p = *p * 10 + i;
    }

    each(i, 0, N, +1) {
      mut_eq(i * 10 + i, u_vec_at(v, i));
    }
  }

  /* int type: modify specific elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    int* first = u_vec_at_mut(v, 0);
    *first = -1;

    int* last = u_vec_at_mut(v, v->len - 1);
    *last = -2;

    int* mid = u_vec_at_mut(v, N / 2);
    *mid = -3;

    mut_eq(-1, u_vec_at(v, 0));
    mut_eq(-3, u_vec_at(v, N / 2));
    mut_eq(-2, u_vec_at(v, N - 1));

    each(i, 1, N / 2, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
    each(i, N / 2 + 1, N - 1, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: pointer arithmetic */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    int* p = u_vec_at_mut(v, 0);
    each(i, 0, N, +1) {
      mut_eq(i, p[i]);
    }
  }

  /* int type: single element */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    int* p = u_vec_at_mut(v, 0);
    mut_ptr_neq_nil(p);
    mut_eq(42, *p);

    *p = 100;
    mut_eq(100, u_vec_at(v, 0));
  }

  /* int type: verify pointers are contiguous */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    int* p0 = u_vec_at_mut(v, 0);
    int* p1 = u_vec_at_mut(v, 1);

    mut_true((p1 - p0) == 1);
  }

  /* str_t type: get mutable pointer to all elements */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    each(i, 0, N, +1) {
      str_t* p = u_vec_at_mut(v, i);
      mut_ptr_neq_nil(p);
      mut_ptr_neq_nil(*p);
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: modify via pointer */
  {
    u_vec_t(str_t) v = make_str_vec(N_SMALL)->ref;

    each(i, 0, N_SMALL, +1) {
      str_t* p = u_vec_at_mut(v, i);
      str_free(*p);
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "mut_%d", i);
      *p = str_new(buf);
    }

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "mut_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: modify first/last */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    str_t* first = u_vec_at_mut(v, 0);
    str_free(*first);
    *first = str_new("first_modified");
    mut_str_eq("first_modified", u_vec_at(v, 0));

    str_t* last = u_vec_at_mut(v, v->len - 1);
    str_free(*last);
    char buf[32] = {};
    snprintf(buf, sizeof(buf), "last_modified_%d", N - 1);
    *last = str_new(buf);
    mut_str_eq(buf, u_vec_at(v, v->len - 1));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: pointer to middle element */
  {
    u_vec_t(str_t) v = make_str_vec(N)->ref;

    str_t* mid = u_vec_at_mut(v, N / 2);
    mut_ptr_neq_nil(mid);
    mut_ptr_neq_nil(*mid);

    char buf[32] = {};
    snprintf(buf, sizeof(buf), "str_%d", N / 2);
    mut_str_eq(buf, *mid);

    u_vec_cleanup(v, str_free(it));
  }

  return 0;
}