/***************************************************************************************************
 * Test: Stress test
 *
 * Coverage:
 * - Large scale insert operations (N_LARGE elements)
 * - Large scale remove operations
 * - Large scale iteration
 * - Large scale access operations
 * - Memory stress with dynamic types
 **************************************************************************************************/

test() {
  /* int type: insert N_LARGE elements */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N_LARGE, v->len);
    mut_true(v->cap >= N_LARGE);

    each(i, 0, N_LARGE, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: insert N_LARGE elements without pre-allocation */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N_LARGE, v->len);
    mut_true(v->cap >= N_LARGE);

    each(i, 0, N_LARGE, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: insert front N_LARGE elements */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_front(v, i);
    }
    mut_eq(N_LARGE, v->len);

    each(i, 0, N_LARGE, +1) {
      mut_eq(N_LARGE - 1 - i, u_vec_at(v, i));
    }
  }

  /* int type: remove all N_LARGE elements from back */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N_LARGE, v->len);

    each(i, 0, N_LARGE, +1) {
      auto val = u_vec_remove_back(v);
      mut_eq(N_LARGE - 1 - i, val);
    }
    mut_eq(0, v->len);
  }

  /* int type: remove all N_LARGE elements from front */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N_LARGE, v->len);

    each(i, 0, N_LARGE, +1) {
      auto val = u_vec_remove_front(v);
      mut_eq(i, val);
    }
    mut_eq(0, v->len);
  }

  /* int type: iterate N_LARGE elements */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }

    i32_t count = 0;
    i32_t sum   = 0;
    u_vec_each (v, it) {
      count++;
      sum += it;
    }
    mut_eq(N_LARGE, count);
    mut_eq(N_LARGE * (N_LARGE - 1) / 2, sum);
  }

  /* int type: reverse iterate N_LARGE elements */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }

    i32_t count = 0;
    i32_t sum   = 0;
    u_vec_reach (v, it) {
      count++;
      sum += it;
    }
    mut_eq(N_LARGE, count);
    mut_eq(N_LARGE * (N_LARGE - 1) / 2, sum);
  }

  /* int type: modify all N_LARGE elements */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }

    u_vec_each_mut (v, it) {
      *it = *it * 2;
    }

    each(i, 0, N_LARGE, +1) {
      mut_eq(i * 2, u_vec_at(v, i));
    }
  }

  /* int type: filter N_LARGE elements */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }

    i32_t count = 0;
    u_vec_each_if (v, it, it % 2 == 0) {
      count++;
    }
    mut_eq(N_LARGE / 2, count);
  }

  /* int type: find in N_LARGE elements */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }

    auto result = u_vec_find_if(v, it == N_LARGE / 2);
    mut_eq(N_LARGE / 2, result);
  }

  /* int type: all_if / any_if on N_LARGE elements */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
    }

    mut_true(u_vec_all_if(v, it >= 0 && it < N_LARGE));
    mut_true(u_vec_any_if(v, it == 0));
    mut_true(u_vec_any_if(v, it == N_LARGE - 1));
    mut_false(u_vec_any_if(v, it >= N_LARGE));
  }

  /* int type: insert and remove alternately */
  {
    u_vec_t(int) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      u_vec_insert_back(v, i);
      if (i % 100 == 99) {
        u_vec_remove_back(v);
      }
    }
    mut_eq(N_LARGE - N_LARGE / 100, v->len);
  }

  /* str_t type: insert N_LARGE strings */
  {
    u_vec_t(str_t) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }
    mut_eq(N_LARGE, v->len);

    each(i, 0, N_LARGE, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: iterate N_LARGE strings */
  {
    u_vec_t(str_t) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }

    i32_t count = 0;
    u_vec_each (v, it) {
      count++;
      mut_ptr_neq_nil(it);
    }
    mut_eq(N_LARGE, count);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: remove all N_LARGE strings */
  {
    u_vec_t(str_t) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }
    mut_eq(N_LARGE, v->len);

    each(i, 0, N_LARGE, +1) {
      str_t val = u_vec_remove_back(v);
      str_free(val);
    }
    mut_eq(0, v->len);
  }

  /* str_t type: find in N_LARGE strings */
  {
    u_vec_t(str_t) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }

    char target[32] = {};
    snprintf(target, sizeof(target), "str_%d", N_LARGE / 2);
    str_t result = u_vec_find_if(v, strcmp(it, target) == 0);
    mut_str_eq(target, result);

    u_vec_cleanup(v, str_free(it));
  }

  /* person_t type: insert N_LARGE persons */
  {
    u_vec_t(person_t) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "person_%d", i);
      u_vec_insert_back(v, person_new(i, buf));
    }
    mut_eq(N_LARGE, v->len);

    each(i, 0, N_LARGE, +1) {
      person_t p = u_vec_at(v, i);
      mut_eq(i, p.id);
    }

    u_vec_cleanup(v, person_free(&it));
  }

  /* person_t type: iterate N_LARGE persons */
  {
    u_vec_t(person_t) v = u_vec_new(v, N_LARGE);

    each(i, 0, N_LARGE, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "person_%d", i);
      u_vec_insert_back(v, person_new(i, buf));
    }

    i32_t count = 0;
    u_vec_each (v, it) {
      count++;
    }
    mut_eq(N_LARGE, count);

    u_vec_cleanup(v, person_free(&it));
  }

  return 0;
}