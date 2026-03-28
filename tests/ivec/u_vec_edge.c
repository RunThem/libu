/***************************************************************************************************
 * Test: Edge cases and boundary conditions
 *
 * Coverage:
 * - Special integer values (INT_MAX, INT_MIN, negative, zero)
 * - Duplicate elements and find behavior
 * - Empty strings and long strings
 * - Capacity boundary (exact expansion trigger)
 * - person_t with NULL name
 **************************************************************************************************/

#include <limits.h>

test() {
  /* int type: INT_MAX values */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert_back(v, INT_MAX);
    u_vec_insert_back(v, INT_MAX - 1);
    u_vec_insert_back(v, INT_MAX);

    mut_eq(3, v->len);
    mut_eq(INT_MAX, u_vec_at(v, 0));
    mut_eq(INT_MAX - 1, u_vec_at(v, 1));
    mut_eq(INT_MAX, u_vec_at(v, 2));
  }

  /* int type: INT_MIN values */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert_back(v, INT_MIN);
    u_vec_insert_back(v, INT_MIN + 1);
    u_vec_insert_back(v, INT_MIN);

    mut_eq(3, v->len);
    mut_eq(INT_MIN, u_vec_at(v, 0));
    mut_eq(INT_MIN + 1, u_vec_at(v, 1));
    mut_eq(INT_MIN, u_vec_at(v, 2));
  }

  /* int type: zero values */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_back(v, 0);
    }
    mut_eq(N_SMALL, v->len);

    u_vec_each (v, it) {
      mut_eq(0, it);
    }
  }

  /* int type: negative values */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_back(v, -i);
    }
    mut_eq(N_SMALL, v->len);

    each(i, 0, N_SMALL, +1) {
      mut_eq(-i, u_vec_at(v, i));
    }
  }

  /* int type: mixed positive and negative */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, -N_SMALL / 2, N_SMALL / 2, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(N_SMALL, v->len);

    mut_eq(-N_SMALL / 2, u_vec_at(v, 0));
    mut_eq(N_SMALL / 2 - 1, u_vec_at(v, N_SMALL - 1));
  }

  /* int type: find duplicate elements - find_if returns first */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 10);
    u_vec_insert_back(v, 20);
    u_vec_insert_back(v, 20);
    u_vec_insert_back(v, 20);
    u_vec_insert_back(v, 30);

    int* p = u_vec_find_if_mut(v, *it == 20);
    mut_ptr_neq_nil(p);
    mut_eq(20, *p);
    mut_eq(20, u_vec_at(v, 1));  /* first occurrence at index 1 */
  }

  /* int type: rfind_if returns last duplicate */
  {
    u_vec_t(int) v = u_vec_new(v);
    u_vec_insert_back(v, 10);
    u_vec_insert_back(v, 20);
    u_vec_insert_back(v, 20);
    u_vec_insert_back(v, 20);
    u_vec_insert_back(v, 30);

    int* p = u_vec_rfind_if_mut(v, *it == 20);
    mut_ptr_neq_nil(p);
    mut_eq(20, *p);
    mut_eq(20, u_vec_at(v, 3));  /* last occurrence at index 3 */
  }

  /* int type: all duplicates */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_back(v, 42);
    }
    mut_eq(N_SMALL, v->len);

    mut_true(u_vec_all_if(v, it == 42));
    mut_true(u_vec_any_if(v, it == 42));
    mut_false(u_vec_any_if(v, it != 42));
  }

  /* int type: find in all duplicates */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_back(v, 100);
    }

    int* p1 = u_vec_find_if_mut(v, *it == 100);
    int* p2 = u_vec_rfind_if_mut(v, *it == 100);

    mut_ptr_neq_nil(p1);
    mut_ptr_neq_nil(p2);
    mut_eq(0, p1 - u_vec_at_mut(v, 0));  /* first at index 0 */
    mut_eq(N_SMALL - 1, p2 - u_vec_at_mut(v, 0));  /* last at index N_SMALL-1 */
  }

  /* str_t type: empty strings */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    u_vec_insert_back(v, str_new(""));
    u_vec_insert_back(v, str_new(""));
    u_vec_insert_back(v, str_new("non-empty"));

    mut_eq(3, v->len);
    mut_str_eq("", u_vec_at(v, 0));
    mut_str_eq("", u_vec_at(v, 1));
    mut_str_eq("non-empty", u_vec_at(v, 2));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: find empty string */
  {
    u_vec_t(str_t) v = u_vec_new(v);
    u_vec_insert_back(v, str_new("a"));
    u_vec_insert_back(v, str_new(""));
    u_vec_insert_back(v, str_new("b"));
    u_vec_insert_back(v, str_new(""));
    u_vec_insert_back(v, str_new("c"));

    str_t* p1 = u_vec_find_if_mut(v, strcmp(*it, "") == 0);
    str_t* p2 = u_vec_rfind_if_mut(v, strcmp(*it, "") == 0);

    mut_ptr_neq_nil(p1);
    mut_ptr_neq_nil(p2);
    mut_str_eq("", *p1);
    mut_str_eq("", *p2);

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: long strings */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    char long_str[256] = {};
    each(i, 0, 200, +1) {
      long_str[i] = 'a' + (i % 26);
    }

    u_vec_insert_back(v, str_new(long_str));
    mut_eq(1, v->len);
    mut_str_eq(long_str, u_vec_at(v, 0));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: very long strings */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    char very_long[1024] = {};
    each(i, 0, 1000, +1) {
      very_long[i] = 'x';
    }

    u_vec_insert_back(v, str_new(very_long));
    mut_eq(1, v->len);
    mut_str_eq(very_long, u_vec_at(v, 0));

    u_vec_cleanup(v, str_free(it));
  }

  /* str_t type: strings with special characters */
  {
    u_vec_t(str_t) v = u_vec_new(v);

    u_vec_insert_back(v, str_new("hello world"));
    u_vec_insert_back(v, str_new("tab\tcharacter"));
    u_vec_insert_back(v, str_new("newline\nchar"));
    u_vec_insert_back(v, str_new("中文测试"));

    mut_eq(4, v->len);
    mut_str_eq("hello world", u_vec_at(v, 0));
    mut_str_eq("tab\tcharacter", u_vec_at(v, 1));
    mut_str_eq("newline\nchar", u_vec_at(v, 2));
    mut_str_eq("中文测试", u_vec_at(v, 3));

    u_vec_cleanup(v, str_free(it));
  }

  /* capacity: exact boundary - fill to capacity */
  {
    u_vec_t(int) v = u_vec_new(v, 16);
    mut_eq(16, v->cap);

    each(i, 0, 16, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(16, v->len);
    mut_eq(16, v->cap);  /* exactly at capacity, no expansion yet */
  }

  /* capacity: trigger expansion at boundary + 1 */
  {
    u_vec_t(int) v = u_vec_new(v, 16);
    mut_eq(16, v->cap);

    each(i, 0, 16, +1) {
      u_vec_insert_back(v, i);
    }
    mut_eq(16, v->cap);

    u_vec_insert_back(v, 16);  /* triggers expansion */
    mut_eq(17, v->len);
    mut_true(v->cap > 16);
  }

  /* capacity: multiple expansions */
  {
    u_vec_t(int) v = u_vec_new(v);
    i32_t initial_cap = v->cap;

    i32_t prev_cap = initial_cap;
    i32_t expansion_count = 0;

    each(i, 0, 100, +1) {
      u_vec_insert_back(v, i);
      if (v->cap > prev_cap) {
        expansion_count++;
        prev_cap = v->cap;
      }
    }
    mut_eq(100, v->len);
    mut_true(expansion_count > 0);
  }

  /* capacity: insert_front triggers expansion */
  {
    u_vec_t(int) v = u_vec_new(v, 16);

    each(i, 0, 20, +1) {
      u_vec_insert_front(v, i);
    }
    mut_eq(20, v->len);
    mut_true(v->cap >= 20);
  }

  /* capacity: insert at middle triggers expansion */
  {
    u_vec_t(int) v = u_vec_new(v, 16);

    each(i, 0, 16, +1) {
      u_vec_insert_back(v, i);
    }

    u_vec_insert(v, 8, -1);  /* insert at middle, triggers expansion */
    mut_eq(17, v->len);
    mut_true(v->cap > 16);
  }

  /* person_t: with NULL name */
  {
    u_vec_t(person_t) v = u_vec_new(v);

    person_t p1 = { .id = 1, .name = NULL };
    person_t p2 = { .id = 2, .name = str_new("valid") };

    u_vec_insert_back(v, p1);
    u_vec_insert_back(v, p2);

    mut_eq(2, v->len);
    mut_eq(1, u_vec_at(v, 0).id);
    mut_ptr_eq_nil(u_vec_at(v, 0).name);
    mut_eq(2, u_vec_at(v, 1).id);
    mut_str_eq("valid", u_vec_at(v, 1).name);

    u_vec_cleanup(v, {
      if (it.name) str_free(it.name);
    });
  }

  /* person_t: find person with NULL name */
  {
    u_vec_t(person_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      person_t p = { .id = i, .name = (i % 3 == 0) ? NULL : str_new("name") };
      u_vec_insert_back(v, p);
    }

    person_t* p = u_vec_find_if_mut(v, it->name == NULL);
    mut_ptr_neq_nil(p);
    mut_eq(0, p->id);  /* first with NULL name at index 0 */

    u_vec_cleanup(v, {
      if (it.name) str_free(it.name);
    });
  }

  /* person_t: all persons with valid names */
  {
    u_vec_t(person_t) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      char buf[32] = {};
      snprintf(buf, sizeof(buf), "person_%d", i);
      u_vec_insert_back(v, person_new(i, buf));
    }

    mut_true(u_vec_all_if(v, it.name != NULL));

    u_vec_cleanup(v, person_free(&it));
  }

  /* person_t: negative id */
  {
    u_vec_t(person_t) v = u_vec_new(v);

    u_vec_insert_back(v, person_new(-100, "negative"));
    u_vec_insert_back(v, person_new(-1, "minus_one"));
    u_vec_insert_back(v, person_new(0, "zero"));
    u_vec_insert_back(v, person_new(INT_MAX, "max"));

    mut_eq(4, v->len);
    mut_eq(-100, u_vec_at(v, 0).id);
    mut_eq(-1, u_vec_at(v, 1).id);
    mut_eq(0, u_vec_at(v, 2).id);
    mut_eq(INT_MAX, u_vec_at(v, 3).id);

    u_vec_cleanup(v, person_free(&it));
  }

  /* mixed: insert special values then modify */
  {
    u_vec_t(int) v = u_vec_new(v);

    u_vec_insert_back(v, INT_MAX);
    u_vec_insert_back(v, 0);
    u_vec_insert_back(v, INT_MIN);

    u_vec_each_mut (v, it) {
      if (*it == INT_MAX) *it = -1;
      if (*it == INT_MIN) *it = 1;
    }

    mut_eq(-1, u_vec_at(v, 0));
    mut_eq(0, u_vec_at(v, 1));
    mut_eq(1, u_vec_at(v, 2));
  }

  /* mixed: find special values after modification */
  {
    u_vec_t(int) v = u_vec_new(v);

    each(i, 0, N_SMALL, +1) {
      u_vec_insert_back(v, i);
    }

    /* modify all to same value */
    u_vec_each_mut (v, it) {
      *it = 42;
    }

    /* find should find first */
    int* p = u_vec_find_if_mut(v, *it == 42);
    mut_ptr_neq_nil(p);
    mut_eq(0, p - u_vec_at_mut(v, 0));
  }

  return 0;
}