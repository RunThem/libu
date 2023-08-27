#pragma once

#include "u.h"

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef struct {
  size_t itsize;
  size_t len;

  any_t itor;

  any_t head;
  any_t tail;
} list_t;

#define list(T)                                                                                    \
  struct {                                                                                         \
    list_t _;                                                                                      \
    T it;                                                                                          \
  }*

#define list_iter(T)                                                                               \
  struct {                                                                                         \
    any_t prev;                                                                                    \
    any_t next;                                                                                    \
    T it;                                                                                          \
  }

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
/* private macro */
#define ____list_node(list, node)                                                                  \
  ((struct {                                                                                       \
    any_t prev;                                                                                    \
    any_t next;                                                                                    \
    typeof((list)->it) it;                                                                         \
  }*)(node))

any_t __list_new(size_t itsize);
#define list_new(T) __list_new(sizeof(T))

ret_t __list_init(any_t* _self, size_t itsize);
#define list_init(list) __list_init(any(list), sizeof((*(list))->it))

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
ret_t __list_clear(any_t _self);
#define list_clear(list) __list_clear(list)

ret_t __list_cleanup(any_t _self);
#define list_cleanup(list)                                                                         \
  do {                                                                                             \
    __list_cleanup(list);                                                                          \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __list_itsize(any_t _self);
#define list_itsize(list) __list_itsize(list)

size_t __list_len(any_t _self);
#define list_len(list) __list_len(list)

bool __list_empty(any_t _self);
#define list_empty(list) __list_empty(list)

ret_t __list_erase(any_t _self, any_t idx);
#define list_erase(list, idx) __list_erase(list, idx)

ret_t __list_push(any_t _self, any_t idx, any_t it);
#define list_push(list, idx, _it) __list_push(list, idx, ((list)->it = (_it), &(list)->it))

ret_t __list_push_f(any_t _self, any_t it);
#define list_push_f(list, _it) __list_push_f(list, ((list)->it = (_it), &(list)->it))

ret_t __list_push_b(any_t _self, any_t it);
#define list_push_b(list, _it) __list_push_b(list, ((list)->it = (_it), &(list)->it))

ret_t __list_pop(any_t _self, any_t idx, any_t it);
#define list_pop(list, idx) (__list_pop(list, idx, (&(list)->it)), (list)->it)

ret_t __list_pop_f(any_t _self, any_t it);
#define list_pop_f(list) (__list_pop_f(list, (list)->_.head, (&(list)->it)), (list)->it)

ret_t __list_pop_b(any_t _self, any_t it);
#define list_pop_b(list) (__list_pop_b(list, (list)->_.tail, (&(list)->it)), (list)->it)

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
any_t __list_head(any_t _self, any_t it);
#define list_head(list) __list_head(list, &(list)->it)

any_t __list_tail(any_t _self, any_t it);
#define list_tail(list) __list_tail(list, &(list)->it)

any_t __list_next(any_t _self, any_t it);
#define list_next(list) __list_next(list, &(list)->it)

any_t __list_prev(any_t _self, any_t it);
#define list_prev(list) __list_prev(list, &(list)->it)

#define list_for(list, _it)                                                                        \
  for (any_t(_it) = list_head(list); (_it) != nullptr; (_it) = list_next(list))
#define list_rfor(list, _it)                                                                       \
  for (any_t(_it) = list_tail(list); (_it) != nullptr; (_it) = list_prev(list))

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
any_t __list_find(any_t _self, any_t it, eq_fn fn);
#define list_find(list, _it, fn)                                                                   \
  ____list_node(list, __list_find(list, ((list)->it = (_it), &(list)->it), fn))
