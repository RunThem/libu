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
 * Type
 **************************************************************************************************/
typedef struct node_t node_t, *node_ref_t;
struct node_t {
  node_ref_t left;
  node_ref_t right;
  node_ref_t parent;
  i64_t height;
};

typedef struct {
  i64_t ksize;
  i64_t vsize;
  i64_t len;

  u_cmp_fn cmp_fn;

  node_ref_t root;
  node_ref_t free;
} avl_t, *avl_ref_t;

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#undef key
#define key(node) (any(node) + sizeof(node_t))

#undef val
#define val(node) (any(node) + sizeof(node_t) + self->ksize)

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
 * Function
 **************************************************************************************************/
pri node_ref_t avl_new_node(avl_ref_t self, node_ref_t parent, any_t key, any_t val) {
  node_ref_t node = nullptr;

  if (self->free) {
    node       = self->free;
    self->free = node->parent;
  } else {
    node = u_zalloc(sizeof(node_t) + self->ksize + self->vsize);
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

pri inline void
    avl_child_replace(avl_ref_t self, node_ref_t parent, node_ref_t oldnode, node_ref_t newnode) {
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

pri inline node_ref_t avl_rotate_left(avl_ref_t self, node_ref_t node) {
  node_ref_t right  = node->right;
  node_ref_t parent = node->parent;

  node->right = right->left;
  if (right->left) {
    right->left->parent = node;
  }

  right->left   = node;
  right->parent = parent;

  avl_child_replace(self, parent, node, right);

  node->parent = right;

  return right;
}

pri inline node_ref_t avl_rotate_right(avl_ref_t self, node_ref_t node) {
  node_ref_t left   = node->left;
  node_ref_t parent = node->parent;

  node->left = left->right;
  if (left->right) {
    left->right->parent = node;
  }

  left->right  = node;
  left->parent = parent;

  avl_child_replace(self, parent, node, left);
  node->parent = left;

  return left;
}

pri inline node_ref_t avl_fix_left(avl_ref_t self, node_ref_t node) {
  int lh           = 0;
  int rh           = 0;
  node_ref_t right = node->right;

  lh = lh(right);
  rh = rh(right);
  if (lh > rh) {
    right = avl_rotate_right(self, right);
    height(right->right);
    height(right);
  }

  node = avl_rotate_left(self, node);
  height(node->left);
  height(node);

  return node;
}

pri inline node_ref_t avl_fix_right(avl_ref_t self, node_ref_t node) {
  int lh          = 0;
  int rh          = 0;
  node_ref_t left = node->left;

  lh = lh(left);
  rh = rh(left);
  if (lh < rh) {
    left = avl_rotate_left(self, left);
    height(left->left);
    height(left);
  }

  node = avl_rotate_right(self, node);
  height(node->right);
  height(node);

  return node;
}

pri node_ref_t avl_pop_left_and_right(avl_ref_t self, node_ref_t node) {
  node_ref_t old    = node;
  node_ref_t parent = nullptr;
  node_ref_t left   = nullptr;
  node_ref_t child  = nullptr;

  node = node->right;
  while ((left = node->left)) {
    node = left;
  }

  child  = node->right;
  parent = node->parent;

  if (child) {
    child->parent = parent;
  }

  avl_child_replace(self, parent, node, child);

  if (node->parent == old) {
    parent = node;
  }

  node->left   = old->left;
  node->right  = old->right;
  node->parent = old->parent;
  node->height = old->height;

  avl_child_replace(self, old->parent, old, node);
  old->left->parent = node;

  if (old->right) {
    old->right->parent = node;
  }

  return parent;
}

pri node_ref_t avl_pop_left_or_right(avl_ref_t self, node_ref_t node) {
  node_ref_t child  = nullptr;
  node_ref_t parent = nullptr;

  child = node->left;
  if (!child) {
    child = node->right;
  }

  parent = node->parent;
  avl_child_replace(self, parent, node, child);

  if (child) {
    child->parent = parent;
  }

  return parent;
}

pri void avl_pop_rebalance(avl_ref_t self, node_ref_t node) {
  int lh       = 0;
  int rh       = 0;
  int diff     = 0;
  i64_t height = 0;

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
      node = avl_fix_left(self, node);
    } else if (diff >= 2) {
      node = avl_fix_right(self, node);
    }

    node = node->parent;
  }
}

pri void avl_put_rebalance(avl_ref_t self, node_ref_t node) {
  int lh       = 0;
  int rh       = 0;
  int diff     = 0;
  i64_t height = 0;

  for (node = node->parent; node; node = node->parent) {
    lh     = lh(node);
    rh     = rh(node);
    height = max(lh, rh) + 1;
    diff   = lh - rh;

    u_brk_if(node->height == height);

    node->height = height;

    if (diff <= -2) {
      node = avl_fix_left(self, node);
    } else if (diff >= 2) {
      node = avl_fix_right(self, node);
    }
  }
}

pub any_t avl_new(i64_t ksize, i64_t vsize, u_cmp_fn cmp_fn) {
  avl_ref_t self = nullptr;

  u_chk_if(ksize == 0, nullptr);
  /* vsize == 0, support set */

  self = u_zalloc(sizeof(avl_t) + ksize + vsize);
  u_end_if(self);

  self->ksize  = ksize;
  self->vsize  = vsize;
  self->len    = 0;
  self->cmp_fn = cmp_fn;
  self->root   = nullptr;

  return self;

end:
  return nullptr;
}

pub void avl_clear(any_t _self) {
  avl_ref_t self  = (avl_ref_t)_self;
  node_ref_t node = nullptr;
  node_ref_t head = self->root;
  node_ref_t tail = self->root;

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

  self->len = 0;
}

pub void avl_cleanup(any_t _self) {
  avl_ref_t self  = (avl_ref_t)_self;
  node_ref_t node = nullptr;

  u_chk_if(self);

  avl_clear(_self);

  while (self->free != nullptr) {
    node       = self->free;
    self->free = node->parent;

    u_free(node);
  }

  u_free(self);
}

pub i64_t avl_len(any_t _self) {
  avl_ref_t self = (avl_ref_t)_self;

  u_chk_if(self, -1);

  return self->len;
}

pub bool avl_is_exist(any_t _self, any_t key) {
  avl_ref_t self  = (avl_ref_t)_self;
  node_ref_t node = self->root;
  ret_t result    = 0;

  u_chk_if(self, false);

  while (node) {
    if (self->cmp_fn) {
      result = self->cmp_fn(key, key(node));
    } else {
      result = memcmp(key, key(node), self->ksize);
    }

    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  return node != nullptr;
}

pub any_t avl_at(any_t _self, any_t key) {
  avl_ref_t self  = (avl_ref_t)_self;
  node_ref_t node = self->root;
  ret_t result    = 0;

  u_chk_if(self, nullptr);
  u_chk_if(self->len == 0, nullptr);

  while (node) {
    if (self->cmp_fn) {
      result = self->cmp_fn(key, key(node));
    } else {
      result = memcmp(key, key(node), self->ksize);
    }

    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  u_end_if(node);

  return val(node);

end:
  return nullptr;
}

pub void avl_pop(any_t _self, any_t key, any_t val) {
  avl_ref_t self    = (avl_ref_t)_self;
  node_ref_t node   = self->root;
  node_ref_t parent = nullptr;
  ret_t result      = 0;

  u_chk_if(self);

  while (node) {
    if (self->cmp_fn) {
      result = self->cmp_fn(key, key(node));
    } else {
      result = memcmp(key, key(node), self->ksize);
    }

    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  u_end_if(node);

  if (node->left && node->right) {
    parent = avl_pop_left_and_right(self, node);
  } else {
    parent = avl_pop_left_or_right(self, node);
  }

  memcpy(key, key(node), self->ksize);
  memcpy(val, val(node), self->vsize);

  node->parent = self->free;
  self->free   = node;

  self->len--;

  if (parent) {
    avl_pop_rebalance(self, parent);
  }

  return;

end:
}

pub void avl_put(any_t _self, any_t key, any_t val) {
  avl_ref_t self    = (avl_ref_t)_self;
  node_ref_t* link  = &self->root;
  node_ref_t parent = nullptr;
  node_ref_t node   = nullptr;
  ret_t result      = 0;

  u_chk_if(self);

  while (link[0]) {
    parent = link[0];

    if (self->cmp_fn) {
      result = self->cmp_fn(key, key(parent));
    } else {
      result = memcmp(key, key(parent), self->ksize);
    }
    u_end_if(result == 0);

    link = (result < 0) ? &(parent->left) : &(parent->right);
  }

  node = avl_new_node(self, parent, key, val);
  u_end_if(node);

  *link = node;
  self->len++;

  avl_put_rebalance(self, node);

  return;

end:
}

pub void avl_pole(any_t _self, any_t key, any_t val, u_order_e order) {
  avl_ref_t self  = (avl_ref_t)_self;
  node_ref_t node = self->root;

  u_chk_if(self);
  u_chk_if(self->len == 0);

  if (order == U_ORDER_ASCEND) {
    while (node->left) {
      node = node->left;
    }
  } else {
    while (node->right) {
      node = node->right;
    }
  }

  memcpy(key, key(node), self->ksize);
  memcpy(val, val(node), self->vsize);
}

pub bool avl_for(any_t _self, bool* init, any_t key, any_t val, any_t* _iter, u_order_e order) {
  avl_ref_t self  = (avl_ref_t)_self;
  node_ref_t iter = *(node_ref_t*)_iter;
  node_ref_t last = nullptr;

  u_chk_if(self, false);
  u_chk_if(self->len == 0, false);

  /* init */
  if (!*init) {
    *init = true;

    iter = self->root;
    if (order == U_ORDER_ASCEND) {
      while (iter->left) {
        iter = iter->left;
      }
    } else {
      while (iter->right) {
        iter = iter->right;
      }
    }
  } else { /* range */
    if (order == U_ORDER_ASCEND) {
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
    } else {
      if (iter->left) {
        iter = iter->left;
        while (iter->right) {
          iter = iter->right;
        }
      } else {
        while (true) {
          last = iter;
          iter = iter->parent;

          u_brk_if(!iter || iter->right == last);
        }
      }
    }
  }

  u_end_if(iter);

  *_iter = iter;

  memcpy(key, key(iter), self->ksize);
  memcpy(val, val(iter), self->vsize);

  return true;

end:
  return false;
}
