#pragma once

#include <u/u.h>

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
typedef fnt(u_tbl_eq_fn, bool, const void*, const void*);

typedef struct {
}* u_tbl_t;

u_tbl_t tbl_new(size_t ksize, size_t vsize, u_tbl_eq_fn eq_fn);

size_t tbl_len(u_tbl_t _self);
bool tbl_isempty(u_tbl_t _self);
bool tbl_isexist(u_tbl_t _self, any_t key);

void tbl_clear(u_tbl_t _self);
void tbl_cleanup(u_tbl_t _self);
void tbl_at(u_tbl_t _self, any_t key, any_t val);
void tbl_re(u_tbl_t _self, any_t key, any_t val);
void tbl_pop(u_tbl_t _self, any_t key, any_t val);
void tbl_put(u_tbl_t _self, any_t key, any_t val);

void tbl_range_init(u_tbl_t _self);
bool tbl_range(u_tbl_t _self, any_t key, any_t val);

/***************************************************************************************************
 * Api
 **************************************************************************************************/
#define u_tbl(K, V)                                                                                \
  struct [[gnu::packed]] {                                                                         \
    u_tbl_t _;                                                                                     \
    K key;                                                                                         \
    V val;                                                                                         \
  }

#define u_tbl_new(K, V, eq_fn)                                                                     \
  { ._ = tbl_new(sizeof(K), sizeof(V), eq_fn) }

#define u_tbl_init(tbl, eq_fn)                                                                     \
  do {                                                                                             \
    (tbl)._ = tbl_new(sizeof((tbl).key), sizeof((tbl).val), eq_fn);                                \
  } while (0)

#define u_tbl_len(tbl)     ({ tbl_len((tbl)._); })
#define u_tbl_isinit(tbl)  ({ (tbl)._ != nullptr; })
#define u_tbl_isempty(tbl) ({ tbl_isempty((tbl)._); })
#define u_tbl_isexist(tbl, _key)                                                                   \
  ({                                                                                               \
    (tbl).key = _key;                                                                              \
    tbl_isexist((tbl)._, &(tbl).key);                                                              \
  })

#define u_tbl_clear(tbl)                                                                           \
  do {                                                                                             \
    tbl_clear((tbl)._);                                                                            \
  } while (0)

#define u_tbl_cleanup(tbl)                                                                         \
  do {                                                                                             \
    tbl_cleanup((tbl)._);                                                                          \
    (tbl)._ = nullptr;                                                                             \
  } while (0)

#define u_tbl_at(tbl, _key)                                                                        \
  ({                                                                                               \
    (tbl).key = _key;                                                                              \
    tbl_at((tbl)._, &(tbl).key, &(tbl).val);                                                       \
    (tbl).val;                                                                                     \
  })

#define u_tbl_re(tbl, _key, _val)                                                                  \
  do {                                                                                             \
    (tbl).key = _key;                                                                              \
    (tbl).val = _val;                                                                              \
    tbl_re((tbl)._, &(tbl).key, &(tbl).val);                                                       \
  } while (0)

#define u_tbl_pop(tbl, _key)                                                                       \
  ({                                                                                               \
    (tbl).key = _key;                                                                              \
    tbl_pop((tbl)._, &(tbl).key, &(tbl).val);                                                      \
    (void)((tbl).val);                                                                             \
  })

#define u_tbl_put(tbl, _key, _val)                                                                 \
  do {                                                                                             \
    (tbl).key = _key;                                                                              \
    (tbl).val = _val;                                                                              \
    tbl_put((tbl)._, &(tbl).key, &(tbl).val);                                                      \
  } while (0)

#define u_tbl_for(tbl) for (tbl_range_init((tbl)._); tbl_range((tbl)._, &(tbl).key, &(tbl).val);)
