#pragma once

#include <u/u.h>

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
typedef fnt(u_tbl_eq_fn, bool, const void*, const void*);

typedef struct {
}* u_tbl_t;

extern u_tbl_t tbl_new(size_t ksize, size_t vsize, u_tbl_eq_fn eq_fn);

extern size_t tbl_len(u_tbl_t _self);

extern bool tbl_isexist(u_tbl_t _self, any_t key);

extern void tbl_clear(u_tbl_t _self);
extern void tbl_cleanup(u_tbl_t _self);

extern void tbl_at(u_tbl_t _self, any_t key, any_t val);
extern void tbl_re(u_tbl_t _self, any_t key, any_t val);
extern void tbl_pop(u_tbl_t _self, any_t key, any_t val);
extern void tbl_put(u_tbl_t _self, any_t key, any_t val);

extern void tbl_range_init(u_tbl_t _self);
extern bool tbl_range(u_tbl_t _self, any_t key, any_t val);

/***************************************************************************************************
 * Api
 **************************************************************************************************/
#define u_tbl(K, V)                                                                                \
  struct [[gnu::packed]] {                                                                         \
    u_tbl_t _;                                                                                     \
    K key;                                                                                         \
    V val;                                                                                         \
  }*

#define u_tbl_init(tbl, eq_fn)                                                                     \
  do {                                                                                             \
    (tbl)    = u_zalloc(sizeof(typeof(*(tbl))));                                                   \
    (tbl)->_ = tbl_new(sizeof(typeof((tbl)->key)), sizeof(typeof((tbl)->val)), eq_fn);             \
  } while (0)

#define u_tbl_from(tbl, eq_fn, ...)                                                                \
  do {                                                                                             \
    struct {                                                                                       \
      typeof((tbl)->key) key;                                                                      \
      typeof((tbl)->val) val;                                                                      \
    } _its[] = {__VA_ARGS__};                                                                      \
                                                                                                   \
    u_tbl_init(tbl, eq_fn);                                                                        \
                                                                                                   \
    for (int i = 0; i < sizeof(_its) / sizeof(_its[0]); i++) {                                     \
      u_tbl_put(tbl, _its[i].key, _its[i].val);                                                    \
    }                                                                                              \
  } while (0)

#define u_tbl_len(tbl) ({ tbl_len((tbl)->_); })

#define u_tbl_isinit(tbl) ({ (tbl) != nullptr && (tbl)->_ != nullptr; })

#define u_tbl_isempty(tbl) ({ tbl_len((tbl)->_) == 0; })

#define u_tbl_isexist(tbl, _key)                                                                   \
  ({                                                                                               \
    (tbl)->key = _key;                                                                             \
    tbl_isexist((tbl)->_, &(tbl)->key);                                                            \
  })

#define u_tbl_clear(tbl)                                                                           \
  do {                                                                                             \
    tbl_clear((tbl)->_);                                                                           \
  } while (0)

#define u_tbl_cleanup(tbl)                                                                         \
  do {                                                                                             \
    tbl_cleanup((tbl)->_);                                                                         \
    u_free_if(tbl);                                                                                \
    (tbl) = nullptr;                                                                               \
  } while (0)

#define u_tbl_at(tbl, _key)                                                                        \
  ({                                                                                               \
    (tbl)->key = _key;                                                                             \
    tbl_at((tbl)->_, &(tbl)->key, &(tbl)->val);                                                    \
    (tbl)->val;                                                                                    \
  })

#define u_tbl_re(tbl, _key, _val)                                                                  \
  do {                                                                                             \
    (tbl)->key = _key;                                                                             \
    (tbl)->val = _val;                                                                             \
    tbl_re((tbl)->_, &(tbl)->key, &(tbl)->val);                                                    \
  } while (0)

#define u_tbl_pop(tbl, _key)                                                                       \
  ({                                                                                               \
    (tbl)->key = _key;                                                                             \
    tbl_pop((tbl)->_, &(tbl)->key, &(tbl)->val);                                                   \
    (void)((tbl)->val);                                                                            \
  })

#define u_tbl_put(tbl, _key, _val)                                                                 \
  do {                                                                                             \
    (tbl)->key = _key;                                                                             \
    (tbl)->val = _val;                                                                             \
    tbl_put((tbl)->_, &(tbl)->key, &(tbl)->val);                                                   \
  } while (0)

#define u_tbl_for(tbl)                                                                             \
  for (tbl_range_init((tbl)->_); tbl_range((tbl)->_, &(tbl)->key, &(tbl)->val);)

#define u_tbl_rfor(tbl)                                                                            \
  for (tbl_range_init((tbl)->_); tbl_range((tbl)->_, &(tbl)->key, &(tbl)->val);)
