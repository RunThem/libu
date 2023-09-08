#pragma once

#include "u.h"

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_VEC_CAP
#  define U_VEC_CAP 16
#endif

#define vec(T)                                                                                     \
  struct {                                                                                         \
    T* item;                                                                                       \
  }*

#define __vec_item(vec) as((vec) + 1, typeof((vec)->item))

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __vec_new(size_t itsize, size_t cap);
#define vec_new(T, arg...) __vec_new(sizeof(T), va_0th(U_VEC_CAP, arg))

any_t __vec_clone(any_t _self);
#define vec_clone(vec) ((typeof(vec))__vec_clone(vec))

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
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

any_t __vec_at(any_t _self, size_t idx);
#define vec_at(vec, idx)  (*(as(__vec_at(vec, idx), typeof((vec)->item))))
#define vec_at_front(vec) (*(as(__vec_at(vec, 0), typeof((vec)->item))))
#define vec_at_back(vec)  (*(as(__vec_at(vec, vec_len(vec) - 1), typeof((vec)->item))))

void __vec_pop(any_t _self, size_t idx);
#define vec_pop(vec, idx)  __vec_pop(vec, idx)
#define vec_pop_front(vec) __vec_pop(vec, 0)
#define vec_pop_back(vec)  __vec_pop(vec, vec_len(vec) - 1)

ret_t __vec_push(any_t _self, size_t idx);
#define vec_push(vec, idx, _item)  (*__vec_item(vec) = (_item), __vec_push(vec, idx))
#define vec_push_front(vec, _item) (*__vec_item(vec) = (_item), __vec_push(vec, 0))
#define vec_push_back(vec, _item)  (*__vec_item(vec) = (_item), __vec_push(vec, vec_len(vec)))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __vec_range(any_t _self, bool flag);
#define vec_for(vec, i)                                                                            \
  (vec)->item = nullptr;                                                                           \
  for (size_t i = 0; __vec_range(vec, true); (i)++)

#define vec_rfor(vec, i)                                                                           \
  (vec)->item = nullptr;                                                                           \
  for (size_t i = vec_len(vec) - 1; __vec_range(vec, false); (i)--)

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
void __vec_sort(any_t _self, cmp_fn fn);
#define vec_sort(vec, fn) __vec_sort(vec, fn)

ssize_t __vec_find(any_t _self, eq_fn fn);
#define vec_find(vec, _item, fn) (*__vec_item(vec) = (_item), __vec_find(vec, fn))

ssize_t __vec_min(any_t _self, cmp_fn fn);
#define vec_min(vec, fn) __vec_min(vec, fn)

ssize_t __vec_max(any_t _self, cmp_fn fn);
#define vec_max(vec, fn) __vec_max(vec, fn)
