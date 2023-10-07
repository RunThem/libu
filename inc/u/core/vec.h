#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_VEC_CAP
#  define U_VEC_CAP 16
#endif

#define u_vec_t(T)                                                                                 \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __vec_new(size_t itsize, size_t cap);
#define u_vec_new(T, ...) __vec_new(sizeof(T), va_0th(U_VEC_CAP, __VA_ARGS__))

any_t __vec_clone(any_t _self);
#define u_vec_clone(vec) ((typeof(vec))__vec_clone(vec))

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __vec_clear(any_t _self);
#define u_vec_clear(vec) __vec_clear(vec)

void __vec_cleanup(any_t _self);
#define u_vec_cleanup(vec)                                                                         \
  do {                                                                                             \
    __vec_cleanup(vec);                                                                            \
    (vec) = nullptr;                                                                               \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __vec_itsize(any_t _self);
#define u_vec_itsize(vec) __vec_itsize(vec)

size_t __vec_len(any_t _self);
#define u_vec_len(vec) __vec_len(vec)

size_t __vec_cap(any_t _self);
#define u_vec_cap(vec) __vec_cap(vec)

bool __vec_empty(any_t _self);
#define u_vec_empty(vec) __vec_empty(vec)

void __vec_at(any_t _self, size_t idx);
void __vec_at_front(any_t _self);
void __vec_at_back(any_t _self);
#define u_vec_at(vec, idx)  (__vec_at(vec, idx), (vec)->item)
#define u_vec_at_front(vec) (__vec_at_front(vec), (vec)->item)
#define u_vec_at_back(vec)  (__vec_at_back(vec), (vec)->item)

void __vec_pop(any_t _self, size_t idx);
void __vec_pop_front(any_t _self);
void __vec_pop_back(any_t _self);
#define u_vec_pop(vec, idx)  (__vec_pop(vec, idx), (vec)->item)
#define u_vec_pop_front(vec) (__vec_pop_front(vec), (vec)->item)
#define u_vec_pop_back(vec)  (__vec_pop_back(vec), (vec)->item)

ret_t __vec_push(any_t _self, size_t idx);
ret_t __vec_push_front(any_t _self);
ret_t __vec_push_back(any_t _self);
#define u_vec_push(vec, idx, _item)  ((vec)->item = (_item), __vec_push(vec, idx))
#define u_vec_push_front(vec, _item) ((vec)->item = (_item), __vec_push_front(vec))
#define u_vec_push_back(vec, _item)  ((vec)->item = (_item), __vec_push_back(vec))

void __vec_re(any_t _self, size_t idx);
void __vec_re_front(any_t _self);
void __vec_re_back(any_t _self);
#define u_vec_re(vec, idx, _item)  ((vec)->item = (_item), __vec_re(vec, idx))
#define u_vec_re_front(vec, _item) ((vec)->item = (_item), __vec_re_front(vec))
#define u_vec_re_back(vec, _item)  ((vec)->item = (_item), __vec_re_back(vec))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __vec_range(any_t _self, ssize_t* idx, bool flag);
#define u_vec_for(vec, i)  for (ssize_t i = -2; __vec_range(vec, &(i), true); (i)++)
#define u_vec_rfor(vec, i) for (ssize_t i = -2; __vec_range(vec, &(i), false); (i)--)

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
void __vec_sort(any_t _self, cmp_fn fn);
#define u_vec_sort(vec, fn) __vec_sort(vec, fn)

ssize_t __vec_find(any_t _self, eq_fn fn);
#define u_vec_find(vec, _item, fn) ((vec)->item = (_item), __vec_find(vec, fn))

ssize_t __vec_min(any_t _self, cmp_fn fn);
#define u_vec_min(vec, fn) __vec_min(vec, fn)

ssize_t __vec_max(any_t _self, cmp_fn fn);
#define u_vec_max(vec, fn) __vec_max(vec, fn)
