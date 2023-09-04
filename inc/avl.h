#pragma once

#include "u.h"

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef fnt(avl_cmp_fn, int, const void*, const void*);

#define avl(T)                                                                                     \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __avl_new(size_t itsize, avl_cmp_fn fn);
#define avl_new(T, fn) __avl_new(sizeof(T), fn)

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __avl_clear(any_t _self);
#define avl_clear(avl) __avl_clear(avl)

void __avl_cleanup(any_t _self);
#define avl_cleanup(avl)                                                                           \
  do {                                                                                             \
    __avl_cleanup(avl);                                                                            \
    (avl) = nullptr;                                                                               \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __avl_itsize(any_t _self);
#define avl_itsize(avl) __avl_itsize(avl)

size_t __avl_len(any_t _self);
#define avl_len(avl) __avl_len(avl)

bool __avl_empty(any_t _self);
#define avl_empty(avl) __avl_empty(avl)

void __avl_at(any_t _self);
#define avl_at(avl, _item) ((avl)->item = (_item), __avl_at(avl), (avl->item))

void __avl_pop(any_t _self);
#define avl_pop(avl, _item) ((avl)->item = (_item), __avl_pop(avl))

ret_t __avl_push(any_t _self);
#define avl_push(avl, _item) ((avl)->item = (_item), __avl_push(avl))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/

#ifndef NDEBUG
void avl_debug(any_t _self);
#endif
