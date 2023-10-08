#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef fnt(u_avl_cmp_fn, int, const void*, const void*);

#define u_avl_t(T)                                                                                 \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __avl_new(size_t itsize, u_avl_cmp_fn cmp_fn);
#define u_avl_new(T, fn) __avl_new(sizeof(T), fn)

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __avl_clear(any_t _self);
#define u_avl_clear(avl) __avl_clear(avl)

void __avl_cleanup(any_t _self);
#define u_avl_cleanup(avl)                                                                         \
  do {                                                                                             \
    __avl_cleanup(avl);                                                                            \
    (avl) = nullptr;                                                                               \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __avl_itsize(any_t _self);
#define u_avl_itsize(avl) __avl_itsize(avl)

size_t __avl_len(any_t _self);
#define u_avl_len(avl) __avl_len(avl)

bool __avl_empty(any_t _self);
#define u_avl_empty(avl) __avl_empty(avl)

bool __avl_exist(any_t _self);
#define u_avl_exist(avl, _item) ((avl)->item = (_item), __avl_exist(avl))

void __avl_re(any_t _self);
#define u_avl_re(avl, _item) ((avl)->item = (_item), __avl_re(avl))

void __avl_at(any_t _self);
#define u_avl_at(avl, _item) ((avl)->item = (_item), __avl_at(avl), ((avl)->item))

void __avl_pop(any_t _self);
#define u_avl_pop(avl, _item) ((avl)->item = (_item), __avl_pop(avl), ((avl)->item))

ret_t __avl_push(any_t _self);
#define u_avl_push(avl, _item) ((avl)->item = (_item), __avl_push(avl))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
void __avl_range_init(any_t _self);
bool __avl_range(any_t _self, bool flag);
#define u_avl_for(avl)  for (__avl_range_init(avl); __avl_range(avl, true);)
#define u_avl_rfor(avl) for (__avl_range_init(avl); __avl_range(avl, false);)

#ifndef NDEBUG
extern void __avl_debug(any_t _self);
#  define u_avl_debug(map) __avl_debug(map)
#endif
