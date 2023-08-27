#pragma once

#include "u.h"

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#ifndef U_MAP_BUCKETS_NUM
#  define U_MAP_BUCKETS_NUM 64
#endif

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef uint64_t hash_t;

typedef fnt(map_hash_fn, hash_t, const uint8_t*, size_t);
typedef fnt(map_eq_fn, bool, const void*, const void*);

typedef struct map_node_t map_node_t;
struct map_node_t {
  map_node_t* next;
  map_node_t* lprev;
  map_node_t* lnext;
  hash_t hash;
};

typedef struct {
  map_hash_fn hash_fn;
  map_eq_fn eq_fn;

  map_node_t* lhead;
  map_node_t* ltail;

  map_node_t* itor;

  size_t ksize;
  size_t vsize;
  size_t len;

  map_node_t buckets[U_MAP_BUCKETS_NUM];
} map_t;

#define map(K, V)                                                                                  \
  struct {                                                                                         \
    map_t _;                                                                                       \
    K key;                                                                                         \
    V val;                                                                                         \
  }*

extern hash_t map_mem_hash(const uint8_t* ptr, size_t len);
extern hash_t map_int_hash(const uint8_t* ptr, size_t len);

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __map_new(size_t ksize, size_t vsize, map_hash_fn hash_fn, map_eq_fn eq_fn);
#define map_new(K, V, eq_fn, arg...)                                                               \
  __map_new(sizeof(K), sizeof(V), va_0th(map_mem_hash, arg), eq_fn)

ret_t __map_init(any_t* _self, size_t ksize, size_t vsize, map_hash_fn hash_fn, map_eq_fn eq_fn);
#define map_init(map, eq_fn, arg...)                                                               \
  __map_init(any(map), sizeof((map)->key), sizeof((map)->val), va_0th(map_mem_hash, arg), eq_fn)

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
ret_t __map_clear(any_t _self);
#define map_clear(map) __map_clear(map)

ret_t __map_cleanup(any_t _self);
#define map_cleanup(map) __map_cleanup(map)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __map_ksize(any_t _self);
#define map_ksize(map) __map_ksize(map)

size_t __map_vsize(any_t _self);
#define map_vsize(map) __map_vsize(map)

size_t __map_len(any_t _self);
#define map_len(map) __map_len(map)

bool __map_empty(any_t _self);
#define map_empty(map) __map_empty(map)

ret_t __map_at(any_t _self, any_t key, any_t val);
#define map_at(map, _key)                                                                          \
  (__map_at(map, ((map)->key = (_key), &(map)->key), (&(map)->val)), (map)->val)

ret_t __map_push(any_t _self, any_t key, any_t val);
#define map_push(map, _key, _val)                                                                  \
  __map_push(map, ((map)->key = (_key), &(map)->key), ((map)->val = (_val), &(map)->val))

ret_t __map_pop(any_t _self, any_t key, any_t val);
#define map_pop(map, _key)                                                                         \
  (__map_pop(map, ((map)->key = (_key), &(map)->key), (&(map)->val)), (map)->val)

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
ret_t __map_range(any_t _self, any_t key, any_t val);
#define map_for(map) for (; 0 == __map_range(map, &(map)->key, &(map)->val);)