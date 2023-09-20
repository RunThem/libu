#include "core/map.h"

/*
 *
 * 0 +---------+
 *   |  ksize  |
 * 8 +---------+
 *   |  visze  |
 * 16+---------+
 *   |   len   |
 * 24+---------+
 *   | hash_fn |
 * 32+---------+
 *   |  eq_fn  |
 * 40+---------+
 *   |   bs    |                     0      8      16     24     32     40     48
 * 48+---------+                     +------+------+------+------+------+------+   +------+------+
 *   | buckets | ------------------> | next | cnt  | next | cnt  | next | cnt  |...| next | cnt  |
 * 56+---------+ -----------+        +------+------+------+------+------+------+   +------+------+
 *   |         |            |           |
 *   |   key   |sizeof(K)   |           v
 *   |         |            v        +------+------+----------------+
 *   +---------+      return point   | next | hash |      ...       |
 *   |  *val   |sizeof(V*)           +------+------+----------+-----+
 *   +---------+                        |            sizeof(k + V)
 *   |         |                        v
 *   |   val   |sizeof(V)            +------+------+----------------+
 *   |         |                     | next | hash |      ...       |
 *   +---------+                     +------+------+----------+-----+
 *                                      |            sizeof(k + V)
 *                                      v
 *                                   +------+------+----------------+
 *                                   | next | hash |      ...       |
 *                                   +------+------+----------+-----+
 *                                      |            sizeof(k + V)
 *                                      v
 *                                    .....
 *                                      |
 *                                      v
 *                                   +------+------+----------------+
 *                                   | next | hash |      ...       |
 *                                   +------+------+----------+-----+
 *                                      |            sizeof(k + V)
 *                                      v
 *                                   +------+------+------+------+------+------+   +------+------+
 *                                   | NULL | idx  | NULL | idx  | NULL | idx  |...| NULL | 0x00 |
 *                                   +------+------+------+------+------+------+   +------+------+
 *                                             |
 *                                             V
 *                                     next bucket index
 *
 *
 */

/*************************************************************************************************
 * Private
 *************************************************************************************************/
static size_t bucket_sizes[] = {
    61,         127,        251,        509,        1021,        2039,       4093,      8191,
    16381,      32749,      65521,      131071,     262139,      524287,     1048573,   2097143,
    4194301,    8388593,    16777213,   33554393,   67108859,    134217689,  268435399, 536870909,
    1073741789, 2147483647, 4294967291, 8589934583, 17179869143, 34359738337};

typedef struct map_node_t map_node_t;
struct map_node_t {
  map_node_t* next;
  u_hash_t hash;
};

#define map_key(map, node) (any(node) + sizeof(map_node_t))
#define map_val(map, node) (any(node) + sizeof(map_node_t) + (map)->ksize)

typedef struct {
  size_t ksize;
  size_t vsize;
  size_t len;

  u_map_hash_fn hash_fn;
  u_map_eq_fn eq_fn;

  size_t bs; /* buckets size */
  map_node_t* buckets;
} map_t;

#undef self_of
#define self_of(self) (assert((self) != nullptr), as((self) - sizeof(map_t), map_t*))

/* fnv 64-bit hash function */
static u_hash_t map_mem_hash(const uint8_t* ptr, size_t len) {
  u_hash_t hash = 14695981039346656037U;

  for (size_t i = 0; i < len; ++i) {
    hash *= 1099511628211U;
    hash ^= (uint64_t)ptr[i];
  }

  return hash;
}

/* int hash function */
static u_hash_t map_int_hash(const uint8_t* ptr, size_t len) {
  u_hash_t hash = 0;

  if (len == 1) {
    hash = (u_hash_t)(*(uint8_t*)ptr);
  } else if (len == 2) {
    hash = (u_hash_t)(*(uint16_t*)ptr);
  } else if (len == 4) {
    hash = (u_hash_t)(*(uint32_t*)ptr);
  } else if (len == 8) {
    hash = (u_hash_t)(*(uint64_t*)ptr);
  } else {
    hash = (u_hash_t)ptr;
  }

  hash = (hash ^ (hash >> 31) ^ (hash >> 62)) * UINT64_C(0x319642b2d24d8ec3);
  hash = (hash ^ (hash >> 27) ^ (hash >> 54)) * UINT64_C(0x96de1b173f119089);
  hash = hash ^ (hash >> 30) ^ (hash >> 60);

  return hash;
}

static map_node_t* __map_node(map_t* self, any_t key, any_t val) {
  map_node_t* node = nullptr;

  node = u_talloc(sizeof(map_node_t) + self->ksize + self->vsize, map_node_t*);
  u_mem_if(node);

  node->hash = self->hash_fn(key, self->ksize);
  memcpy(map_key(self, node), key, self->ksize);
  memcpy(map_val(self, node), val, self->vsize);

  return node;

err:
  return nullptr;
}

static map_node_t* __map_buckets(size_t bs) {
  map_node_t* buckets = nullptr;

  buckets = u_calloc(bs * 2, sizeof(map_node_t));
  u_mem_if(buckets);

  for (size_t i = 0, j = bs; i < bs; i++, j++) {
    buckets[i].next = &buckets[j];
    buckets[i].hash = 0;

    buckets[j].next = nullptr;
    buckets[j].hash = i + 1;
  }

  buckets[bs * 2 - 1].hash = 0;

  return buckets;

err:
  return nullptr;
}

static map_node_t* __map_next(map_t* self, map_node_t* node) {
  size_t idx = 0;

  if (node != nullptr) {
    node = node->next;

    if (node->next != nullptr) {
      return node;
    }

    if (node->hash == 0) {
      return nullptr;
    }

    idx  = node->hash;
    node = nullptr;
  }

  for (size_t i = idx; i < bucket_sizes[self->bs]; i++) {
    if (self->buckets[i].hash != 0) {
      node = self->buckets[i].next;
      break;
    }
  }

  return node;
}

static bool __map_resize(map_t* self) {
  map_node_t* nbuckets = nullptr;
  map_node_t* node     = nullptr;
  map_node_t* tmp      = nullptr;
  map_node_t* list     = nullptr;
  map_node_t* nlist    = nullptr;
  size_t bs            = 0;

  bs       = bucket_sizes[self->bs + 1];
  nbuckets = __map_buckets(bs);
  u_mem_if(nbuckets);

  for (size_t i = 0; i < bucket_sizes[self->bs]; i++) {
    if (self->buckets[i].hash == 0) {
      continue;
    }

    for (node = self->buckets[i].next, list = node->next; node->next != nullptr;
         node = list, list = list->next) {
      nlist = &nbuckets[node->hash % bs];

      node->next  = nlist->next;
      nlist->next = node;

      nlist->hash++;
    }
  }

  self->bs++;

  u_free(self->buckets);
  self->buckets = nbuckets;

  return true;

err:
  return false;
}

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __map_new(size_t ksize, size_t vsize, u_map_eq_fn eq_fn, enum u_map_hash_fn hash_fn) {
  map_t* self = nullptr;

  u_assert(ksize == 0);
  u_assert(vsize == 0);
  u_assert(eq_fn == nullptr);
  u_assert(hash_fn >= U_MAP_HASH_FN_MAX);

  self = u_zalloc(sizeof(map_t) + ksize + sizeof(any_t) + vsize);
  u_mem_if(self);

  self->buckets = __map_buckets(bucket_sizes[0]);
  u_mem_if(self->buckets);

  self->ksize = ksize;
  self->vsize = vsize;
  self->len   = 0;
  self->eq_fn = eq_fn;
  self->bs    = 0;

  if (hash_fn == U_MAP_FNV_64_HASH_FN) {
    self->hash_fn = map_mem_hash;
  } else if (hash_fn == U_MAP_INT_HASH_FN) {
    self->hash_fn = map_int_hash;
  }

  return self + 1;

err:
  u_free_if(self);

  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
ret_t __map_clear(any_t _self) {
  map_t* self      = self_of(_self);
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;

  for (size_t i = 0; i < bucket_sizes[self->bs]; i++) {
    list = self->buckets[i].next;

    for (node = list, list = list->next; node->next != nullptr; node = list, list = list->next) {
      u_free(node);
    }

    self->buckets[i].next = node;
    self->buckets[i].hash = 0;
  }

  self->len = 0;

  return 0;
}

ret_t __map_cleanup(any_t _self) {
  map_t* self = self_of(_self);

  __map_clear(_self);
  u_free(self->buckets);
  u_free(self);

  return 0;
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __map_ksize(any_t _self) {
  return self_of(_self)->ksize;
}

size_t __map_vsize(any_t _self) {
  return self_of(_self)->vsize;
}

size_t __map_len(any_t _self) {
  return self_of(_self)->len;
}

bool __map_empty(any_t _self) {
  return self_of(_self)->len == 0;
}

bool __map_exist(any_t _self) {
  map_t* self      = self_of(_self);
  any_t key        = nullptr;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  u_hash_t hash    = 0;

  key = _self + sizeof(any_t);

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % bucket_sizes[self->bs]];

  for (node = list->next; node->next != nullptr; node = node->next) {
    if (hash == node->hash && self->eq_fn(map_key(self, node), key)) {
      break;
    }
  }

  return node->next != nullptr;
}

any_t __map_at(any_t _self) {
  map_t* self      = self_of(_self);
  any_t key        = nullptr;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  u_hash_t hash    = 0;

  key = _self + sizeof(any_t);

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % bucket_sizes[self->bs]];

  for (node = list->next; node->next != nullptr; node = node->next) {
    if (hash == node->hash && self->eq_fn(map_key(self, node), key)) {
      break;
    }
  }

  u_mem_if(node->next);

  return map_val(self, node);

err:
  return nullptr;
}

void __map_pop(any_t _self) {
  map_t* self      = self_of(_self);
  any_t key        = nullptr;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  map_node_t* prev = nullptr;
  u_hash_t hash    = 0;

  key = _self + sizeof(any_t);

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % bucket_sizes[self->bs]];

  for (prev = list, node = list->next; node->next != nullptr; prev = node, node = node->next) {
    if (hash == node->hash && self->eq_fn(map_key(self, node), key)) {
      break;
    }
  }

  if (node->next != nullptr) {
    prev->next = node->next;

    list->hash--;
    self->len--;

    u_free(node);
  }
}

ret_t __map_push(any_t _self) {
  map_t* self      = self_of(_self);
  any_t key        = nullptr;
  any_t val        = nullptr;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  u_hash_t hash    = 0;
  size_t idx       = 0;

  key = _self + sizeof(any_t);
  val = key + self->ksize;

  if (self->len >= as(U_MAP_RESIZE_RADIO * bucket_sizes[self->bs], size_t)) {
    __map_resize(self);
  }

  hash = self->hash_fn(key, self->ksize);
  list = &self->buckets[hash % bucket_sizes[self->bs]];

  for (node = list->next; node->next != nullptr; node = node->next) {
    if (hash == node->hash && self->eq_fn(map_key(self, node), key)) {
      break;
    }
  }

  /* node already exists */
  if (node->next != nullptr) {
    memcpy(map_val(self, node), val, self->vsize);
  } else {
    node = __map_node(self, key, val);
    u_mem_if(node);

    node->next = list->next;
    list->next = node;

    list->hash++;
    self->len++;
  }

  return 0;

err:
  u_free_if(node);

  return -2;
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __map_range(any_t _self) {
  map_t* self      = self_of(_self);
  map_node_t* node = nullptr;
  any_t key        = nullptr;
  any_t* val       = nullptr;
  size_t i         = 0;

  u_ret_if(self->len == 0, false);

  key = _self + sizeof(any_t);
  val = _self;

  node = __map_next(self, (*val == nullptr) ? nullptr : (*val) - self->ksize - sizeof(map_node_t));
  if (node == nullptr) {
    bzero(key, self->ksize);
    *val = key + self->ksize;

    return false;
  }

  memcpy(key, map_key(self, node), self->ksize);
  *val = map_val(self, node);

  return true;
}

#ifndef NDEBUG
void __map_debug(any_t _self) {
  map_t* self      = self_of(_self);
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;

  inf("map %p {ksize(%zu), vsize(%zu), len(%zu), buckets_size(%zu)}",
      self,
      self->ksize,
      self->vsize,
      self->len,
      bucket_sizes[self->bs]);

  for (size_t i = 0; i < bucket_sizes[self->bs]; i++) {
    list = &self->buckets[i];

    if (list->hash == 0) {
      continue;
    }

    inf("buckets[%zu] @%zu", i, list->hash);

    for (node = list->next; node->next != nullptr; node = node->next) {
      __prt("    %p {%p, %d, '%c'}\n",
            node,
            node->next,
            *as(map_key(self, node), int*),
            *as(map_val(self, node), char*));
    }

    __prt("    %p {%p, %zu}\n", node, node->next, node->hash);
  }
}
#endif
