#pragma once

#include "u.h"

typedef struct {
  size_t itsize;
  size_t len;

  any_t head;
  any_t tail;
} list_t;

#define list(T)                                                                                    \
  struct {                                                                                         \
    list_t _;                                                                                      \
    T it;                                                                                          \
  }

#define list_itsize(list) ((list)->_.itsize)
#define list_len(list)    ((list)->_.len)
#define list_empty(list)  ((list)->_.len == 0)

ret_t __list_init(any_t _self, size_t itsize);
#define list_init(list, itsize) __list_init(list, itsize)

ret_t __list_push(any_t _self, any_t idx, any_t it);
#define list_push(list, idx, _it) __list_push(list, idx, ((list)->it = (_it), &(list)->it))
#define list_push_f(list, _it)    __list_push(list, nullptr, ((list)->it = (_it), &(list)->it))
#define list_push_b(list, _it)    __list_push(list, (list)->_.tail, ((list)->it = (_it), &(list)->it))

ret_t __list_pop(any_t _self, any_t idx, any_t it);
#define list_pop(list, idx) (__list_pop(list, idx, &(list)->it), (list)->it)
#define list_pop_f(list)    (__list_pop(list, (list)->_.head, &(list)->it), (list)->it)
#define list_pop_b(list)    (__list_pop(list, (list)->_.tail, &(list)->it), (list)->it)

ret_t __list_clear(any_t _self);
#define list_clear(list) __list_clear(list)

ret_t __list_cleanup(any_t _self);
#define list_cleanup(list) __list_cleanup(list)

/* private macro */
#define ____list_node(list, node)                                                                  \
  ((struct {                                                                                       \
    any_t prev;                                                                                    \
    any_t next;                                                                                    \
    typeof((list)->it) it;                                                                         \
  }*)(node))

any_t __list_find(any_t _self, any_t it, eq_fn fn);
#define list_find(list, _it, fn)                                                                   \
  ____list_node(list, __list_find(list, ((list)->it = (_it), &(list)->it), fn))

#define list_head(list) ____list_node(list, (list)->_.head)
#define list_tail(list) ____list_node(list, (list)->_.tail)

#define list_for(list, _it) for (auto(_it) = list_head(list); (_it) != nullptr; (_it) = (_it)->next)
#define list_rfor(list, _it)                                                                       \
  for (auto(_it) = list_tail(list); (_it) != nullptr; (_it) = (_it)->prev)
