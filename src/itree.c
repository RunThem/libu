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

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#undef key
#define key(node) (any(node) + sizeof(tnode_t))

#undef val
#define val(node) (any(node) + sizeof(tnode_t) + self->ksize)

#undef lh
#define lh(node) (((node)->left) ? ((node)->left)->height : 0)

#undef rh
#define rh(node) (((node)->right) ? ((node)->right)->height : 0)

#undef height
#define height(node)                                                                               \
  do {                                                                                             \
    (node)->height = max(lh(node), rh(node)) + 1;                                                  \
  } while (0)

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct tnode_t tnode_t, *tnode_ref_t;
struct tnode_t {
  tnode_ref_t left;
  tnode_ref_t right;
  tnode_ref_t parent;
  i32_t height;
};

typedef struct tree_t tree_t, *tree_ref_t;
struct tree_t {
  typeof_unqual(*(u_tree_t(tree_ref_t, tree_ref_t)){}) m;

  i32_t ksize;
  i32_t vsize;
  i32_t len;

  u_cmp_fn cmp_fn;

  tnode_ref_t root;
  tnode_ref_t free;
  tnode_ref_t iter; /* iter */
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri tnode_ref_t tree_new_node(tree_ref_t self, tnode_ref_t parent, any_t key, any_t val) {
  tnode_ref_t node = nullptr;

  if (self->free) {
    node       = self->free;
    self->free = node->parent;
  } else {
    node = u_zalloc(sizeof(tnode_t) + self->ksize + self->vsize);
    u_end_if(node);
  }

  node->left   = nullptr;
  node->right  = nullptr;
  node->parent = parent;
  node->height = 1;

  memcpy(key(node), key, self->ksize);
  memcpy(val(node), val, self->vsize);

  return node;

end:
  return nullptr;
}

pri inline void tree_child_replace(tree_ref_t self,
                                   tnode_ref_t parent,
                                   tnode_ref_t oldnode,
                                   tnode_ref_t newnode) {
  if (!parent) {
    self->root = newnode;
    return;
  }

  if (parent->left == oldnode) {
    parent->left = newnode;
  } else {
    parent->right = newnode;
  }
}

pri inline tnode_ref_t tree_rotate_left(tree_ref_t self, tnode_ref_t node) {
  tnode_ref_t right  = node->right;
  tnode_ref_t parent = node->parent;

  node->right = right->left;
  if (right->left) {
    right->left->parent = node;
  }

  right->left   = node;
  right->parent = parent;

  tree_child_replace(self, parent, node, right);

  node->parent = right;

  return right;
}

pri inline tnode_ref_t tree_rotate_right(tree_ref_t self, tnode_ref_t node) {
  tnode_ref_t left   = node->left;
  tnode_ref_t parent = node->parent;

  node->left = left->right;
  if (left->right) {
    left->right->parent = node;
  }

  left->right  = node;
  left->parent = parent;

  tree_child_replace(self, parent, node, left);
  node->parent = left;

  return left;
}

pri inline tnode_ref_t tree_fix_left(tree_ref_t self, tnode_ref_t node) {
  int lh            = 0;
  int rh            = 0;
  tnode_ref_t right = node->right;

  lh = lh(right);
  rh = rh(right);
  if (lh > rh) {
    right = tree_rotate_right(self, right);
    height(right->right);
    height(right);
  }

  node = tree_rotate_left(self, node);
  height(node->left);
  height(node);

  return node;
}

pri inline tnode_ref_t tree_fix_right(tree_ref_t self, tnode_ref_t node) {
  int lh           = 0;
  int rh           = 0;
  tnode_ref_t left = node->left;

  lh = lh(left);
  rh = rh(left);
  if (lh < rh) {
    left = tree_rotate_left(self, left);
    height(left->left);
    height(left);
  }

  node = tree_rotate_right(self, node);
  height(node->right);
  height(node);

  return node;
}

pri tnode_ref_t tree_pop_left_and_right(tree_ref_t self, tnode_ref_t node) {
  tnode_ref_t old    = node;
  tnode_ref_t parent = nullptr;
  tnode_ref_t left   = nullptr;
  tnode_ref_t child  = nullptr;

  node = node->right;
  while ((left = node->left)) {
    node = left;
  }

  child  = node->right;
  parent = node->parent;

  if (child) {
    child->parent = parent;
  }

  tree_child_replace(self, parent, node, child);

  if (node->parent == old) {
    parent = node;
  }

  node->left   = old->left;
  node->right  = old->right;
  node->parent = old->parent;
  node->height = old->height;

  tree_child_replace(self, old->parent, old, node);
  old->left->parent = node;

  if (old->right) {
    old->right->parent = node;
  }

  return parent;
}

pri tnode_ref_t tree_pop_left_or_right(tree_ref_t self, tnode_ref_t node) {
  tnode_ref_t child  = nullptr;
  tnode_ref_t parent = nullptr;

  child = node->left;
  if (!child) {
    child = node->right;
  }

  parent = node->parent;
  tree_child_replace(self, parent, node, child);

  if (child) {
    child->parent = parent;
  }

  return parent;
}

pri void tree_pop_rebalance(tree_ref_t self, tnode_ref_t node) {
  int lh       = 0;
  int rh       = 0;
  int diff     = 0;
  i32_t height = 0;

  while (node) {
    lh     = lh(node);
    rh     = rh(node);
    height = max(lh, rh) + 1;
    diff   = lh - rh;

    if (node->height != height) {
      node->height = height;
    } else if (diff >= -1 && diff <= -1) {
      break;
    }

    if (diff <= -2) {
      node = tree_fix_left(self, node);
    } else if (diff >= 2) {
      node = tree_fix_right(self, node);
    }

    node = node->parent;
  }
}

pri void tree_put_rebalance(tree_ref_t self, tnode_ref_t node) {
  int lh       = 0;
  int rh       = 0;
  int diff     = 0;
  i32_t height = 0;

  for (node = node->parent; node; node = node->parent) {
    lh     = lh(node);
    rh     = rh(node);
    height = max(lh, rh) + 1;
    diff   = lh - rh;

    u_brk_if(node->height == height);

    node->height = height;

    if (diff <= -2) {
      node = tree_fix_left(self, node);
    } else if (diff >= 2) {
      node = tree_fix_right(self, node);
    }
  }
}

pub any_t $tree_new(i32_t ksize, i32_t vsize, u_cmp_fn cmp_fn) {
  tree_ref_t self = nullptr;

  u_chk_if(ksize == 0, nullptr);
  /* vsize == 0, support set */

  self = u_zalloc(sizeof(tree_t) + ksize + vsize);
  u_end_if(self);

  self->ksize  = ksize;
  self->vsize  = vsize;
  self->cmp_fn = cmp_fn;
  self->m.ref  = self;

  return self;

end:
  return nullptr;
}

pub void $tree_clear(any_t _self) {
  tree_ref_t self  = (tree_ref_t)_self;
  tnode_ref_t node = nullptr;
  tnode_ref_t head = self->root;
  tnode_ref_t tail = self->root;

  u_chk_if(self->len == 0);

  while (head) {
    node = head;

    if (node->left) {
      tail->parent = node->left;
      tail         = tail->parent;
      tail->parent = nullptr;
    }

    if (node->right) {
      tail->parent = node->right;
      tail         = tail->parent;
      tail->parent = nullptr;
    }

    head = head->parent;

    u_free(node);
  }

  self->len   = 0;
  self->m.len = 0;
}

pub void $tree_cleanup(any_t _self) {
  tree_ref_t self  = (tree_ref_t)_self;
  tnode_ref_t node = nullptr;

  u_chk_if(self);

  $tree_clear(_self);

  while (self->free != nullptr) {
    node       = self->free;
    self->free = node->parent;

    u_free(node);
  }

  u_free(self);
}

pub any_t $tree_at(any_t _self, any_t key, any_t val) {
  tree_ref_t self  = (tree_ref_t)_self;
  tnode_ref_t node = self->root;
  int result       = 0;

  u_chk_if(self, nullptr);
  u_chk_if(self->len == 0, nullptr);

  while (node) {
    result = self->cmp_fn(key, key(node));
    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  u_end_if(node);

  if (val != nullptr) {
    memcpy(val(node), val, self->vsize);
  }

  return val(node);

end:
  return nullptr;
}

pub void $tree_pop(any_t _self, any_t key, any_t val) {
  tree_ref_t self    = (tree_ref_t)_self;
  tnode_ref_t node   = self->root;
  tnode_ref_t parent = nullptr;
  int result         = 0;

  u_chk_if(self);

  while (node) {
    result = self->cmp_fn(key, key(node));
    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  u_end_if(node);

  if (node->left && node->right) {
    parent = tree_pop_left_and_right(self, node);
  } else {
    parent = tree_pop_left_or_right(self, node);
  }

  memcpy(key, key(node), self->ksize);
  memcpy(val, val(node), self->vsize);

  node->parent = self->free;
  self->free   = node;

  self->len--;
  self->m.len--;

  if (parent) {
    tree_pop_rebalance(self, parent);
  }

  return;

end:
}

pub void $tree_put(any_t _self, any_t key, any_t val) {
  tree_ref_t self    = (tree_ref_t)_self;
  tnode_ref_t* link  = &self->root;
  tnode_ref_t parent = nullptr;
  tnode_ref_t node   = nullptr;
  int result         = 0;

  u_chk_if(self);

  while (link[0]) {
    parent = link[0];

    result = self->cmp_fn(key, key(parent));
    u_end_if(result == 0);

    link = (result < 0) ? &(parent->left) : &(parent->right);
  }

  node = tree_new_node(self, parent, key, val);
  u_end_if(node);

  *link = node;
  self->len++;
  self->m.len++;

  tree_put_rebalance(self, node);

  return;

end:
}

pub bool $tree_each(any_t _self, any_t key, any_t val) {
  tree_ref_t self  = (tree_ref_t)_self;
  tnode_ref_t iter = nullptr;
  tnode_ref_t last = nullptr;

  u_chk_if(self, false);
  u_chk_if(self->len == 0, false);

  if (key == nullptr) {
    self->iter = nullptr;
  } else {
    if (self->iter == nullptr) {
      iter = self->root;

      while (iter->left) {
        iter = iter->left;
      }
    } else {
      iter = self->iter;

      if (iter->right) {
        iter = iter->right;
        while (iter->left) {
          iter = iter->left;
        }
      } else {
        while (true) {
          last = iter;
          iter = iter->parent;

          u_brk_if(!iter || iter->left == last);
        }
      }
    }

    u_end_if(iter);

    self->iter = iter;

    memcpy(key, key(iter), self->ksize);
    memcpy(val, val(iter), self->vsize);
  }

  return true;

end:
  return false;
}
