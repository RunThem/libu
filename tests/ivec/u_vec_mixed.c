/***************************************************************************************************
 * Test: Mixed operations
 *
 * Coverage:
 * - Insert + remove combinations
 * - Insert + access combinations
 * - Insert + iterate combinations
 * - Clear + reuse combinations
 * - Resize + insert combinations
 * - Complex operation sequences
 **************************************************************************************************/

test() {
  /* int type: insert then remove all */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N, v->len);

    each(i, 0, N, +1) {
      u_vec_remove_back(v);
    }
    mut_eq(0, v->len);
  }

  /* int type: insert front then remove front */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_front(v, i);
    }
    mut_eq(N, v->len);

    each(i, 0, N, +1) {
      auto val = u_vec_remove_front(v);
      mut_eq(N - 1 - i, val);
    }
    mut_eq(0, v->len);
  }

  /* int type: insert at various positions then remove */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert(v, 0, 10);
    u_vec_insert(v, 0, 20);
    u_vec_insert(v, 1, 30);
    u_vec_insert(v, 3, 40);

    mut_eq(4, v->len);
    mut_eq(20, u_vec_at(v, 0));
    mut_eq(30, u_vec_at(v, 1));
    mut_eq(10, u_vec_at(v, 2));
    mut_eq(40, u_vec_at(v, 3));

    u_vec_remove(v, 1);
    mut_eq(3, v->len);
    mut_eq(20, u_vec_at(v, 0));
    mut_eq(10, u_vec_at(v, 1));
    mut_eq(40, u_vec_at(v, 2));
  }

  /* int type: insert, modify, then verify */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_back(v, i);
    }

    u_vec_each_mut (v, it) {
      *it = *it * 2;
    }

    each(i, 0, N, +1) {
      mut_eq(i * 2, u_vec_at(v, i));
    }
  }

  /* int type: insert, clear, then reuse */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N, v->len);

    u_vec_clear(v);
    mut_eq(0, v->len);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_back(v, i * 10);
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      mut_eq(i * 10, u_vec_at(v, i));
    }
  }

  /* int type: resize then insert */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_resize(v, N);
    mut_eq(N, v->cap);
    mut_eq(0, v->len);

    each(i, 0, N, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N, v->len);

    each(i, 0, N, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: insert, resize, then continue insert */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N / 2, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N / 2, v->len);

    i32_t old_cap = v->cap;
    u_vec_resize(v, old_cap * 2);

    each(i, N / 2, N, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N, v->len);

    each(i, 0, N, +1) {
      mut_eq(i, u_vec_at(v, i));
    }
  }

  /* int type: alternating insert and remove */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_back(v, i);
      if (i % 2 == 1) {
        u_vec_remove_back(v);
      }
    }
    mut_eq(N_SMALL / 2, v->len);

    each(i, 0, N_SMALL / 2, +1) {
      mut_eq(i * 2, u_vec_at(v, i));
    }
  }

  /* int type: insert front and back alternately */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      if (i % 2 == 0) {
        u_vec_insert_front(v, i);
      } else {
        u_vec_insert_back(v, i);
      }
    }
    mut_eq(N_SMALL, v->len);
  }

  /* int type: find, modify, find again */
  {
    u_vec_t(int) v = make_vec(N)->ref;

    int* p = u_vec_find_if_mut(v, *it == 100);
    mut_ptr_neq_nil(p);
    *p = -1;

    p = u_vec_find_if_mut(v, *it == 100);
    mut_ptr_eq_nil(p);

    p = u_vec_find_if_mut(v, *it == -1);
    mut_ptr_neq_nil(p);
  }

  /* str_t type: insert then remove all */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      str_t val = u_vec_remove_back(v);
      str_free(val);
    }
    mut_eq(0, v->len);
  }

  /* str_t type: insert, modify, cleanup */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "str_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }

    u_vec_each_mut (v, it) {
      str_free(*it);
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "modified_%d", (int)(it - u_vec_at_mut(v, 0)));
      *it = str_new(buf);
    }

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "modified_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: insert, clear, reuse, cleanup */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "first_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }

    u_vec_clear(v, str_free(it));
    mut_eq(0, v->len);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "second_%d", i);
      u_vec_insert_back(v, str_new(buf));
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "second_%d", i);
      mut_str_eq(buf, u_vec_at(v, i));
    }

    u_vec_cleanup(v, str_free(it));
  }

  /* person_t type: insert, access, cleanup */
  {
    u_vec_t(person_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "person_%d", i);
      u_vec_insert_back(v, person_new(i, buf));
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
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