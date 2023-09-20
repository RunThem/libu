#pragma once

#include "u.h"

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_STACK_CAP
#  define U_STACK_CAP 16
#endif

#define stack(T)                                                                                   \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __stack_new(size_t itsize, size_t cap);
#define stack_new(T, arg...) __stack_new(sizeof(T), va_0th(U_STACK_CAP, arg))

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __stack_clear(any_t _self);
#define stack_clear(stack) __stack_clear(stack)

void __stack_cleanup(any_t _self);
#define stack_cleanup(stack)                                                                       \
  do {                                                                                             \
    __stack_cleanup(stack);                                                                        \
    (stack) = nullptr;                                                                             \
  } while (0)

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

void __stack_peek(any_t _self);
#define stack_peek(stack) (__stack_peek(stack), (stack)->item)

void __stack_pop(any_t _self);
#define stack_pop(stack) __stack_pop(stack)

ret_t __stack_push(any_t _self);
#define stack_push(stack, _item) ((stack)->item = (_item), __stack_push(stack))