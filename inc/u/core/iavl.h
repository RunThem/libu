#pragma once

#include <u/u.h>

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
typedef fnt(u_avl_cmp_fn, int, const void*, const void*);

typedef struct {
}* u_avl_t;

extern u_avl_t avl_new(size_t ksize, size_t vsize, u_avl_cmp_fn cmp_fn);

extern size_t avl_len(u_avl_t _self);

extern void avl_clear(u_avl_t _self);
extern void avl_cleanup(u_avl_t _self);

extern bool avl_isexist(u_avl_t _self, any_t key);

extern void avl_at(u_avl_t _self, any_t key, any_t val);
extern void avl_re(u_avl_t _self, any_t key, any_t val);
extern void avl_pop(u_avl_t _self, any_t key, any_t val);
extern void avl_put(u_avl_t _self, any_t key, any_t val);

extern void avl_range_init(u_avl_t _self);
extern bool avl_range(u_avl_t _self, bool flag, any_t key, any_t val);

/***************************************************************************************************
 * Api
 **************************************************************************************************/
#if 0
#  define u_avl(K, V)                                                                              \
    struct [[gnu::packed]] {                                                                       \
      u_avl_t _;                                                                                   \
      K key;                                                                                       \
      V val;                                                                                       \
    }*

#  define u_avl_init(avl, cmp_fn)                                                                  \
    do {                                                                                           \
      (avl)    = u_zalloc(sizeof(typeof(*(avl))));                                                 \
      (avl)->_ = avl_new(sizeof(typeof((avl)->key)), sizeof(typeof((avl)->val)), cmp_fn);          \
    } while (0)

#  define u_avl_from(avl, cmp_fn, ...)                                                             \
    do {                                                                                           \
      struct {                                                                                     \
        typeof((avl)->key) key;                                                                    \
        typeof((avl)->val) val;                                                                    \
      } _its[] = {__VA_ARGS__};                                                                    \
                                                                                                   \
      u_avl_init(avl, cmp_fn);                                                                     \
                                                                                                   \
      for (int i = 0; i < sizeof(_its) / sizeof(_its[0]); i++) {                                   \
        u_avl_put(avl, _its[i].key, _its[i].val);                                                  \
      }                                                                                            \
    } while (0)

#  define u_avl_len(avl) ({ avl_len((avl)->_); })

#  define u_avl_isinit(avl) ({ (avl) != nullptr && (avl)->_ != nullptr; })

#  define u_avl_isempty(avl) ({ avl_len((avl)->_) == 0; })

#  define u_avl_isexist(avl, _key)                                                                 \
    ({                                                                                             \
      (avl)->key = _key;                                                                           \
      avl_isexist((avl)->_, &(avl)->key);                                                          \
    })

#  define u_avl_clear(avl)                                                                         \
    do {                                                                                           \
      avl_clear((avl)->_);                                                                         \
    } while (0)

#  define u_avl_cleanup(avl)                                                                       \
    do {                                                                                           \
      avl_cleanup((avl)->_);                                                                       \
      u_free_if(avl);                                                                              \
      (avl) = nullptr;                                                                             \
    } while (0)

#  define u_avl_at(avl, _key)                                                                      \
    ({                                                                                             \
      (avl)->key = _key;                                                                           \
      avl_at((avl)->_, &(avl)->key, &(avl)->val);                                                  \
      (avl)->val;                                                                                  \
    })

#  define u_avl_re(avl, _key, _val)                                                                \
    do {                                                                                           \
      (avl)->key = _key;                                                                           \
      (avl)->val = _val;                                                                           \
      avl_re((avl)->_, &(avl)->key, &(avl)->val);                                                  \
    } while (0)

#  define u_avl_pop(avl, _key)                                                                     \
    ({                                                                                             \
      (avl)->key = _key;                                                                           \
      avl_pop((avl)->_, &(avl)->key, &(avl)->val);                                                 \
      (void)((avl)->val);                                                                          \
    })

#  define u_avl_put(avl, _key, _val)                                                               \
    do {                                                                                           \
      (avl)->key = _key;                                                                           \
      (avl)->val = _val;                                                                           \
      avl_put((avl)->_, &(avl)->key, &(avl)->val);                                                 \
    } while (0)

#  define u_avl_for(avl)                                                                           \
    for (avl_range_init((avl)->_); avl_range((avl)->_, 1, &(avl)->key, &(avl)->val);)

#  define u_avl_rfor(avl)                                                                          \
    for (avl_range_init((avl)->_); avl_range((avl)->_, 0, &(avl)->key, &(avl)->val);)

#endif
