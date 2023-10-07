#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
enum u_head_attr {
  U_HEAD_MIN = 1,
  U_HEAD_MAX,
};

#ifndef U_HEAD_CAP
#  define U_HEAD_CAP 16
#endif

typedef fnt(u_head_cmp_fn, int, const void*, const void*);

#define u_head_t(T)                                                                                \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

#define __head_item(head, _item) (head)->item = (_item)

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __head_new(size_t itsize, u_head_cmp_fn cmp_fn, enum u_head_attr attr);
#define head_new(T, cmp_fn, attr) __head_new(sizeof(T), cmp_fn, attr)

size_t __head_len(any_t _self);
#define head_len(head) __head_len(head)

size_t __head_cap(any_t _self);
#define head_cap(head) __head_cap(head)

void __head_pop(any_t _self);
#define head_pop(head) (__head_pop(head), (head)->item)

ret_t __head_push(any_t _self);
#define head_push(head, _item) (__head_item(head, _item), __head_push(head))
