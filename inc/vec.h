#pragma once

#include "u.h"

#ifndef U_VEC_ITEMS_CAP
#  define U_VEC_ITEMS_CAP 16
#endif

typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
} vec_t;

#define vec(T)                                                                                     \
  struct {                                                                                         \
    vec_t _;                                                                                       \
    T it;                                                                                          \
  }

#define vec_isinit(vec) ((vec)->_.itsize != 0)
#define vec_itsize(vec) ((vec)->_.itsize)
#define vec_len(vec)    ((vec)->_.len)
#define vec_cap(vec)    ((vec)->_.cap)
#define vec_items(vec)  (as((vec)->_.items, typeof((vec)->it)*))
#define vec_empty(vec)  ((vec)->_.len == 0)
#define vec_clear(vec)  ((vec)->_.len = 0)

/* private macro */
#define ____vec_bzero(vec) bzero(&(vec)->it, vec_itsize(vec))

ret_t __vec_init(any_t _self, size_t itsize, size_t cap);
#define vec_init(vec, arg...) __vec_init(vec, sizeof((vec)->it), va_0th(U_VEC_ITEMS_CAP, arg))

ret_t __vec_resize(any_t _self, size_t cap);
#define vec_resize(vec, cap) __vec_resize(vec, cap)

ret_t __vec_push(any_t _self, size_t idx, any_t it);
#define vec_push(vec, idx, _it)                                                                    \
  __vec_push(vec, idx, (____vec_bzero(vec), (vec)->it = (_it), &(vec)->it))
#define vec_push_f(vec, _it) __vec_push(vec, 0, (____vec_bzero(vec), (vec)->it = (_it), &(vec)->it))
#define vec_push_b(vec, _it)                                                                       \
  __vec_push(vec, vec_len(vec), (____vec_bzero(vec), (vec)->it = (_it), &(vec)->it))

ret_t __vec_pop(any_t _self, size_t idx, any_t it);
#define vec_pop(vec, idx) __vec_pop(vec, idx, (____vec_bzero(vec), &(vec)->it))
#define vec_pop_f(vec)    __vec_pop(vec, 0, (____vec_bzero(vec), &(vec)->it))
#define vec_pop_b(vec)    __vec_pop(vec, vec_len(vec) - 1, (____vec_bzero(vec), &(vec)->it))

ret_t __vec_at(any_t _self, size_t idx, any_t it);
#define vec_at(vec, idx) (__vec_at(vec, idx, (____vec_bzero(vec), &(vec)->it)), (vec)->it)
#define vec_at_f(vec)    (__vec_at(vec, 0, (____vec_bzero(vec), &(vec)->it)), (vec)->it)
#define vec_at_b(vec)    (__vec_at(vec, vec_len(vec) - 1, (____vec_bzero(vec), &(vec)->it)), (vec)->it)

ret_t __vec_set(any_t _self, size_t idx, any_t it);
#define vec_set(vec, idx, _it)                                                                     \
  __vec_set(vec, idx, (____vec_bzero(vec), (vec)->it = (_it), &(vec)->it))
#define vec_set_f(vec, _it) __vec_set(vec, 0, (____vec_bzero(vec), (vec)->it = (_it), &(vec)->it))
#define vec_set_b(vec, _it)                                                                        \
  __vec_set(vec, vec_len(vec) - 1, (____vec_bzero(vec), (vec)->it = (_it), &(vec)->it))

ret_t __vec_cleanup(any_t _self);
#define vec_cleanup(vec) __vec_cleanup(vec)

vec_t __vec_clone(any_t _self);
#define vec_clone(vec)                                                                             \
  ((struct {                                                                                       \
    vec_t _;                                                                                       \
    typeof((vec)->it) it;                                                                          \
  }){._ = __vec_clone(vec)})

ret_t __vec_erase(any_t _self, size_t idx);
#define vec_erase(vec, idx) __vec_erase(vec, idx)

ssize_t __vec_find(any_t _self, any_t it, eq_fn fn);
#define vec_find(vec, _it, fn)                                                                     \
  __vec_find(vec, (____vec_bzero(vec), (vec)->it = (_it), &(vec)->it), fn)

ssize_t __vec_sort(any_t _self, cmp_fn fn);
#define vec_sort(vec, fn) __vec_sort(vec, fn)

ssize_t __vec_min(any_t _self, cmp_fn fn);
#define vec_min(vec, fn) __vec_min(vec, fn)

ssize_t __vec_max(any_t _self, cmp_fn fn);
#define vec_max(vec, fn) __vec_max(vec, fn)

bool __vec_cmp(any_t _self, any_t _cmp);
#define vec_cmp(vec1, vec2) __vec_cmp(vec1, vec2)

#define vec_for(vec, i) for (size_t i = 0; (i) < vec_len(vec); (i)++)
