/* clang-format off */
#include "mut.h"
#include "u/u.h"
#include <string.h>

/***************************************************************************************************
 * Test Constants
 **************************************************************************************************/
#undef N
#define N 3000
#define N_SMALL 10
#define N_LARGE 5000

/***************************************************************************************************
 * Dynamic Memory Test Types
 **************************************************************************************************/

// String type for testing dynamic memory
typedef char* str_t;

// Person struct for testing complex types with nested pointers
typedef struct {
  i32_t id;
  str_t name;
} person_t;

/***************************************************************************************************
 * Dynamic Memory Test Helpers
 **************************************************************************************************/

static str_t str_new(const char* s) {
  return strdup(s);
}

static void str_free(str_t s) {
  if (s) free(s);
}

static person_t person_new(i32_t id, const char* name) {
  person_t p = { .id = id, .name = str_new(name) };
  return p;
}

static void person_free(person_t* p) {
  if (p && p->name) {
    free(p->name);
    p->name = NULL;
  }
}

static i32_t person_cmp(const person_t* a, const person_t* b) {
  return a->id - b->id;
}

/***************************************************************************************************
 * Vector Test Helpers
 **************************************************************************************************/

u_vec_t(int) make_vec(i32_t n) {
  u_vec_t(int) v = u_vec_new(v, n);

  each(i, 0, n, +1) {
    u_vec_insert_back(v, i);
  }

  return v->ref;
}

u_vec_t(str_t) make_str_vec(i32_t n) {
  u_vec_t(str_t) v = u_vec_new(v, n);

  each(i, 0, n, +1) {
    char buf[32] = {};
    snprintf(buf, sizeof(buf), "str_%d", i);
    u_vec_insert_back(v, str_new(buf));
  }

  return v->ref;
}

u_vec_t(person_t) make_person_vec(i32_t n) {
  u_vec_t(person_t) v = u_vec_new(v, n);

  each(i, 0, n, +1) {
    char buf[32] = {};
    snprintf(buf, sizeof(buf), "person_%d", i);
    u_vec_insert_back(v, person_new(i, buf));
  }

  return v->ref;
}

/***************************************************************************************************
 * Other Container Test Helpers
 **************************************************************************************************/

u_tree_t(int, int) make_tree(i32_t n) {
  u_tree_t(int, int) t = u_tree_new(t, fn_cmp(int));

  each(i, 0, n, +1) {
    u_tree_insert(t, i, i);
  }

  return t->ref;
}

u_dict_t(int, int) make_dict(i32_t n) {
  u_dict_t(int, int) d = u_dict_new(d);

  each(i, 0, n, +1) {
    u_dict_insert(d, i, i);
  }

  return d->ref;
}
