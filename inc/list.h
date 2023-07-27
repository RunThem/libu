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

#define list_iter(T)                                                                               \
  struct {                                                                                         \
    any_t prev;                                                                                    \
    any_t next;                                                                                    \
    T it;                                                                                          \
  }

#define list_isinit(list) ((list)->_.itsize != 0)
#define list_itsize(list) ((list)->_.itsize)
#define list_len(list)    ((list)->_.len)
#define list_empty(list)  ((list)->_.len == 0)

/* private macro */
#define ____list_bzero(list) bzero(&(list)->it, list_itsize(list))
#define ____list_node(list, node)                                                                  \
  ((struct {                                                                                       \
    any_t prev;                                                                                    \
    any_t next;                                                                                    \
    typeof((list)->it) it;                                                                         \
  }*)(node))

ret_t __list_init(any_t _self, size_t itsize);
#define list_init(list) __list_init(list, sizeof((list)->it))

ret_t __list_push(any_t _self, any_t idx, any_t it);
#define list_push(list, idx, _it)                                                                  \
  __list_push(list, idx, (____list_bzero(list), (list)->it = (_it), &(list)->it))
#define list_push_f(list, _it)                                                                     \
  __list_push(list, nullptr, (____list_bzero(list), (list)->it = (_it), &(list)->it))
#define list_push_b(list, _it)                                                                     \
  __list_push(list, (list)->_.tail, (____list_bzero(list), (list)->it = (_it), &(list)->it))

ret_t __list_pop(any_t _self, any_t idx, any_t it);
#define list_pop(list, idx) (__list_pop(list, idx, (____list_bzero(list), &(list)->it)), (list)->it)
#define list_pop_f(list)                                                                           \
  (__list_pop(list, (list)->_.head, (____list_bzero(list), &(list)->it)), (list)->it)
#define list_pop_b(list)                                                                           \
  (__list_pop(list, (list)->_.tail, (____list_bzero(list), &(list)->it)), (list)->it)

ret_t __list_erase(any_t _self, any_t idx);
#define list_erase(list, idx) __list_erase(list, idx)

ret_t __list_clear(any_t _self);
#define list_clear(list) __list_clear(list)

ret_t __list_cleanup(any_t _self);
#define list_cleanup(list) __list_cleanup(list)

any_t __list_find(any_t _self, any_t it, eq_fn fn);
#define list_find(list, _it, fn)                                                                   \
  ____list_node(list,                                                                              \
                __list_find(list, (____list_bzero(list), (list)->it = (_it), &(list)->it), fn))

#define list_head(list) ____list_node(list, (list)->_.head)
#define list_tail(list) ____list_node(list, (list)->_.tail)

#define list_for(list, _it) for (auto(_it) = list_head(list); (_it) != nullptr; (_it) = (_it)->next)
#define list_rfor(list, _it)                                                                       \
  for (auto(_it) = list_tail(list); (_it) != nullptr; (_it) = (_it)->prev)
