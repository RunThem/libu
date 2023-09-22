#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_QUEUE_CAP
#  define U_QUEUE_CAP 16
#endif

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/

#define u_queue_t(T)                                                                               \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __queue_new(size_t itsize, size_t cap);
#define u_queue_new(T, ...) __queue_new(sizeof(T), va_0th(U_QUEUE_CAP, __VA_ARGS__))

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __queue_clear(any_t _self);
#define u_queue_clear(queue) __queue_clear(queue)

void __queue_cleanup(any_t _self);
#define u_queue_cleanup(queue)                                                                     \
  do {                                                                                             \
    __queue_cleanup(queue);                                                                        \
    (queue) = nullptr;                                                                             \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __queue_itsize(any_t _self);
#define u_queue_itsize(queue) __queue_itsize(queue)

size_t __queue_len(any_t _self);
#define queue_len(queue) __queue_len(queue)

size_t __queue_cap(any_t _self);
#define u_queue_cap(queue) __queue_cap(queue)

bool __queue_empty(any_t _self);
#define u_queue_empty(queue) __queue_empty(queue)

void __queue_peek(any_t _self);
#define u_queue_peek(queue) (__queue_peek(queue), (queue)->item)

void __queue_pop(any_t _self);
#define u_queue_pop(queue) __queue_pop(queue)

ret_t __queue_push(any_t _self);
#define u_queue_push(queue, _item) ((queue)->item = (_item), __queue_push(queue))
