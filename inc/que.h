#pragma once

#include "u.h"

#ifndef U_QUE_ITEMS_CAP
#  define U_QUE_ITEMS_CAP 16
#endif

typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;
  size_t s_idx;
  size_t e_idx;
  any_t items;
} que_t;

#define que(T)                                                                                     \
  struct {                                                                                         \
    que_t _;                                                                                       \
    T it;                                                                                          \
  }

#define que_isinit(que) ((que)->_.itsize != 0)
#define que_itsize(que) ((que)->_.itsize)
#define que_len(que)    ((que)->_.len)
#define que_cap(que)    ((que)->_.cap)
#define que_empty(que)  ((que)->_.len == 0)

/* private macro */
#define ____que_bzero(que) bzero(&(que)->it, que_itsize(que))

ret_t __que_init(any_t _self, size_t itsize, size_t cap);
#define que_init(que, arg...) __que_init(que, sizeof((que)->it), va_0th(U_QUE_ITEMS_CAP, arg))

ret_t __que_resize(any_t _self, size_t cap);
#define que_resize(que, cap) __que_resize(que, cap)

ret_t __que_push(any_t _self, any_t it);
#define que_push(que, _it) __que_push(que, (____que_bzero(que), (que)->it = (_it), &(que)->it))

ret_t __que_pop(any_t _self, any_t it);
#define que_pop(que) __que_pop(que, (____que_bzero(que), &(que)->it))

ret_t __que_peek(any_t _self, any_t it);
#define que_peek(que) (__que_peek(que, (____que_bzero(que), &(que)->it)), (que)->it)

ret_t __que_clear(any_t _self);
#define que_clear(que) __que_clear(que)

ret_t __que_cleanup(any_t _self);
#define que_cleanup(que) __que_cleanup(que)
