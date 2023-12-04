#include <u/u.h>

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef fnt(u_set_cmp_fn, int, const void*, const void*);

#define u_set_t(T)                                                                                 \
  struct {                                                                                         \
    T item;                                                                                        \
  }*

/*************************************************************************************************
 * Create
 *************************************************************************************************/
#define u_set_new(T, fn) (__avl_new(sizeof(T), 1, fn))

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
#define u_set_clear(set) (__avl_clear(set))

#define u_set_cleanup(set)                                                                         \
  do {                                                                                             \
    __avl_cleanup(set);                                                                            \
    (set) = nullptr;                                                                               \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
#define u_set_itsize(set) (__avl_ksize(set))

#define u_set_len(set) (__avl_len(set))

#define u_set_empty(set) (__avl_empty(set))

#define u_set_exist(set, _item) ((set)->item = (_item), __avl_exist(set))

#define u_set_re(set, _item) ((set)->item = (_item), __avl_re(set))

#define u_set_at(set, _item) ((set)->item = (_item), __avl_at(set), (set)->item)

#define u_set_pop(set, _item) ((set)->item = (_item), __avl_pop(set), (set)->item)

#define u_set_push(set, _item) ((set)->item = (_item), __avl_push(set))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
#define u_set_for(set)  u_avl_for(set)
#define u_set_rfor(set) u_avl_rfor(set)
