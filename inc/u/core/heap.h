#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
enum u_heap_attr {
  U_HEAP_MIN = 1,
  U_HEAP_MAX,
};

#ifndef U_HEAP_CAP
#  define U_HEAP_CAP 16
#endif

typedef fnt(u_heap_cmp_fn, int, const void*, const void*);

#define u_heap_t(T)                                                                                \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __heap_new(size_t itsize, u_heap_cmp_fn cmp_fn, enum u_heap_attr attr);
#define u_heap_new(T, cmp_fn, attr) (__heap_new(sizeof(T), cmp_fn, attr))

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __heap_itsize(any_t _self);
#define u_heap_itsize(heap) (__heap_itsize(heap))

size_t __heap_len(any_t _self);
#define u_heap_len(heap) (__heap_len(heap))

size_t __heap_cap(any_t _self);
#define u_heap_cap(heap) (__heap_cap(heap))

bool __heap_empty(any_t _self);
#define u_heap_empty(stack) (__heap_empty(stack))

void __heap_peek(any_t _self);
#define u_heap_peek(heap) (__heap_peed(heap), (heap)->item)

void __heap_pop(any_t _self);
#define u_heap_pop(heap) (__heap_pop(heap), (heap)->item)

ret_t __heap_push(any_t _self);
#define u_heap_push(heap, _item) ((heap)->item = (_item), __heap_push(heap))
