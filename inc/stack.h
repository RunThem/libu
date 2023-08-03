#pragma once

#include "u.h"

#ifndef U_STACK_ITEMS_CAP
#  define U_STACK_ITEMS_CAP 16
#endif

typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
} stack_t;

#define stack(T)                                                                                   \
  struct {                                                                                         \
    stack_t _;                                                                                     \
    T it;                                                                                          \
  }

#define stack_isinit(stack) ((stack)->_.itsize != 0)
#define stack_itsize(stack) ((stack)->_.itsize)
#define stack_len(stack)    ((stack)->_.len)
#define stack_cap(stack)    ((stack)->_.cap)
#define stack_empty(stack)  ((stack)->_.len == 0)

/* private macro */
#define ____stack_bzero(stack) bzero(&(stack)->it, stack_itsize(stack))

ret_t __stack_init(any_t _self, size_t itsize, size_t cap);
#define stack_init(stack, arg...)                                                                  \
  __stack_init(stack, sizeof((stack)->it), va_0th(U_STACK_ITEMS_CAP, arg))

ret_t __stack_resize(any_t _self, size_t cap);
#define stack_resize(stack, cap) __stack_resize(stack, cap)

ret_t __stack_push(any_t _self, any_t it);
#define stack_push(stack, _it)                                                                     \
  __stack_push(stack, (____stack_bzero(stack), (stack)->it = (_it), &(stack)->it))

ret_t __stack_pop(any_t _self, any_t it);
#define stack_pop(stack) __stack_pop(stack, (____stack_bzero(stack), &(stack)->it))

ret_t __stack_peek(any_t _self, any_t it);
#define stack_peek(stack) (__stack_peek(stack, (____stack_bzero(stack), &(stack)->it), (stack)->it)

ret_t __stack_clear(any_t _self);
#define stack_clear(stack) __stack_clear(stack)

ret_t __stack_cleanup(any_t _self);
#define stack_cleanup(stack) __stack_cleanup(stack)
