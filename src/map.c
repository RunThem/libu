#include "map.h"

/* |    8    |     ...      |
 *    hash      key + val
 * */

fn_eq_def(map_hash_eq, hash_t, (x == y));
fn_eq_def(map_node_eq, any_t, (x == y));

#define __map_key(map, node) (any(node) + sizeof(hash_t))
#define __map_val(map, node) (any(node) + sizeof(hash_t) + (map)->ksize)

/* fnv 64-bit hash function */
hash_t map_mem_hash(const uint8_t* ptr, size_t len) {
  hash_t hash = 14695981039346656037U;
  for (size_t i = 0; i < len; ++i) {
    hash *= 1099511628211U;
    hash ^= (uint64_t)ptr[i];
  }

  return hash;
}

/* int hash function */
hash_t map_int_hash(const uint8_t* ptr, size_t len) {
  hash_t hash = 0;

  if (len == 1) {
    hash = (hash_t)(*(uint8_t*)ptr);
  } else if (len == 2) {
    hash = (hash_t)(*(uint16_t*)ptr);
  } else if (len == 4) {
    hash = (hash_t)(*(uint32_t*)ptr);
  } else if (len == 8) {
    hash = (hash_t)(*(uint64_t*)ptr);
  } else {
    hash = (hash_t)ptr;
  }

  hash = (hash ^ (hash >> 31) ^ (hash >> 62)) * UINT64_C(0x319642b2d24d8ec3);
  hash = (hash ^ (hash >> 27) ^ (hash >> 54)) * UINT64_C(0x96de1b173f119089);
  hash = hash ^ (hash >> 30) ^ (hash >> 60);

  return hash;
}

ret_t __map_init(any_t _self, size_t ksize, size_t vsize, map_hash_fn fn) {
  map_t* self = as(_self, map_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(ksize == 0, -1);
  u_ret_if(vsize == 0, -1);

  self->len     = 0;
  self->ksize   = ksize;
  self->vsize   = vsize;
  self->hash_fn = fn != nullptr ? fn : map_mem_hash;

  vec_init(&self->hashs);

  arr_for(self->buckets, i) {
    __list_init(&self->buckets[i], sizeof(hash_t) + ksize + vsize);
  }

  return 0;
}

ret_t __map_clear(any_t _self) {
  map_t* self = as(_self, map_t*);

  u_ret_if(_self == nullptr, -1);

  self->len = 0;

  vec_clear(&self->hashs);

  arr_for(self->buckets, i) {
    __list_clear(&self->buckets[i]);
  }

  return 0;
}

ret_t __map_cleanup(any_t _self) {
  map_t* self = as(_self, map_t*);

  u_ret_if(_self == nullptr, -1);

  self->len     = 0;
  self->ksize   = 0;
  self->vsize   = 0;
  self->hash_fn = nullptr;

  vec_cleanup(&self->hashs);

  arr_for(self->buckets, i) {
    __list_cleanup(&self->buckets[i]);
  }

  return 0;
}

static hash_t* __map_node(any_t _self, any_t key, any_t val) {
  hash_t* hash = nullptr;
  map_t* self  = as(_self, map_t*);

  u_ret_if(_self == nullptr, nullptr);
  u_ret_if(key == nullptr, nullptr);
  u_ret_if(val == nullptr, nullptr);

  hash = u_talloc(sizeof(hash_t) + self->ksize + self->vsize, hash_t*);
  u_alloc_if(hash);

  memcpy(__map_key(self, hash), key, self->ksize);
  memcpy(__map_val(self, hash), val, self->vsize);
  *hash = self->hash_fn(key, self->ksize);

  return hash;

err:
  return nullptr;
}

ret_t __map_push(any_t _self, any_t key, any_t val) {
  hash_t* hash          = nullptr;
  list_t* list          = nullptr;
  map_t* self           = as(_self, map_t*);
  list_iter(hash_t)* it = nullptr;

  u_ret_if(_self == nullptr, -1);
  u_ret_if(key == nullptr, -1);
  u_ret_if(val == nullptr, -1);

  hash = __map_node(_self, key, val);
  u_alloc_if(hash);

  list = &self->buckets[(*hash) % U_MAP_BUCKETS_NUM];
  it   = __list_find(list, hash, fn_eq_use(map_hash_eq));

  if (it == nullptr) {
    /* add node */
    self->len++;
    __list_push(list, list->tail, hash);
  } else {
    /* change node */
    memcpy(__map_val(self, &it->it), val, self->vsize);
  }

  vec_push_b(&self->hashs, *hash);

  u_free(hash);

  return 0;

err:
  u_free_if(hash);

  return -2;
}

ret_t __map_pop(any_t _self, any_t key, any_t val) {
  size_t idx            = 0;
  hash_t hash           = {0};
  list_t* list          = nullptr;
  map_t* self           = as(_self, map_t*);
  list_iter(hash_t)* it = nullptr;

  u_ret_if(_self == nullptr, -1);
  u_ret_if(key == nullptr, -1);
  u_ret_if(val == nullptr, -1);

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % U_MAP_BUCKETS_NUM];
  u_goto_if(list->len == 0);

  it = __list_find(list, &hash, fn_eq_use(map_hash_eq));
  u_goto_if(it == nullptr);

  memcpy(val, __map_val(self, &it->it), self->vsize);

  idx = vec_find(&self->hashs, hash, fn_eq_use(map_node_eq));

  vec_erase(&self->hashs, idx);
  __list_erase(list, it);

  self->len--;

  return 0;

err:
  return -2;
}

ret_t __map_at(any_t _self, any_t key, any_t val) {
  hash_t hash           = {0};
  list_t* list          = nullptr;
  map_t* self           = as(_self, map_t*);
  list_iter(hash_t)* it = nullptr;

  u_ret_if(_self == nullptr, -1);
  u_ret_if(key == nullptr, -1);
  u_ret_if(val == nullptr, -1);

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % U_MAP_BUCKETS_NUM];
  u_goto_if(list->len == 0);

  it = __list_find(list, &hash, fn_eq_use(map_hash_eq));
  u_goto_if(it == nullptr);

  memcpy(val, __map_val(self, &it->it), self->vsize);

  return 0;

err:
  return -2;
}

ret_t __map_range(any_t _self, size_t idx, any_t key, any_t val) {
  hash_t hash           = {0};
  list_t* list          = nullptr;
  map_t* self           = as(_self, map_t*);
  list_iter(hash_t)* it = nullptr;

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx >= vec_len(&self->hashs), -1);
  u_ret_if(key == nullptr, -1);
  u_ret_if(val == nullptr, -1);

  hash = vec_at(&self->hashs, idx);

  list = &self->buckets[hash % U_MAP_BUCKETS_NUM];
  u_goto_if(list->len == 0);

  it = __list_find(list, &hash, fn_eq_use(map_hash_eq));
  u_goto_if(it == nullptr);

  memcpy(key, __map_key(self, &it->it), self->ksize);
  memcpy(val, __map_val(self, &it->it), self->vsize);

  return 0;

err:
  return -2;
}
