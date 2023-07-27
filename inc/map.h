#pragma once

#include "list.h"
#include "u.h"
#include "vec.h"

#ifndef U_MAP_BUCKETS_NUM
#  define U_MAP_BUCKETS_NUM 5
#endif

typedef uint64_t hash_t;

typedef fnt(map_hash_fn, hash_t, const uint8_t*, size_t);

extern hash_t map_mem_hash(const uint8_t* ptr, size_t len);
extern hash_t map_int_hash(const uint8_t* ptr, size_t len);

typedef struct {
  map_hash_fn hash_fn;
  size_t ksize;
  size_t vsize;
  size_t len;
  vec_t nodes;
  list_t buckets[U_MAP_BUCKETS_NUM];
} map_t;

#define map(K, V)                                                                                  \
  struct {                                                                                         \
    map_t _;                                                                                       \
    K key;                                                                                         \
    V val;                                                                                         \
  }

#define map_isinit(map) ((map)->_.ksize != 0 && (map)->_.vsize != 0)
#define map_ksize(map)  ((map)->_.ksize)
#define map_vsize(map)  ((map)->_.vsize)
#define map_len(map)    ((map)->_.len)
#define map_empty(map)  ((map)->_.len == 0)

#define ____map_k_bzero(map) bzero(&(map)->key, map_ksize(map))
#define ____map_v_bzero(map) bzero(&(map)->val, map_vsize(map))

ret_t __map_init(any_t _self, size_t ksize, size_t vsize, map_hash_fn fn);
#define map_init(map, fn) __map_init(map, sizeof((map)->key), sizeof((map)->val), fn)

ret_t __map_push(any_t _self, any_t key, any_t val);
#define map_push(map, _key, _val)                                                                  \
  __map_push(map,                                                                                  \
             (____map_k_bzero(map), (map)->key = (_key), &(map)->key),                             \
             (____map_v_bzero(map), (map)->val = (_val), &(map)->val))

ret_t __map_pop(any_t _self, any_t key, any_t val);
#define map_pop(map, _key)                                                                         \
  __map_pop(map,                                                                                   \
            (____map_k_bzero(map), (map)->key = (_key), &(map)->key),                              \
            (____map_v_bzero(map), &(map)->val))

ret_t __map_at(any_t _self, any_t key, any_t val);
#define map_at(map, _key)                                                                          \
  __map_at(map,                                                                                    \
           (____map_k_bzero(map), (map)->key = (_key), &(map)->key),                               \
           (____map_v_bzero(map), &(map)->val))

ret_t __map_clear(any_t _self);
#define map_clear(map) __map_clear(map)

ret_t __map_cleanup(any_t _self);
#define map_cleanup(map) __map_cleanup(map)

#define map_for(map, it)                                                                           \
  for (size_t i = 0; i < mm._.nodes.len; i++)                                                      \
    for (auto(it) = ((struct {                                                                     \
           any_t __prev;                                                                           \
           any_t __next;                                                                           \
           hash_t __hash;                                                                          \
           typeof((map)->key) key;                                                                 \
           typeof((map)->val) val;                                                                 \
         }**)((map)->_.nodes.items))[i];                                                           \
         (it) != nullptr;                                                                          \
         (it) = nullptr)
