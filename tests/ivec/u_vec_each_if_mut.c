/***************************************************************************************************
 * Test: u_vec_each_if_mut
 *
 * Coverage:
 * - Iterate with condition on empty vector (mutable pointer)
 * - Iterate with condition on single element (mutable pointer)
 * - Iterate with condition filtering elements (mutable pointer)
 * - Modify filtered elements during iteration
 **************************************************************************************************/

test() {
  /* int type: iterate empty vector with condition */
  {
    u_vec_t(int) v = u_vec_new(v);

    bool iterated = false;
    u_vec_each_if_mut (v, it, *it > 0) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* int type: iterate single element matching condition */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    i32_t count = 0;
    u_vec_each_if_mut (v, it, *it == 42) {
      count++;
      mut_ptr_neq_nil(it);
      mut_eq(42, *it);
    }
    mut_eq(1, count);
  }

  /* int type: iterate single element not matching condition */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 42);

    bool iterated = false;
    u_vec_each_if_mut (v, it, *it == 0) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* int type: filter even numbers */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each_if_mut (v, it, *it % 2 == 0) {
      mut_true(*it % 2 == 0);
      count++;
    }
    mut_eq(N / 2, count);
  }

  /* int type: filter odd numbers */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each_if_mut (v, it, *it % 2 == 1) {
      mut_true(*it % 2 == 1);
      count++;
    }
    mut_eq(N / 2, count);
  }

  /* int type: modify filtered elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_each_if_mut (v, it, *it % 2 == 0) {
      *it = -1;
    }

    each(i, 0, N, +1) {
      if (i % 2 == 0) {
        mut_eq(-1, u_vec_at(v, i));
      } else {
        mut_eq(i, u_vec_at(v, i));
      }
    }
  }

  /* int type: modify filtered elements with calculation */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_each_if_mut (v, it, *it > N / 2) {
      *it = *it * 2;
    }

    each(i, 0, N, +1) {
      if (i > N / 2) {
        mut_eq(i * 2, u_vec_at(v, i));
      } else {
        mut_eq(i, u_vec_at(v, i));
      }
    }
  }

  /* int type: filter range and modify */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    u_vec_each_if_mut (v, it, *it >= 100 && *it < 200) {
      *it = 0;
    }

    each(i, 0, N, +1) {
      if (i >= 100 && i < 200) {
        mut_eq(0, u_vec_at(v, i));
      } else {
        mut_eq(i, u_vec_at(v, i));
      }
    }
  }

  /* int type: filter with sum */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t sum = 0;
    u_vec_each_if_mut (v, it, *it % 2 == 0) {
      sum += *it;
    }
    mut_eq((N / 2 - 1) * (N / 2), sum);
  }

  /* int type: filter with break */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each_if_mut (v, it, *it % 2 == 0) {
      count++;
      if (*it == 100) break;
    }
    mut_eq(51, count);
  }

  /* int type: complex condition */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each_if_mut (v, it, (*it % 3 == 0) && (*it % 5 == 0)) {
      mut_true(*it % 15 == 0);
      count++;
    }
    mut_eq(N / 15, count);
  }

  /* int type: filter first N elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each_if_mut (v, it, *it < N_SMALL) {
      mut_true(*it < N_SMALL);
      count++;
    }
    mut_eq(N_SMALL, count);
  }

  /* int type: filter last N elements */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each_if_mut (v, it, *it >= N - N_SMALL) {
      mut_true(*it >= N - N_SMALL);
      count++;
    }
    mut_eq(N_SMALL, count);
  }

  /* int type: no elements match condition */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    bool iterated = false;
    u_vec_each_if_mut (v, it, *it > N * 10) {
      iterated = true;
    }
    mut_false(iterated);
  }

  /* int type: all elements match condition */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    i32_t count = 0;
    u_vec_each_if_mut (v, it, *it >= 0) {
      count++;
    }
    mut_eq(N, count);
  }

  return 0;
}