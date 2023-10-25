#include <u/core/map.h>

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
 *   |  itor   |
 * 48+---------+
 *   |  b_idx  |                     0      8      16     24     32     40     48
 * 56+---------+                     +------+------+------+------+------+------+   +------+------+
 *   | buckets | ------------------> | next | cnt  | next | cnt  | next | cnt  |...| next | cnt  |
 * 64+---------+ -----------+        +------+------+------+------+------+------+   +------+------+
 *   |         |            |           |
 *   |   key   |sizeof(K)   |           v
 *   |         |            v        +------+------+----------------+
 *   +---------+      return point   | next | hash |      ...       |
 *   |         |                     +------+------+----------+-----+
 *   |   val   |sizeof(V)               |            sizeof(k + V)
 *   |         |                        v
 *   +---------+                     +------+------+----------------+
 *                                   | next | hash |      ...       |
 *                                   +------+------+----------+-----+
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

#undef key
#define key(node) (any(node) + sizeof(map_node_t))

#undef val
#define val(node) (any(node) + sizeof(map_node_t) + self->ksize)

typedef struct map_t map_t;
struct map_t {
  size_t ksize;
  size_t vsize;
  size_t len;

  u_map_hash_fn hash_fn;
  u_map_eq_fn eq_fn;

  map_node_t* itor;
  map_node_t* free;

  size_t bucket_idx;
  map_node_t* buckets;
};

#undef selfof
#define selfof(self) (assert((self) != nullptr), as((self) - sizeof(map_t), map_t*))

/* fnv 64-bit hash function */
static u_hash_t map_mem_hash(const u8_t* ptr, size_t len) {
  u_hash_t hash = 14695981039346656037U;

  for (size_t i = 0; i < len; ++i) {
    hash *= 1099511628211U;
    hash ^= (u64_t)ptr[i];
  }

  return hash;
}

/* int hash function */
static u_hash_t map_int_hash(const u8_t* ptr, size_t len) {
  u_hash_t hash = 0;

  if (len == 1) {
    hash = (u_hash_t)(*(u8_t*)ptr);
  } else if (len == 2) {
    hash = (u_hash_t)(*(u16_t*)ptr);
  } else if (len == 4) {
    hash = (u_hash_t)(*(u32_t*)ptr);
  } else if (len == 8) {
    hash = (u_hash_t)(*(u64_t*)ptr);
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

  if (self->free != nullptr) {
    node       = self->free;
    self->free = node->next;
  } else {
    node = u_zalloc(sizeof(map_node_t) + self->ksize + self->vsize);
    u_mem_if(node);
  }

  node->hash = self->hash_fn(key, self->ksize);
  memcpy(key(node), key, self->ksize);
  memcpy(val(node), val, self->vsize);

  return node;

err:
  return nullptr;
}

static map_node_t* __map_find(map_t* self, map_node_t** list, map_node_t** prev, any_t key) {
  u_hash_t hash    = 0;
  map_node_t* node = nullptr;

  hash  = self->hash_fn(key, self->ksize);
  *list = &self->buckets[hash % bucket_sizes[self->bucket_idx]];

  for (*prev = *list, node = (*list)->next; node->next != nullptr;
       *prev = node, node = node->next) {

    if (hash == node->hash && self->eq_fn(key(node), key)) {
      break;
    }
  }

  return node;
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

static void __map_next(map_t* self) {
  size_t idx = 0;

  if (self->itor != nullptr) {
    self->itor = self->itor->next;

    /* ok */
    u_nret_if(self->itor->next != nullptr);

    /* end */
    u_if(self->itor->hash == 0) {
      self->itor = nullptr;
      return;
    }

    idx        = self->itor->hash;
    self->itor = nullptr;
  }

  for (size_t i = idx; i < bucket_sizes[self->bucket_idx]; i++) {
    if (self->buckets[i].hash != 0) {
      self->itor = self->buckets[i].next;
      break;
    }
  }
}

static bool __map_resize(map_t* self) {
  map_node_t* nbuckets = nullptr;
  map_node_t* node     = nullptr;
  map_node_t* tmp      = nullptr;
  map_node_t* list     = nullptr;
  map_node_t* nlist    = nullptr;
  size_t bucket_idx    = 0;

  bucket_idx = bucket_sizes[self->bucket_idx + 1];
  nbuckets   = __map_buckets(bucket_idx);
  u_mem_if(nbuckets);

  for (size_t i = 0; i < bucket_sizes[self->bucket_idx]; i++) {
    if (self->buckets[i].hash == 0) {
      continue;
    }

    for (node = self->buckets[i].next, list = node->next; node->next != nullptr;
         node = list, list = list->next) {
      nlist = &nbuckets[node->hash % bucket_idx];

      node->next  = nlist->next;
      nlist->next = node;

      nlist->hash++;
    }
  }

  self->bucket_idx++;

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

  u_check_ret(ksize == 0, nullptr);
  u_check_ret(vsize == 0, nullptr);
  u_check_ret(eq_fn == nullptr, nullptr);
  u_check_ret(hash_fn >= U_MAP_HASH_FN_MAX, nullptr);

  self = u_zalloc(sizeof(map_t) + ksize + vsize);
  u_mem_if(self);

  self->buckets = __map_buckets(bucket_sizes[0]);
  u_mem_if(self->buckets);

  self->ksize      = ksize;
  self->vsize      = vsize;
  self->len        = 0;
  self->eq_fn      = eq_fn;
  self->bucket_idx = 0;

  if (hash_fn == U_MAP_FNV_64_HASH_FN) {
    self->hash_fn = map_mem_hash;
  } else if (hash_fn == U_MAP_INT_HASH_FN) {
    self->hash_fn = map_int_hash;
  }

  infln("map new(ksize(%zu), vsize(%zu), eq_fn(%p), hash_fn(%u))", ksize, vsize, eq_fn, hash_fn);

  return self + 1;

err:
  u_free_if(self);

  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __map_clear(any_t _self) {
  map_t* self      = selfof(_self);
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;

  for (size_t i = 0; i < bucket_sizes[self->bucket_idx]; i++) {
    list = self->buckets[i].next;

    for (node = list, list = list->next; node->next != nullptr; node = list, list = list->next) {
      u_free(node);
    }

    self->buckets[i].next = node;
    self->buckets[i].hash = 0;
  }

  while (self->free != nullptr) {
    node       = self->free;
    self->free = node->next;

    u_free(node);
  }

  self->len = 0;
}

void __map_cleanup(any_t _self) {
  map_t* self = selfof(_self);

  __map_clear(_self);

  u_free(self->buckets);
  u_free(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __map_ksize(any_t _self) {
  return selfof(_self)->ksize;
}

size_t __map_vsize(any_t _self) {
  return selfof(_self)->vsize;
}

size_t __map_len(any_t _self) {
  return selfof(_self)->len;
}

bool __map_empty(any_t _self) {
  return selfof(_self)->len == 0;
}

bool __map_exist(any_t _self) {
  map_t* self      = selfof(_self);
  any_t key        = _self;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  map_node_t* prev = nullptr;
  u_hash_t hash    = 0;

  node = __map_find(self, &list, &prev, key);
  noused(prev);

  return node->next != nullptr;
}

void __map_re(any_t _self) {
  map_t* self      = selfof(_self);
  any_t key        = _self;
  any_t val        = _self + self->ksize;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  map_node_t* prev = nullptr;
  u_hash_t hash    = 0;

  node = __map_find(self, &list, &prev, key);
  noused(prev);

  u_nret_if(node->next == nullptr, "node not exists.");

  memcpy(val(node), val, self->vsize);
}

void __map_at(any_t _self) {
  map_t* self      = selfof(_self);
  any_t key        = _self;
  any_t val        = _self + self->ksize;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  map_node_t* prev = nullptr;
  u_hash_t hash    = 0;

  node = __map_find(self, &list, &prev, key);
  noused(prev);

  u_if(node->next == nullptr, "node not exists") {
    bzero(val, self->vsize);
  }
  else {
    memcpy(val, val(node), self->vsize);
  }
}

void __map_pop(any_t _self) {
  map_t* self      = selfof(_self);
  any_t key        = _self;
  any_t val        = _self + self->ksize;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  map_node_t* prev = nullptr;
  u_hash_t hash    = 0;

  bzero(val, self->vsize);

  node = __map_find(self, &list, &prev, key);

  if (node->next != nullptr) {
    prev->next = node->next;

    list->hash--;
    self->len--;

    memcpy(val, val(node), self->vsize);

    node->next = self->free;
    self->free = node;
  }
}

ret_t __map_push(any_t _self) {
  map_t* self      = selfof(_self);
  any_t key        = _self;
  any_t val        = _self + self->ksize;
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;
  map_node_t* prev = nullptr;
  u_hash_t hash    = 0;
  size_t idx       = 0;

  if (self->len >= as(U_MAP_RESIZE_RADIO * bucket_sizes[self->bucket_idx], size_t)) {
    __map_resize(self);
  }

  node = __map_find(self, &list, &prev, key);
  noused(prev);

  /* node already exists */
  if (node->next != nullptr) {
    memcpy(val(node), val, self->vsize);
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
void __map_range_init(any_t _self) {
  selfof(_self)->itor = nullptr;
}

bool __map_range(any_t _self) {
  map_t* self = selfof(_self);
  any_t key   = _self;
  any_t val   = _self + self->ksize;

  u_ret_if(self->len == 0, false);

  __map_next(self);
  u_ret_if(self->itor == nullptr, false);

  memcpy(key, key(self->itor), self->ksize + self->vsize);

  return true;
}

#ifndef NDEBUG
void __map_debug(any_t _self) {
  map_t* self      = selfof(_self);
  map_node_t* node = nullptr;
  map_node_t* list = nullptr;

  fprintf(stderr,
          "map %p {ksize(%zu), vsize(%zu), len(%zu), buckets_size(%zu)}\n",
          self,
          self->ksize,
          self->vsize,
          self->len,
          bucket_sizes[self->bucket_idx]);

  for (size_t i = 0; i < bucket_sizes[self->bucket_idx]; i++) {
    list = &self->buckets[i];

    if (list->hash == 0) {
      continue;
    }

    fprintf(stderr, "buckets[%zu] @%zu", i, list->hash);

    for (node = list->next; node->next != nullptr; node = node->next) {
      fprintf(stderr,
              "    %p {%p, %d, '%c'}\n",
              node,
              node->next,
              *as(key(node), int*),
              *as(val(node), char*));
    }

    fprintf(stderr, "    %p {%p, %zu}\n", node, node->next, node->hash);
  }
}
#endif
