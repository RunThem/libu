/* MIT License
 *
 * Copyright (c) 2023 RunThem <iccy.fun@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * */

#include <u/u.h>

static size_t bucket_sizes[] = {
    7,
    61,
    127,
    251,
    509,
    1021,
    2039,
    4093,
    8191,
    1'6381,
    3'2749,
    6'5521,
    13'1071,
    26'2139,
    52'4287,
    104'8573,
    209'7143,
    419'4301,
    838'8593,
    1677'7213,
    3355'4393,
    6710'8859,
    1'3421'7689,
    2'6843'5399,
    5'3687'0909,
    10'7374'1789,
    21'4748'3647,
    42'9496'7291,
    85'8993'4583,
    171'7986'9143,
    343'5973'8337,
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

typedef struct map_t map_t;
struct map_t {
  u8_t flags[4];
  size_t ksize;
  size_t vsize;
  size_t len;

  u_hash_fn hash_fn;

  node_t* iter;
  node_t* free_nodes;

  i8_t bucket_idx;
  node_t* buckets;
};

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#define U_RESIZE_RADIO 0.75

#undef key
#define key(node) (any(node) + sizeof(node_t))

#undef val
#define val(node) (any(node) + sizeof(node_t) + self->ksize)

/***************************************************************************************************
 * Function
 **************************************************************************************************/
/* [idx, len] { 0xffff } { 0xfffe } */
static void map_debug(map_t* self) {
  fprintf(stderr,
          "ksize(%zu), vsize(%zu), len(%zu), buckets(%zu)\n",
          self->ksize,
          self->vsize,
          self->len,
          bucket_sizes[self->bucket_idx]);

  for (size_t i = 0; i < bucket_sizes[self->bucket_idx]; i++) {
    fprintf(stderr, "[%zu, %zu] ", i, self->buckets[i].hash);

    for (node_t* node = &self->buckets[i]; node != nullptr; node = node->next) {
      fprintf(stderr, "{ %p } ", node);
    }

    fprintf(stderr, "\n");
  }
}

/* fnv 64-bit hash function */
static u_hash_t hash_fnv64bit(cu8_t* ptr, size_t len) {
  u_hash_t hash = 1469'5981'0393'4665'6037U;

  for (size_t i = 0; i < len; ++i) {
    hash *= 1'0995'1162'8211U;
    hash ^= (u64_t)ptr[i];
  }

  return hash;
}

static node_t* map_make_node(map_t* self, any_t key, any_t val) {
  node_t* node = nullptr;

  if (self->free_nodes != nullptr) {
    node             = self->free_nodes;
    self->free_nodes = node->next;
  } else {
    node = u_zalloc(sizeof(node_t) + self->ksize + self->vsize);
    u_nil_if(node);
  }

  node->hash = self->hash_fn(key, self->ksize);
  memcpy(key(node), key, self->ksize);
  memcpy(val(node), val, self->vsize);

  return node;

err:
  return nullptr;
}

static node_t* map_find(map_t* self, node_t* idx[2], any_t key) {
  u_hash_t hash = 0;
  node_t* node  = nullptr;

  hash   = self->hash_fn(key, self->ksize);
  idx[0] = &self->buckets[hash % bucket_sizes[self->bucket_idx]];

  for (idx[1] = idx[0], node = (idx[0])->next; node->next != nullptr;
       idx[1] = node, node = node->next) {

    if (hash == node->hash && memcmp(key(node), key, self->ksize) == 0) {
      break;
    }
  }

  return node;
}

static node_t* map_make_buckets(size_t bs) {
  node_t* buckets = nullptr;

  buckets = u_calloc(bs * 2, sizeof(node_t));
  u_nil_if(buckets);

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

static void map_next(map_t* self) {
  size_t idx = 0;

  if (self->iter != nullptr) {
    self->iter = self->iter->next;

    /* ok */
    u_nret_if(self->iter->next != nullptr);

    /* end */
    if (self->iter->hash == 0) {
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

static bool map_resize(map_t* self) {
  node_t* nbuckets  = nullptr;
  node_t* node      = nullptr;
  node_t* tmp       = nullptr;
  node_t* list      = nullptr;
  node_t* nlist     = nullptr;
  size_t bucket_idx = 0;

  bucket_idx = bucket_sizes[self->bucket_idx + 1];
  nbuckets   = map_make_buckets(bucket_idx);
  u_nil_if(nbuckets);

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

any_t map_new(size_t ksize, size_t vsize, u_hash_fn hash_fn) {
  map_t* self = nullptr;

  u_chk_if(ksize == 0, nullptr);
  u_chk_if(vsize == 0, nullptr);

  self = u_zalloc(sizeof(map_t) + ksize + vsize);
  u_nil_if(self);

  self->buckets = map_make_buckets(bucket_sizes[0]);
  u_nil_if(self->buckets);

  self->ksize      = ksize;
  self->vsize      = vsize;
  self->len        = 0;
  self->bucket_idx = 0;
  self->hash_fn    = hash_fn != nullptr ? hash_fn : hash_fnv64bit;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

void map_clear(any_t _self) {
  map_t* self  = (map_t*)_self;
  node_t* node = nullptr;
  node_t* list = nullptr;

  u_nchk_if(self == nullptr);

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

void map_cleanup(any_t _self) {
  map_t* self = (map_t*)_self;

  u_nchk_if(self == nullptr);

  map_clear(_self);

  u_free_if(self->buckets);
  u_free_if(self);
}

size_t map_len(any_t _self) {
  map_t* self = (map_t*)_self;

  u_chk_if(self == nullptr, 0);

  return self->len;
}

bool map_exist(any_t _self, any_t key) {
  map_t* self    = (map_t*)_self;
  node_t* node   = nullptr;
  node_t* idx[2] = {};

  u_chk_if(self == nullptr, false);
  u_chk_if(key == nullptr, false);

  node = map_find(self, idx, key);

  return node->next != nullptr;
}

any_t map_at(any_t _self, any_t key) {
  map_t* self    = (map_t*)_self;
  node_t* node   = nullptr;
  node_t* idx[2] = {};

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(key == nullptr, nullptr);
  u_chk_if(self->len == 0, nullptr);

  node = map_find(self, idx, key);
  u_ret_if(node->next == nullptr, nullptr, "node not exists.");

  return val(node);
}

void map_pop(any_t _self, any_t key, any_t val) {
  map_t* self    = (map_t*)_self;
  node_t* node   = nullptr;
  node_t* idx[2] = {};

  u_nchk_if(self == nullptr);
  u_nchk_if(key == nullptr);
  u_nchk_if(val == nullptr);

  node = map_find(self, idx, key);
  if (node->next != nullptr) {
    idx[0]->next = node->next;
    idx[1]->hash--;

    self->len--;

    memcpy(val, val(node), self->vsize);

    node->next       = self->free_nodes;
    self->free_nodes = node;

    return;
  }

  bzero(val, self->vsize);
}

void map_put(any_t _self, any_t key, any_t val) {
  map_t* self    = (map_t*)_self;
  node_t* node   = nullptr;
  node_t* idx[2] = {};
  ret_t code     = 0;

  u_nchk_if(self == nullptr);
  u_nchk_if(key == nullptr);
  u_nchk_if(val == nullptr);

  if (self->len >= (size_t)(0.75 * (f64_t)bucket_sizes[self->bucket_idx])) {
    code = map_resize(self);
    u_err_if(code != true, "resize failed.");
  }

  node = map_find(self, idx, key);

  /* node already exists */
  if (node->next != nullptr) {
    memcpy(val(node), val, self->vsize);
  } else {
    node = map_make_node(self, key, val);
    u_nil_if(node);

    node->next   = idx[0]->next;
    idx[0]->next = node;

    idx[0]->hash++;
    self->len++;
  }

  return;

err:
  u_free_if(node);
}

bool map_for_init(any_t _self, bool flag) {
  map_t* self = (map_t*)_self;

  u_chk_if(self == nullptr, false);

  if (self->flags[0] == 0) {
    self->flags[0] = 1;
  } else if (self->flags[0] == 2) {
    self->flags[0] = 0;
  }

  self->iter = nullptr;

  return self->flags[0];
}

void map_for_end(any_t _self) {
  map_t* self = (map_t*)_self;

  u_nchk_if(self == nullptr);

  self->flags[0] = 2;
}

bool map_for(any_t _self, any_t key, any_t val) {
  map_t* self = (map_t*)_self;

  u_chk_if(self == nullptr, false);

  map_next(self);
  u_ret_if(self->iter == nullptr, false);

  memcpy(key, key(self->iter), self->ksize);
  memcpy(val, val(self->iter), self->vsize);

  return true;
}
