#pragma once

#include "u.h"

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_STACK_ITEMS_CAP
#  define U_STACK_ITEMS_CAP 16
#endif

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
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
  }*

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __stack_new(size_t itsize, size_t cap);
#define stack_new(T, arg...) __stack_new(sizeof(T), va_0th(U_STACK_ITEMS_CAP, arg))

ret_t __stack_init(any_t* _self, size_t itsize, size_t cap);
#define stack_init(stack, arg...)                                                                  \
  __stack_init(any(stack), sizeof((stack)->it), va_0th(U_STACK_ITEMS_CAP, arg))

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
ret_t __stack_resize(any_t _self, size_t cap);
#define stack_resize(stack, cap) __stack_resize(stack, cap)

ret_t __stack_clear(any_t _self);
#define stack_clear(stack) __stack_clear(stack)

ret_t __stack_cleanup(any_t _self);
#define stack_cleanup(stack) __stack_cleanup(stack)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __stack_itsize(any_t _self);
#define stack_itsize(stack) __stack_itsize(stack)

size_t __stack_len(any_t _self);
#define stack_len(stack) __stack_len(stack)

size_t __stack_cap(any_t _self);
#define stack_cap(stack) __stack_cap(stack)

bool __stack_empty(any_t _self);
#define stack_empty(stack) __stack_empty(stack)

ret_t __stack_push(any_t _self, any_t it);
#define stack_push(stack, _it) __stack_push(stack, ((stack)->it = (_it), &(stack)->it))

ret_t __stack_pop(any_t _self, any_t it);
#define stack_pop(stack) __stack_pop(stack, &(stack)->it)

ret_t __stack_peek(any_t _self, any_t it);
#define stack_peek(stack) (__stack_peek(stack, &(stack)->it), (stack)->it)
