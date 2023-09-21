#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#define U_MAP_RESIZE_RADIO 0.7

enum u_map_hash_fn {
  U_MAP_FNV_64_HASH_FN = 1,
  U_MAP_INT_HASH_FN,

  U_MAP_HASH_FN_MAX,
};

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef uint64_t u_hash_t;

typedef fnt(u_map_hash_fn, u_hash_t, const uint8_t*, size_t);
typedef fnt(u_map_eq_fn, bool, const void*, const void*);

#define u_map_t(K, V)                                                                              \
  struct {                                                                                         \
    V* val;                                                                                        \
    K key;                                                                                         \
  }*

#define __map_key(map, _key) (map)->key = (_key)
#define __map_val(map, _val)                                                                       \
  *as(any(map) + sizeof((map)->key) + sizeof(any_t), typeof((map)->val)) = (_val)

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __map_new(size_t ksize, size_t vsize, u_map_eq_fn eq_fn, enum u_map_hash_fn hash_fn);
#define u_map_new(K, V, eq_fn, hash_fn) __map_new(sizeof(K), sizeof(V), eq_fn, hash_fn)

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
ret_t __map_clear(any_t _self);
#define u_map_clear(map) __map_clear(map)

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
#define u_map_ksize(map) __map_ksize(map)

size_t __map_vsize(any_t _self);
#define u_map_vsize(map) __map_vsize(map)

size_t __map_len(any_t _self);
#define u_map_len(map) __map_len(map)

bool __map_empty(any_t _self);
#define u_map_empty(map) __map_empty(map)

bool __map_exist(any_t _self);
#define u_map_exist(map, _key) (__map_key(map, _key), __map_exist(map))

any_t __map_at(any_t _self);
#define u_map_at(map, _key) (*(__map_key(map, _key), as(__map_at(map), typeof((map)->val))))

void __map_pop(any_t _self);
#define u_map_pop(map, _key) (__map_key(map, _key), __map_pop(map))

ret_t __map_push(any_t _self);
#define u_map_push(map, _key, _val) (__map_key(map, _key), __map_val(map, _val), __map_push(map))

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __map_range(any_t _self);
#define u_map_for(map) for ((map)->val = nullptr; __map_range(map);)

#ifndef NDEBUG
extern void __map_debug(any_t _self);
#  define u_map_debug(map) __map_debug(map)
#endif
