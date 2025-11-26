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

#include <u/tree.h>
#include <u/u.h>

U_TREE_DEFINE(m, u_malloc, u_free, int ksize; u_hash_t hash);

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#define U_MAP_BUCKET_INIT_SIZE 8

#undef key
#define key(node) (node->u)

#undef val
#define val(node) (node->u + self->ksize)

/***************************************************************************************************
 * type
 **************************************************************************************************/
u_struct_def(dict, [[gnu::packed]]) {
  typeof_unqual(*(u_dict_t(dict_mut_t, dict_mut_t)){}) m;

  i32_t ksize;
  i32_t vsize;

  u_hash_fn hash_fn;

  int mask_size;
  int bucket_size;
  mtree_t* buckets;

  /* iterator index or node */
  int iter_idx;
  mnode_t iter_node;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
static inline int node_cmp(mnode_t x, mnode_t y) {
  if (x->hash == y->hash) {
    return memcmp(x->u, y->u, x->ksize);
  }

  return x->hash > y->hash ? 1 : -1;
}

/* fnv 64-bit hash function */
pri inline u_hash_t hash_fnv64bit(const u8_t* ptr, size_t len) {
  u_hash_t hash = 14695981039346656037U;

  for (size_t i = 0; i < len; ++i) {
    hash *= 1099511628211U;
    hash ^= (u64_t)ptr[i];
  }

  return hash;
}

pri void dict_rehash(dict_mut_t self) {
  mtree_t* buckets = nullptr;
  mnode_t next     = nullptr;
  mnode_t node     = nullptr;
  int bucket_size  = self->bucket_size;
  i64_t limit      = 0;

  limit = (self->m.len * 6) >> 2;
  u_end_if(limit <= self->bucket_size);

  while (bucket_size < limit) {
    bucket_size <<= 1;
  }

  buckets = u_calloc(sizeof(mtree_t), bucket_size);
  u_end_if(buckets);

  for (int i = 0; i < bucket_size; i++) {
    buckets[i] = mtree_new(node_cmp);
  }

  for (int i = 0; i < self->bucket_size; i++) {
    if (self->buckets[i]->len != 0) {
      while ((node = mtree_tear(self->buckets[i], &next))) {
        mtree_put(buckets[node->hash % bucket_size], node);
      }
    }

    mtree_del(self->buckets[i]);
  }

  self->bucket_size = bucket_size;
  self->buckets     = buckets;
  self->mask_size   = bucket_size - 1;

  return;

end:
}

pub any_t $dict_new(i32_t ksize, i32_t vsize, u_hash_fn hash_fn) {
  dict_mut_t self = nullptr;

  assert(ksize);

  self = u_zalloc(sizeof(dict_t));
  u_end_if(self);

  self->iter_node = mtree_new_node(ksize + vsize);
  u_end_if(self->iter_node);

  self->mask_size   = U_MAP_BUCKET_INIT_SIZE - 1;
  self->bucket_size = U_MAP_BUCKET_INIT_SIZE;
  self->buckets     = u_calloc(sizeof(mtree_t), self->bucket_size);
  u_end_if(self->buckets);

  for (int i = 0; i < self->bucket_size; i++) {
    self->buckets[i] = mtree_new(node_cmp);
    u_end_if(self->buckets[i]);
  }

  self->iter_node->ksize = ksize;
  self->ksize            = ksize;
  self->vsize            = vsize;
  self->hash_fn          = hash_fn ? hash_fn : hash_fnv64bit;
  self->m.ref            = any(self);

  return self;

end:
  for (int i = 0; self && i < self->bucket_size; i++) {
    mtree_del(self->buckets[i]);
  }

  u_free_if(self->buckets);
  u_free_if(self->iter_node);
  u_free_if(self);

  return nullptr;
}

pub void $dict_clear(any_t _self) {
  dict_mut_t self = (dict_mut_t)_self;
  mnode_t node    = nullptr;
  mnode_t next    = nullptr;

  u_chk_if(self);

  for (int i = 0; i < self->bucket_size; i++) {
    if (self->buckets[i]->len == 0) {
      continue;
    }

    while ((node = mtree_tear(self->buckets[i], &next))) {
      mtree_del_node(node);
    }
  }

  self->m.len = 0;
}

pub void $dict_cleanup(any_t _self) {
  dict_mut_t self = (dict_mut_t)_self;

  u_chk_if(self);

  for (int i = 0; i < self->bucket_size; i++) {
    mtree_del(self->buckets[i]);
  }

  u_free(self->buckets);
  u_free(self->iter_node);
  u_free(self);
}

pub any_t $dict_at(any_t _self, any_t key) {
  dict_mut_t self = (dict_mut_t)_self;
  mnode_t node    = nullptr;
  u_hash_t hash   = 0;
  int idx         = 0;

  u_chk_if(self, nullptr);

  hash = self->hash_fn(key, self->ksize);
  idx  = (int)(hash & self->mask_size);

  self->iter_node->hash = hash;
  memcpy(key(self->iter_node), key, self->ksize);

  node = mtree_at(self->buckets[idx], self->iter_node);
  u_end_if(node);

  return &node->u[0];

end:
  return nullptr;
}

pub void $dict_del(any_t _self, any_t data) {
  dict_mut_t self = (dict_mut_t)_self;
  mnode_t node    = u_container_of(data, typeof(*(mnode_t){}), u);
  int idx         = 0;

  u_chk_if(self);

  idx = (int)(node->hash & self->mask_size);

  mtree_pop(self->buckets[idx], node);
  mtree_del_node(node);

  self->m.len--;
}

pub any_t $dict_add(any_t _self, any_t key) {
  dict_mut_t self = (dict_mut_t)_self;
  mtree_t tree    = nullptr;
  mnode_t node    = nullptr;
  u_hash_t hash   = 0;
  int result      = 0;

  u_chk_if(self, nullptr);

  hash = self->hash_fn(key, self->ksize);
  tree = self->buckets[hash & self->mask_size];

  node = mtree_new_node(self->ksize + self->vsize);
  u_end_if(node);

  node->ksize = (int)self->ksize;
  node->hash  = hash;
  memcpy(key(node), key, self->ksize);

  if (tree->len == 0) {
    tree->root = node;
    tree->len++;
  } else {
    result = mtree_put(tree, node);
    u_end_if(result == false);
  }

  dict_rehash(self);

  self->m.len++;

  return &node->u[0];

end:
  u_free_if(node);

  return nullptr;
}

pub any_t $dict_each(any_t _self, bool init) {
  dict_mut_t self = (dict_mut_t)_self;
  mnode_t iter    = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(self->m.len == 0, nullptr);

  if (init) {
    self->iter_idx         = 0;
    return self->iter_node = nullptr;
  }

  iter = self->iter_node;
  while (true) {
    if (iter == nullptr) {
      iter = mtree_first(self->buckets[self->iter_idx]);
    } else {
      iter = mtree_next(iter);
    }

    u_brk_if(iter != nullptr);

    self->iter_idx++;
    u_end_if(self->iter_idx == self->bucket_size);
  }

  u_end_if(iter);

  self->iter_node = iter;

  return &iter->u[0];

end:
  return nullptr;
}
