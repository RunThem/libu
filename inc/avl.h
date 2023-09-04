#pragma once

#include "u.h"

typedef fnt(avl_cmp_fn, int, const void*, const void*);

#define avl(T)                                                                                     \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

any_t __avl_new(size_t itsize, avl_cmp_fn fn);
#define avl_new(T, fn) __avl_new(sizeof(T), fn)

void __avl_clear(any_t _self);
#define avl_clear(avl) __avl_clear(avl)

size_t __avl_len(any_t _self);
#define avl_len(avl) __avl_len(avl)

void __avl_pop(any_t _self);
#define avl_pop(avl, _item) ((avl)->item = (_item), __avl_pop(avl))

ret_t __avl_push(any_t _self);
#define avl_push(avl, _item) ((avl)->item = (_item), __avl_push(avl))

void avl_debug(any_t _self);
