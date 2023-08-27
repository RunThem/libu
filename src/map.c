#include "map.h"

/* |    8    |     ...      |
 *    hash      key + val
 * */

/*************************************************************************************************
 * Private macro
 *************************************************************************************************/
#define __map_key(map, node) (any(node) + sizeof(map_node_t))
#define __map_val(map, node) (any(node) + sizeof(map_node_t) + (map)->ksize)

/*************************************************************************************************
 * Private function
 *************************************************************************************************/
static map_node_t* __map_node(any_t _self, any_t key, any_t val) {
  map_t* self      = as(_self, map_t*);
  map_node_t* node = nullptr;

  node = u_talloc(sizeof(map_node_t) + self->ksize + self->vsize, map_node_t*);
  u_alloc_if(node);

  node->hash = self->hash_fn(key, self->ksize);
  memcpy(__map_key(self, node), key, self->ksize);
  memcpy(__map_val(self, node), val, self->vsize);

  return node;

err:
  return nullptr;
}

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

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __map_new(size_t ksize, size_t vsize, map_hash_fn hash_fn, map_eq_fn eq_fn) {
  map_t* self = nullptr;

  u_ret_if(ksize == 0, nullptr);
  u_ret_if(vsize == 0, nullptr);
  u_ret_if(hash_fn == nullptr, nullptr);
  u_ret_if(eq_fn == nullptr, nullptr);

  self = u_zalloc(sizeof(map_t) + ksize + vsize);
  u_alloc_if(self);

  self->hash_fn = hash_fn;
  self->eq_fn   = eq_fn;
  self->ksize   = ksize;
  self->vsize   = vsize;
  self->len     = 0;
  self->lhead   = nullptr;
  self->ltail   = nullptr;

  return self;

err:
  return nullptr;
}

ret_t __map_init(any_t* _self, size_t ksize, size_t vsize, map_hash_fn hash_fn, map_eq_fn eq_fn) {
  u_ret_if(_self == nullptr, -1);
  u_ret_if(*_self != nullptr, -1);

  *_self = __map_new(ksize, vsize, hash_fn, eq_fn);
  u_alloc_if(*_self);

  return 0;

err:
  return -2;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
ret_t __map_clear(any_t _self) {
  map_t* self      = as(_self, map_t*);
  map_node_t* itor = nullptr;
  map_node_t* list = nullptr;

  u_ret_if(_self == nullptr, -1);

  arr_for(self->buckets, i) {
    list = &self->buckets[i];

    for (itor = list->next; itor != nullptr; itor = list) {
      list = itor->next;

      u_free(itor);
    }
  }

  self->len = 0;

  return 0;
}

ret_t __map_cleanup(any_t _self) {
  map_t* self = as(_self, map_t*);

  u_ret_if(_self == nullptr, -1);

  __map_clear(self);

  u_free(self);

  return 0;
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
inline size_t __map_ksize(any_t _self) {
  map_t* self = as(_self, map_t*);

  u_ret_if(_self == nullptr, -1);

  return self->ksize;
}

inline size_t __map_vsize(any_t _self) {
  map_t* self = as(_self, map_t*);

  u_ret_if(_self == nullptr, -1);

  return self->vsize;
}

inline size_t __map_len(any_t _self) {
  map_t* self = as(_self, map_t*);

  u_ret_if(_self == nullptr, 0);

  return self->len;
}

inline bool __map_empty(any_t _self) {
  return __map_len(_self) != 0;
}

ret_t __map_at(any_t _self, any_t key, any_t val) {
  map_t* self      = as(_self, map_t*);
  map_node_t* list = nullptr;
  map_node_t* itor = nullptr;
  hash_t hash      = 0;

  u_ret_if(_self == nullptr, -1);
  u_ret_if(key == nullptr, -1);
  u_ret_if(val == nullptr, -1);

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % U_MAP_BUCKETS_NUM];

  for (itor = list->next; itor != nullptr; itor = itor->next) {
    if (hash == itor->hash && self->eq_fn(__map_key(self, itor), key)) {
      break;
    }
  }

  u_goto_if(itor == nullptr);

  memcpy(key, __map_key(self, itor), self->ksize);
  memcpy(val, __map_val(self, itor), self->vsize);

  return 0;

err:
  return -2;
}

ret_t __map_push(any_t _self, any_t key, any_t val) {
  map_t* self      = as(_self, map_t*);
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  map_node_t* itor = nullptr;
  hash_t hash      = 0;

  u_ret_if(_self == nullptr, -1);
  u_ret_if(key == nullptr, -1);
  u_ret_if(val == nullptr, -1);

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % U_MAP_BUCKETS_NUM];

  for (itor = list->next; itor != nullptr; itor = itor->next) {
    if (hash == itor->hash && self->eq_fn(__map_key(self, itor), key)) {
      break;
    }
  }

  /* node already exists */
  if (itor != nullptr) {
    memcpy(__map_val(self, itor), val, self->vsize);
  } else {
    node = __map_node(_self, key, val);
    u_alloc_if(node);

    node->next = list->next;
    list->next = node;

    node->lprev = self->ltail;
    node->lnext = nullptr;

    if (self->ltail != nullptr) {
      self->ltail->lnext = node;
    }

    self->ltail = node;

    if (self->lhead == nullptr) {
      self->lhead = node;
    }

    self->len++;
  }

  return 0;

err:
  u_free_if(node);

  return -2;
}

ret_t __map_pop(any_t _self, any_t key, any_t val) {
  map_t* self      = as(_self, map_t*);
  map_node_t* list = nullptr;
  map_node_t* itor = nullptr;
  map_node_t* prev = nullptr;
  hash_t hash      = 0;

  u_ret_if(_self == nullptr, -1);
  u_ret_if(key == nullptr, -1);
  u_ret_if(val == nullptr, -1);

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % U_MAP_BUCKETS_NUM];

  for (itor = list->next; itor != nullptr; prev = itor, itor = itor->next) {
    if (hash == itor->hash && self->eq_fn(__map_key(self, itor), key)) {
      break;
    }
  }

  u_goto_if(itor == nullptr);

  if (itor == list->next) {
    list->next = itor->next;
  } else {
    prev->next = itor->next;
  }

  if (self->lhead == itor) {
    self->lhead = itor->lnext;
  }

  if (self->ltail == itor) {
    self->ltail = itor->lprev;
  }

  if (itor->lprev != nullptr) {
    itor->lprev->lnext = itor->lnext;
  }

  if (itor->lnext != nullptr) {
    itor->lnext->lprev = itor->lprev;
  }

  self->len--;

  memcpy(key, __map_key(self, itor), self->ksize);
  memcpy(val, __map_val(self, itor), self->vsize);

  u_free(itor);

  return 0;

err:
  return -2;
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
ret_t __map_range(any_t _self, any_t key, any_t val) {
  map_t* self = as(_self, map_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(key == nullptr, -1);
  u_ret_if(val == nullptr, -1);

  if (self->itor == nullptr) {
    self->itor = self->lhead;
  } else {
    self->itor = self->itor->lnext;
  }

  u_goto_if(self->itor == nullptr);

  memcpy(key, __map_key(self, self->itor), self->ksize);
  memcpy(val, __map_val(self, self->itor), self->vsize);

  return 0;

err:

  return -2;
}
