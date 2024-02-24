#include <u/core.h>

/***************************************************************************************************
 * Private
 **************************************************************************************************/
#define U_RESIZE_RADIO 0.75

static size_t bucket_sizes[] = {
    7,         61,         127,        251,        509,        1021,        2039,        4093,
    8191,      16381,      32749,      65521,      131071,     262139,      524287,      1048573,
    2097143,   4194301,    8388593,    16777213,   33554393,   67108859,    134217689,   268435399,
    536870909, 1073741789, 2147483647, 4294967291, 8589934583, 17179869143, 34359738337,
};

/***************************************************************************************************
 * Tyoe
 **************************************************************************************************/
typedef u64_t u_hash_t;

typedef struct node_t node_t;
struct node_t {
  node_t* next;
  u_hash_t hash;
};

typedef struct tbl_t tbl_t;
struct tbl_t {
  bool flags[4];
  size_t ksize;
  size_t vsize;
  size_t len;

  node_t* iter;
  node_t* free_nodes;

  i8_t bucket_idx;
  node_t* buckets;
};

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#undef key
#define key(node) (any(node) + sizeof(node_t))

#undef val
#define val(node) (any(node) + sizeof(node_t) + self->ksize)

/***************************************************************************************************
 * Function
 **************************************************************************************************/
/* [idx, len] { 0xffff } { 0xfffe } */
static void tbl_debug(tbl_t* self) {
  infln("ksize(%zu), vsize(%zu), len(%zu), buckets(%zu)",
        self->ksize,
        self->vsize,
        self->len,
        bucket_sizes[self->bucket_idx]);

  for (size_t i = 0; i < bucket_sizes[self->bucket_idx]; i++) {
    print("[%zu, %zu] ", i, self->buckets[i].hash);

    for (node_t* node = &self->buckets[i]; node != nullptr; node = node->next) {
      print("{ %p } ", node);
    }

    println();
  }
}

/* fnv 64-bit hash function */
static u_hash_t hash_fnv64bit(const u8_t* ptr, size_t len) {
  u_hash_t hash = 14695981039346656037U;

  for (size_t i = 0; i < len; ++i) {
    hash *= 1099511628211U;
    hash ^= (u64_t)ptr[i];
  }

  return hash;
}

static node_t* tbl_make_node(tbl_t* self, any_t key, any_t val) {
  node_t* node = nullptr;

  if (self->free_nodes != nullptr) {
    node             = self->free_nodes;
    self->free_nodes = node->next;
  } else {
    node = u_zalloc(sizeof(node_t) + self->ksize + self->vsize);
    u_mem_if(node);
  }

  node->hash = hash_fnv64bit(key, self->ksize);
  memcpy(key(node), key, self->ksize);
  memcpy(val(node), val, self->vsize);

  return node;

err:
  return nullptr;
}

static node_t* tbl_find(tbl_t* self, node_t* idx[2], any_t key) {
  u_hash_t hash = 0;
  node_t* node  = nullptr;

  hash   = hash_fnv64bit(key, self->ksize);
  idx[0] = &self->buckets[hash % bucket_sizes[self->bucket_idx]];

  for (idx[1] = idx[0], node = (idx[0])->next; node->next != nullptr;
       idx[1] = node, node = node->next) {

    if (hash == node->hash && memcmp(key(node), key, self->ksize) == 0) {
      break;
    }
  }

  return node;
}

static node_t* tbl_make_buckets(size_t bs) {
  node_t* buckets = nullptr;

  buckets = u_calloc(bs * 2, sizeof(node_t));
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

static void tbl_next(tbl_t* self) {
  size_t idx = 0;

  if (self->iter != nullptr) {
    self->iter = self->iter->next;

    /* ok */
    u_nret_if(self->iter->next != nullptr);

    /* end */
    u_if(self->iter->hash == 0) {
      self->iter = nullptr;
      return;
    }

    idx        = self->iter->hash;
    self->iter = nullptr;
  }

  for (size_t i = idx; i < bucket_sizes[self->bucket_idx]; i++) {
    if (self->buckets[i].hash != 0) {
      self->iter = self->buckets[i].next;
      break;
    }
  }
}

static bool tbl_resize(tbl_t* self) {
  node_t* nbuckets  = nullptr;
  node_t* node      = nullptr;
  node_t* tmp       = nullptr;
  node_t* list      = nullptr;
  node_t* nlist     = nullptr;
  size_t bucket_idx = 0;

  bucket_idx = bucket_sizes[self->bucket_idx + 1];
  nbuckets   = tbl_make_buckets(bucket_idx);
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

u_tbl_t tbl_new(size_t ksize, size_t vsize) {
  tbl_t* self = nullptr;

  u_check_ret(ksize == 0, nullptr);
  u_check_ret(vsize == 0, nullptr);

  self = u_zalloc(sizeof(tbl_t) + ksize + vsize);
  u_mem_if(self);

  self->buckets = tbl_make_buckets(bucket_sizes[0]);
  u_mem_if(self->buckets);

  self->ksize      = ksize;
  self->vsize      = vsize;
  self->len        = 0;
  self->bucket_idx = 0;

  infln("tbl new(ksize(%zu), vsize(%zu))", ksize, vsize);

  return as(self, u_tbl_t);

err:
  u_free_if(self);

  return nullptr;
}

void tbl_clear(u_tbl_t _self) {
  tbl_t* self  = as(_self, tbl_t*);
  node_t* node = nullptr;
  node_t* list = nullptr;

  u_check_nret(self == nullptr);

  for (size_t i = 0; i < bucket_sizes[self->bucket_idx]; i++) {
    list = self->buckets[i].next;

    for (node = list, list = list->next; node->next != nullptr; node = list, list = list->next) {
      u_free(node);
    }

    self->buckets[i].next = node;
    self->buckets[i].hash = 0;
  }

  while (self->free_nodes != nullptr) {
    node             = self->free_nodes;
    self->free_nodes = node->next;

    u_free(node);
  }

  self->len = 0;
}

void tbl_cleanup(u_tbl_t _self) {
  tbl_t* self = as(_self, tbl_t*);

  u_check_nret(self == nullptr);

  tbl_clear(_self);

  u_free_if(self->buckets);
  u_free_if(self);
}

size_t tbl_len(u_tbl_t _self) {
  tbl_t* self = as(_self, tbl_t*);

  u_check_ret(self == nullptr, 0);

  return self->len;
}

bool tbl_exist(u_tbl_t _self, any_t key) {
  tbl_t* self    = as(_self, tbl_t*);
  node_t* node   = nullptr;
  node_t* idx[2] = {};

  u_check_ret(self == nullptr, false);
  u_check_ret(key == nullptr, false);

  node = tbl_find(self, idx, key);

  return node->next != nullptr;
}

void tbl_re(u_tbl_t _self, any_t key, any_t val) {
  tbl_t* self    = as(_self, tbl_t*);
  node_t* node   = nullptr;
  node_t* idx[2] = {};

  u_check_nret(self == nullptr);
  u_check_nret(key == nullptr);
  u_check_nret(val == nullptr);

  node = tbl_find(self, idx, key);
  u_nret_if(node->next == nullptr, "node not exists.");

  if (self->vsize != 0) {
    memcpy(val(node), val, self->vsize);
  } else {
    memcpy(key(node), key, self->ksize);
  }
}

void tbl_at(u_tbl_t _self, any_t key, any_t val) {
  tbl_t* self    = as(_self, tbl_t*);
  node_t* node   = nullptr;
  node_t* idx[2] = {};

  u_check_nret(self == nullptr);
  u_check_nret(key == nullptr);
  u_check_nret(val == nullptr);

  node = tbl_find(self, idx, key);
  u_if(node->next == nullptr, "node not exists") {
    bzero(val, self->vsize);
  }
  else {
    memcpy(val, val(node), self->vsize);
  }
}

void tbl_pop(u_tbl_t _self, any_t key, any_t val) {
  tbl_t* self    = as(_self, tbl_t*);
  node_t* node   = nullptr;
  node_t* idx[2] = {};

  u_check_nret(self == nullptr);
  u_check_nret(key == nullptr);
  u_check_nret(val == nullptr);

  node = tbl_find(self, idx, key);
  if (node->next != nullptr) {
    idx[0]->next = node->next;
    idx[1]->hash--;

    self->len--;

    memcpy(val, val(node), self->vsize);

    node->next       = self->free_nodes;
    self->free_nodes = node;
  }

  bzero(val, self->vsize);
}

void tbl_put(u_tbl_t _self, any_t key, any_t val) {
  tbl_t* self    = as(_self, tbl_t*);
  node_t* node   = nullptr;
  node_t* idx[2] = {};
  ret_t code     = 0;

  u_check_nret(self == nullptr);
  u_check_nret(key == nullptr);
  u_check_nret(val == nullptr);

  if (self->len >= as(U_RESIZE_RADIO * bucket_sizes[self->bucket_idx], size_t)) {
    code = tbl_resize(self);
    u_err_if(code != 0, "resize failed.");
  }

  node = tbl_find(self, idx, key);

  /* node already exists */
  if (node->next != nullptr) {
    memcpy(val(node), val, self->vsize);
  } else {
    node = tbl_make_node(self, key, val);
    u_mem_if(node);

    node->next   = idx[0]->next;
    idx[0]->next = node;

    idx[0]->hash++;
    self->len++;
  }

  return;

err:
  u_free_if(node);
}

any_t tbl_each_init(u_tbl_t _self, bool flag) {
  tbl_t* self = as(_self, tbl_t*);

  u_check_ret(self == nullptr, nullptr);

  self->iter = nullptr;

  return nullptr;
}

bool tbl_each(u_tbl_t _self, any_t key, any_t val) {
  tbl_t* self = as(_self, tbl_t*);

  u_check_ret(self == nullptr, false);

  tbl_next(self);
  u_ret_if(self->iter == nullptr, false);

  memcpy(key, key(self->iter), self->ksize);
  memcpy(val, val(self->iter), self->vsize);

  return true;
}
