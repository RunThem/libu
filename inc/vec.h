#pragma once

#include "u.h"

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_VEC_CAP
#  define U_VEC_CAP 16
#endif

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
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
  }*

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __vec_new(size_t itsize, size_t cap);
#define vec_new(T, arg...) __vec_new(sizeof(T), va_0th(U_VEC_CAP, arg))

ret_t __vec_init(any_t* _self, size_t itsize, size_t cap);
#define vec_init(vec, arg...) __vec_init((any_t*)vec, sizeof((*(vec))->it), va_0th(U_VEC_CAP, arg))

vec_t* __vec_clone(any_t _self);
#define vec_clone(vec) ((typeof(vec))__vec_clone(vec))

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
ret_t __vec_resize(any_t _self, size_t cap);
#define vec_resize(vec, cap) __vec_resize(vec, cap)

void __vec_clear(any_t _self);
#define vec_clear(vec) __vec_clear(vec)

void __vec_cleanup(any_t _self);
#define vec_cleanup(vec)                                                                           \
  do {                                                                                             \
    __vec_cleanup(vec);                                                                            \
    (vec) = nullptr;                                                                               \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __vec_itsize(any_t _self);
#define vec_itsize(vec) __vec_itsize(vec)

size_t __vec_len(any_t _self);
#define vec_len(vec) __vec_len(vec)

size_t __vec_cap(any_t _self);
#define vec_cap(vec) __vec_cap(vec)

bool __vec_empty(any_t _self);
#define vec_empty(vec) __vec_empty(vec)

ret_t __vec_erase(any_t _self, size_t idx);
#define vec_erase(vec, idx) __vec_erase(vec, idx)

void __vec_at(any_t _self, size_t idx, any_t it);
void __vec_at_f(any_t _self, any_t it);
void __vec_at_b(any_t _self, any_t it);
#define vec_at(vec, idx) (__vec_at(vec, idx, &(vec)->it), (vec)->it)
#define vec_at_f(vec)    (__vec_at_f(vec, &(vec)->it), (vec)->it)
#define vec_at_b(vec)    (__vec_at_b(vec, &(vec)->it), (vec)->it)

ret_t __vec_set(any_t _self, size_t idx, any_t it);
ret_t __vec_set_f(any_t _self, any_t it);
ret_t __vec_set_b(any_t _self, any_t it);
#define vec_set(vec, idx, _it) __vec_set(vec, idx, ((vec)->it = (_it), &(vec)->it))
#define vec_set_f(vec, _it)    __vec_set_f(vec, ((vec)->it = (_it), &(vec)->it))
#define vec_set_b(vec, _it)    __vec_set_b(vec, ((vec)->it = (_it), &(vec)->it))

void __vec_pop(any_t _self, size_t idx, any_t it);
void __vec_pop_f(any_t _self, any_t it);
void __vec_pop_b(any_t _self, any_t it);
#define vec_pop(vec, idx) (__vec_pop(vec, idx, &(vec)->it), (vec)->it)
#define vec_pop_f(vec)    (__vec_pop_f(vec, &(vec)->it), (vec)->it)
#define vec_pop_b(vec)    (__vec_pop_b(vec, &(vec)->it), (vec)->it)

ret_t __vec_push(any_t _self, size_t idx, any_t it);
ret_t __vec_push_f(any_t _self, any_t it);
ret_t __vec_push_b(any_t _self, any_t it);
#define vec_push(vec, idx, _it) __vec_push(vec, idx, ((vec)->it = (_it), &(vec)->it))
#define vec_push_f(vec, _it)    __vec_push_f(vec, ((vec)->it = (_it), &(vec)->it))
#define vec_push_b(vec, _it)    __vec_push_b(vec, ((vec)->it = (_it), &(vec)->it))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __vec_range(any_t _self, size_t idx, any_t it);
#define vec_for(vec, i)  for (size_t i = 0; __vec_range(vec, i, &(vec)->it); (i)++)
#define vec_rfor(vec, i) for (size_t i = __vec_len(vec) - 1; __vec_range(vec, i, &(vec)->it); (i)--)

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
ssize_t __vec_find(any_t _self, any_t it, eq_fn fn);
#define vec_find(vec, _it, fn) __vec_find(vec, ((vec)->it = (_it), &(vec)->it), fn)

ssize_t __vec_sort(any_t _self, cmp_fn fn);
#define vec_sort(vec, fn) __vec_sort(vec, fn)

ssize_t __vec_min(any_t _self, cmp_fn fn);
#define vec_min(vec, fn) __vec_min(vec, fn)

ssize_t __vec_max(any_t _self, cmp_fn fn);
#define vec_max(vec, fn) __vec_max(vec, fn)

bool __vec_cmp(any_t _self, any_t _cmp);
#define vec_cmp(vec1, vec2) __vec_cmp(vec1, vec2)
