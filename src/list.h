#pragma once

#include "list.in"
#include "u.h"

#define list(T)                                                                                    \
  struct {                                                                                         \
    size_t len;                                                                                    \
    void* head; /* head */                                                                         \
    void* tail; /* tail */                                                                         \
    struct {                                                                                       \
      void* prev; /* head */                                                                       \
      void* next; /* tail */                                                                       \
      T val;                                                                                       \
    } node[0];                                                                                     \
  }*

#undef _
#define _(list)      (*(list))
#define list_T(list) typeof(_(list)->node[0])
#define list_V(list) typeof(_(list)->node[0].val)

#define list_itsize(list) ({ sizeof(list_T(list)); })

#define list_init(list) __list_init(list)

#define list_clear(list) __list_clear(list)

#define list_cleanup(list) __list_cleanup(list)
