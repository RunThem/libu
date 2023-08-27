#pragma once

#include "u.h"

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_QUE_ITEMS_CAP
#  define U_QUE_ITEMS_CAP 16
#endif

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;

  size_t s_idx;
  size_t e_idx;

  any_t items;
} queue_t;

#define queue(T)                                                                                   \
  struct {                                                                                         \
    queue_t _;                                                                                     \
    T it;                                                                                          \
  }*

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __queue_new(size_t itsize, size_t cap);
#define queue_new(T, arg...) __queue_new(sizeof(T), va_0th(U_QUE_ITEMS_CAP, arg))

ret_t __queue_init(any_t* _self, size_t itsize, size_t cap);
#define queue_init(queue, arg...)                                                                  \
  __queue_init(any(queue), sizeof((queue)->it), va_0th(U_QUE_ITEMS_CAP, arg))

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
ret_t __queue_resize(any_t _self, size_t cap);
#define queue_resize(queue, cap) __queue_resize(queue, cap)

ret_t __queue_clear(any_t _self);
#define queue_clear(queue) __queue_clear(queue)

ret_t __queue_cleanup(any_t _self);
#define queue_cleanup(queue) __queue_cleanup(queue)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __queue_itsize(any_t _self);
#define queue_itsize(queue) __queue_itsize(queue)

size_t __queue_len(any_t _self);
#define queue_len(queue) __queue_len(queue)

size_t __queue_cap(any_t _self);
#define queue_cap(queue) __queue_cap(queue)

bool __queue_empty(any_t _self);
#define queue_empty(queue) __queue_empty(queue)

ret_t __queue_push(any_t _self, any_t it);
#define queue_push(queue, _it) __queue_push(queue, ((queue)->it = (_it), &(queue)->it))

ret_t __queue_pop(any_t _self, any_t it);
#define queue_pop(queue) __queue_pop(queue, &(queue)->it)

ret_t __queue_peek(any_t _self, any_t it);
#define queue_peek(queue) (__queue_peek(queue, (&(queue)->it)), (queue)->it)
