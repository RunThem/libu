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

#ifdef USE_MIMALLOC
#  define U_PRI_ALLOC(size) mi_calloc(size, 1)
#  define U_PRI_FREE(ptr)   mi_free(ptr)
#endif
#define U_PRI_TREE_USERDATA                                                                        \
  size_t ksize;                                                                                    \
  u_hash_t hash
#define U_PRI_DEBUG
#include <u/pri.h>

/***************************************************************************************************
 * Tyoe
 **************************************************************************************************/
typedef struct {
  i8_t flags[4];
  size_t ksize;
  size_t vsize;
  size_t len;

  u_hash_fn hash_fn;

  int mask_size;
  int bucket_size;
  tree_t* buckets;

  int iter_idx;
  tnode_t iter_node;
  tnode_t next_node;

  tnode_t key;
} map_t, *map_ref_t;

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#define U_MAP_BUCKET_INIT_SIZE 8

#define key(node) (node->u)
#define val(node) (node->u + self->ksize)

/***************************************************************************************************
 * Function
 **************************************************************************************************/
static inline int tree_cmp_fn(tnode_t x, tnode_t y) {
  if (x->hash == y->hash) {
    return memcmp(x->u, y->u, x->ksize);
  }

  return x->hash > y->hash ? 1 : -1;
}

static inline void tree_node_dump(tnode_t n) {
  printf("h(%d), %d\n", n->h, *(int*)key(n));
}

/* fnv 64-bit hash function */
pri inline u_hash_t hash_fnv64bit(const u8_t* ptr, size_t len) {
  u_hash_t hash = 1469'5981'0393'4665'6037U;

  for (size_t i = 0; i < len; ++i) {
    hash *= 1'0995'1162'8211U;
    hash ^= (u64_t)ptr[i];
  }

  return hash;
}

pri void map_rehash(map_ref_t self) {
  tree_t* buckets = nullptr;
  int size        = self->bucket_size;
  tnode_t next    = nullptr;
  tnode_t node    = nullptr;
  size_t limit    = 0;

  limit = (self->len * 6) >> 2;

  u_chk_if(limit <= self->bucket_size);

  while (size < limit) {
    size <<= 1;
  }

  buckets = u_calloc(sizeof(tree_t), size);
  u_end_if(buckets);

  u_each (i, size) {
    buckets[i] = tree_new();
  }

  u_each (i, self->bucket_size) {
    if (self->buckets[i]->len != 0) {
      while ((node = tree_tear(self->buckets[i], &next))) {
        tree_put(buckets[node->hash % size], node);
      }
    }

    tree_del(self->buckets[i]);
  }

  self->bucket_size = size;
  self->buckets     = buckets;
  self->mask_size   = size - 1;

  return;
end:
}

pri inline void map_next(map_ref_t self) {
  while (true) {
    self->iter_node = tree_tear(self->buckets[self->iter_idx], &self->next_node);
    u_brk_if(self->iter_node != nullptr);

    self->iter_idx++;
    self->next_node = nullptr;
    u_end_if(self->iter_idx == self->bucket_size);
  }

  return;

end:
  self->iter_node = nullptr;
}

pub any_t map_new(size_t ksize, size_t vsize, u_hash_fn hash_fn) {
  map_ref_t self = nullptr;
  size_t size    = 0;

  u_chk_if(ksize == 0, nullptr);
  u_chk_if(vsize == 0, nullptr);

  self = u_zalloc(sizeof(map_t));
  u_end_if(self);

  self->key = tree_new_node(ksize + vsize);
  u_end_if(self->key);

  self->mask_size   = U_MAP_BUCKET_INIT_SIZE - 1;
  self->bucket_size = U_MAP_BUCKET_INIT_SIZE;
  self->buckets     = u_calloc(sizeof(tree_t), self->bucket_size);
  u_end_if(self->buckets);

  u_each (i, self->bucket_size) {
    self->buckets[i] = tree_new();
    u_end_if(self->buckets[i]);
  }

  self->key->ksize = ksize;
  self->ksize      = ksize;
  self->vsize      = vsize;
  self->len        = 0;
  self->hash_fn    = hash_fn != nullptr ? hash_fn : hash_fnv64bit;

  return self;

end:
  u_each (i, self->bucket_size) {
    tree_del(self->buckets[i]);
  }

  u_free_if(self->buckets);
  u_free_if(self->key);
  u_free_if(self);

  return nullptr;
}

pub void map_clear(any_t _self) {
  map_ref_t self = (map_ref_t)_self;
  tnode_t node   = nullptr;
  tnode_t next   = nullptr;

  u_chk_if(self);

  u_each (i, self->bucket_size) {
    if (self->buckets[i]->len == 0) {
      continue;
    }

    while ((node = tree_tear(self->buckets[i], &next))) {
      tree_del_node(node);
    }
  }

  self->len = 0;
}

pub void map_cleanup(any_t _self) {
  map_ref_t self = (map_ref_t)_self;

  u_chk_if(self);

  u_each (i, self->bucket_size) {
    tree_del(self->buckets[i]);
  }

  u_free(self->buckets);
  u_free(self->key);
  u_free(self);
}

pub inline size_t map_len(any_t _self) {
  map_ref_t self = (map_ref_t)_self;

  u_chk_if(self, 0);

  return self->len;
}

pub bool map_exist(any_t _self, any_t key) {
  map_ref_t self = (map_ref_t)_self;
  tnode_t node   = nullptr;
  u_hash_t hash  = 0;
  int result     = 0;
  int idx        = 0;

  u_chk_if(self, false);
  u_chk_if(key, false);

  hash = self->hash_fn(key, self->ksize);
  idx  = (int)(hash & self->mask_size);

  self->key->hash = hash;
  memcpy(key(self->key), key, self->ksize);

  node = tree_at(self->buckets[idx], self->key);

  return node != nullptr;
}

pub any_t map_at(any_t _self, any_t key) {
  map_ref_t self = (map_ref_t)_self;
  tnode_t node   = nullptr;
  u_hash_t hash  = 0;
  int result     = 0;
  int idx        = 0;

  u_chk_if(self, nullptr);
  u_chk_if(key, nullptr);

  hash = self->hash_fn(key, self->ksize);
  idx  = (int)(hash & self->mask_size);

  self->key->hash = hash;
  memcpy(key(self->key), key, self->ksize);

  node = tree_at(self->buckets[idx], self->key);
  u_end_if(node);

  return val(node);

end:
  return nullptr;
}

pub void map_pop(any_t _self, any_t key, any_t val) {
  map_ref_t self = (map_ref_t)_self;
  tnode_t node   = nullptr;
  u_hash_t hash  = 0;
  int result     = 0;
  int idx        = 0;

  u_chk_if(self);
  u_chk_if(key);
  u_chk_if(val);

  hash = self->hash_fn(key, self->ksize);
  idx  = (int)(hash & self->mask_size);

  self->key->hash = hash;
  memcpy(key(self->key), key, self->ksize);

  node = tree_at(self->buckets[idx], self->key);
  u_end_if(node);

  memcpy(val, val(node), self->vsize);

  tree_pop(self->buckets[idx], node);
  tree_del_node(node);

  self->len--;

  return;

end:
}

pub void map_put(any_t _self, any_t key, any_t val) {
  map_ref_t self = (map_ref_t)_self;
  tree_t tree    = nullptr;
  tnode_t node   = nullptr;
  u_hash_t hash  = 0;
  int result     = 0;

  u_chk_if(self);
  u_chk_if(key);
  u_chk_if(val);

  hash = self->hash_fn(key, self->ksize);
  tree = self->buckets[hash & self->mask_size];

  node = tree_new_node(self->ksize + self->vsize);
  u_end_if(node);

  node->ksize = (int)self->ksize;
  node->hash  = hash;
  memcpy(key(node), key, self->ksize);
  memcpy(val(node), val, self->vsize);

  if (tree->len == 0) {
    tree->root = node;
    tree->len++;
  } else {
    result = tree_put(tree, node);
    u_end_if(result == false);
  }

  map_rehash(self);

  self->len++;

  return;

end:
  u_free_if(node);
}

pub bool map_for_init(any_t _self, bool flag) {
  map_ref_t self = (map_ref_t)_self;

  u_chk_if(self, false);

  if (self->flags[0] == 0) {
    self->flags[0] = 1;
  } else if (self->flags[0] == 2) {
    self->flags[0] = 0;
  }

  self->next_node = nullptr;
  self->iter_node = nullptr;
  self->iter_idx  = 0;

  return self->flags[0];
}

pub void map_for_end(any_t _self) {
  map_ref_t self = (map_ref_t)_self;

  u_chk_if(self);

  self->flags[0] = 2;
}

pub bool map_for(any_t _self, any_t key, any_t val) {
  map_ref_t self = (map_ref_t)_self;

  u_chk_if(self, false);

  map_next(self);
  u_end_if(self->iter_node);

  memcpy(key, key(self->iter_node), self->ksize);
  memcpy(val, val(self->iter_node), self->vsize);

  return true;

end:
  return false;
}
