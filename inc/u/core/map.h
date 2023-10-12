#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#define U_MAP_RESIZE_RADIO 0.75

enum u_map_hash_fn {
  U_MAP_FNV_64_HASH_FN = 1,
  U_MAP_INT_HASH_FN,

  U_MAP_HASH_FN_MAX,
};

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef u64_t u_hash_t;

typedef fnt(u_map_hash_fn, u_hash_t, const u8_t*, size_t);
typedef fnt(u_map_eq_fn, bool, const void*, const void*);

#define u_map_t(K, V)                                                                              \
  struct [[gnu::packed]] {                                                                         \
    K key;                                                                                         \
    V val;                                                                                         \
  }*

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __map_new(size_t ksize, size_t vsize, u_map_eq_fn eq_fn, enum u_map_hash_fn hash_fn);
#define u_map_new(K, V, eq_fn, hash_fn) (__map_new(sizeof(K), sizeof(V), eq_fn, hash_fn))

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
ret_t __map_clear(any_t _self);
#define u_map_clear(map) (__map_clear(map))

ret_t __map_cleanup(any_t _self);
#define u_map_cleanup(map)                                                                         \
  do {                                                                                             \
    __map_cleanup(map);                                                                            \
    (map) = nullptr;                                                                               \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __map_ksize(any_t _self);
#define u_map_ksize(map) (__map_ksize(map))

size_t __map_vsize(any_t _self);
#define u_map_vsize(map) (__map_vsize(map))

size_t __map_len(any_t _self);
#define u_map_len(map) (__map_len(map))

bool __map_empty(any_t _self);
#define u_map_empty(map) (__map_empty(map))

bool __map_exist(any_t _self);
#define u_map_exist(map, _key) ((map)->key = (_key), __map_exist(map))

void __map_re(any_t _self);
#define u_map_re(map, _key, _val) ((map)->key = (_key), (map)->val = (_val), __map_re(map))

void __map_at(any_t _self);
#define u_map_at(map, _key) ((map)->key = (_key), __map_at(map), (map)->val)

void __map_pop(any_t _self);
#define u_map_pop(map, _key) ((map)->key = (_key), __map_pop(map), (map)->val)

ret_t __map_push(any_t _self);
#define u_map_push(map, _key, _val) ((map)->key = (_key), (map)->val = (_val), __map_push(map))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
void __map_range_init(any_t _self);
bool __map_range(any_t _self);
#define u_map_for(map) for (__map_range_init(map); __map_range(map);)

/*************************************************************************************************
 * Debug
 *************************************************************************************************/
#ifndef NDEBUG
extern void __map_debug(any_t _self);
#  define u_map_debug(map) __map_debug(map)
#endif
