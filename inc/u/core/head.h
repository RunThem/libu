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

#define u_head_t(K, V)                                                                             \
  struct {                                                                                         \
    V* val;                                                                                        \
    K key;                                                                                         \
  }*

#define __head_key(head, _key) (head)->key = (_key)
#define __head_val(head, _val)                                                                     \
  *as(any(head) + sizeof((head)->key) + sizeof(any_t), typeof((head)->val)) = (_val)

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __head_new(size_t ksize, size_t vsize, u_head_cmp_fn cmp_fn, enum u_head_attr attr);
#define head_new(K, V, cmp_fn, attr) __head_new(sizeof(K), sizeof(V), cmp_fn, attr)

ret_t __head_push(any_t _self);
#define head_push(head, _key, _val)                                                                \
  (__head_key(head, _key), __head_val(head, _val), __head_push(head))
