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

#include "u/itree.h"

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
    (node)->height = u_max(lh(node), rh(node)) + 1;                                                \
  } while (0)

/***************************************************************************************************
 * Type
 **************************************************************************************************/
u_struct_def(tnode, [[gnu::packed]]) {
  tnode_mut_t left;
  tnode_mut_t right;
  tnode_mut_t parent;
  i32_t height;

  u8_t data[0];
};

u_struct_def(tree, [[gnu::packed]]) {
  any_t ref;
  int len;

  i32_t ksize;
  i32_t vsize;

  u_cmp_fn cmp_fn;

  tnode_mut_t root;
  tnode_mut_t free;
  tnode_mut_t iter; /* iter */
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri tnode_mut_t __u_tree_new_node(tree_mut_t self, tnode_mut_t parent) {
  tnode_mut_t node = NULL;

  if (self->free) {
    node       = self->free;
    self->free = node->parent;
  } else {
    node = u_zalloc(sizeof(tnode_t) + self->ksize + self->vsize);
    u_end_if(node);
  }

  node->left   = NULL;
  node->right  = NULL;
  node->parent = parent;
  node->height = 1;

  return node;

end:
  return NULL;
}

pri inline void __u_tree_child_replace(tree_mut_t self,
                                       tnode_mut_t parent,
                                       tnode_mut_t oldnode,
                                       tnode_mut_t newnode) {
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

pri inline tnode_mut_t __u_tree_rotate_left(tree_mut_t self, tnode_mut_t node) {
  tnode_mut_t right  = node->right;
  tnode_mut_t parent = node->parent;

  node->right = right->left;
  if (right->left) {
    right->left->parent = node;
  }

  right->left   = node;
  right->parent = parent;

  __u_tree_child_replace(self, parent, node, right);

  node->parent = right;

  return right;
}

pri inline tnode_mut_t __u_tree_rotate_right(tree_mut_t self, tnode_mut_t node) {
  tnode_mut_t left   = node->left;
  tnode_mut_t parent = node->parent;

  node->left = left->right;
  if (left->right) {
    left->right->parent = node;
  }

  left->right  = node;
  left->parent = parent;

  __u_tree_child_replace(self, parent, node, left);
  node->parent = left;

  return left;
}

pri inline tnode_mut_t __u_tree_fix_left(tree_mut_t self, tnode_mut_t node) {
  int lh            = 0;
  int rh            = 0;
  tnode_mut_t right = node->right;

  lh = lh(right);
  rh = rh(right);
  if (lh > rh) {
    right = __u_tree_rotate_right(self, right);
    height(right->right);
    height(right);
  }

  node = __u_tree_rotate_left(self, node);
  height(node->left);
  height(node);

  return node;
}

pri inline tnode_mut_t __u_tree_fix_right(tree_mut_t self, tnode_mut_t node) {
  int lh           = 0;
  int rh           = 0;
  tnode_mut_t left = node->left;

  lh = lh(left);
  rh = rh(left);
  if (lh < rh) {
    left = __u_tree_rotate_left(self, left);
    height(left->left);
    height(left);
  }

  node = __u_tree_rotate_right(self, node);
  height(node->right);
  height(node);

  return node;
}

pri tnode_mut_t __u_tree_pop_left_and_right(tree_mut_t self, tnode_mut_t node) {
  tnode_mut_t old    = node;
  tnode_mut_t parent = NULL;
  tnode_mut_t left   = NULL;
  tnode_mut_t child  = NULL;

  node = node->right;
  while ((left = node->left)) {
    node = left;
  }

  child  = node->right;
  parent = node->parent;

  if (child) {
    child->parent = parent;
  }

  __u_tree_child_replace(self, parent, node, child);

  if (node->parent == old) {
    parent = node;
  }

  node->left   = old->left;
  node->right  = old->right;
  node->parent = old->parent;
  node->height = old->height;

  __u_tree_child_replace(self, old->parent, old, node);
  old->left->parent = node;

  if (old->right) {
    old->right->parent = node;
  }

  return parent;
}

pri tnode_mut_t __u_tree_pop_left_or_right(tree_mut_t self, tnode_mut_t node) {
  tnode_mut_t child  = NULL;
  tnode_mut_t parent = NULL;

  child = node->left;
  if (!child) {
    child = node->right;
  }

  parent = node->parent;
  __u_tree_child_replace(self, parent, node, child);

  if (child) {
    child->parent = parent;
  }

  return parent;
}

pri void __u_tree_pop_rebalance(tree_mut_t self, tnode_mut_t node) {
  int lh       = 0;
  int rh       = 0;
  int diff     = 0;
  i32_t height = 0;

  while (node) {
    lh     = lh(node);
    rh     = rh(node);
    height = u_max(lh, rh) + 1;
    diff   = lh - rh;

    if (node->height != height) {
      node->height = height;
    } else if (diff >= -1 && diff <= 1) {
      break;
    }

    if (diff <= -2) {
      node = __u_tree_fix_left(self, node);
    } else if (diff >= 2) {
      node = __u_tree_fix_right(self, node);
    }

    node = node->parent;
  }
}

pri void __u_tree_put_rebalance(tree_mut_t self, tnode_mut_t node) {
  int lh       = 0;
  int rh       = 0;
  int diff     = 0;
  i32_t height = 0;

  for (node = node->parent; node; node = node->parent) {
    lh     = lh(node);
    rh     = rh(node);
    height = u_max(lh, rh) + 1;
    diff   = lh - rh;

    u_brk_if(node->height == height);

    node->height = height;

    if (diff <= -2) {
      node = __u_tree_fix_left(self, node);
    } else if (diff >= 2) {
      node = __u_tree_fix_right(self, node);
    }
  }
}

pub any_t __u_tree_new(i32_t ksize, i32_t vsize, u_cmp_fn cmp_fn) {
  tree_mut_t self = NULL;

  u_chk_if(ksize == 0, NULL);
  /* vsize == 0, support set */

  self = u_zalloc(sizeof(tree_t) + ksize + vsize);
  u_end_if(self);

  self->ksize  = ksize;
  self->vsize  = vsize;
  self->cmp_fn = cmp_fn;
  self->ref    = any(self);

  return self;

end:
  return NULL;
}

pub void __u_tree_clear(any_t _self) {
  tree_mut_t self  = (tree_mut_t)_self;
  tnode_mut_t node = NULL;
  tnode_mut_t head = self->root;
  tnode_mut_t tail = self->root;

  u_chk_if(self->len == 0);

  while (head) {
    node = head;

    if (node->left) {
      tail->parent = node->left;
      tail         = tail->parent;
      tail->parent = NULL;
    }

    if (node->right) {
      tail->parent = node->right;
      tail         = tail->parent;
      tail->parent = NULL;
    }

    head = head->parent;

    u_free(node);
  }

  self->len = 0;
}

pub void __u_tree_cleanup(any_t _self) {
  tree_mut_t self  = (tree_mut_t)_self;
  tnode_mut_t node = NULL;

  u_chk_if(self);

  __u_tree_clear(_self);

  while (self->free != NULL) {
    node       = self->free;
    self->free = node->parent;

    u_free(node);
  }

  u_free(self);
}

pub any_t __u_tree_at(any_t _self, any_t key) {
  tree_mut_t self  = (tree_mut_t)_self;
  tnode_mut_t node = self->root;
  int result       = 0;

  u_chk_if(self, NULL);
  u_chk_if(self->len == 0, NULL);

  while (node) {
    result = self->cmp_fn(key, &node->data[0]);
    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  u_end_if(node);

  return &node->data[0];

end:
  return NULL;
}

pub void __u_tree_del(any_t _self, any_t key) {
  tree_mut_t self    = (tree_mut_t)_self;
  tnode_mut_t node   = self->root;
  tnode_mut_t parent = NULL;
  int result         = 0;

  u_chk_if(self);

  while (node) {
    result = self->cmp_fn(key, &node->data[0]);
    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  u_end_if(node);

  if (node->left && node->right) {
    parent = __u_tree_pop_left_and_right(self, node);
  } else {
    parent = __u_tree_pop_left_or_right(self, node);
  }

  node->parent = self->free;
  self->free   = node;

  self->len--;

  if (parent) {
    __u_tree_pop_rebalance(self, parent);
  }

  return;

end:
  return;
}

pub any_t __u_tree_add(any_t _self, any_t key) {
  tree_mut_t self    = (tree_mut_t)_self;
  tnode_mut_t* link  = &self->root;
  tnode_mut_t parent = NULL;
  tnode_mut_t node   = NULL;
  int result         = 0;

  u_chk_if(self, NULL);

  while (link[0]) {
    parent = link[0];

    result = self->cmp_fn(key, &parent->data[0]);
    u_end_if(result == 0);

    link = (result < 0) ? &(parent->left) : &(parent->right);
  }

  node = __u_tree_new_node(self, parent);
  u_end_if(node);

  *link = node;
  self->len++;

  __u_tree_put_rebalance(self, node);

  return &node->data[0];

end:
  return NULL;
}

pub any_t __u_tree_each(any_t _self, bool init) {
  tree_mut_t self  = (tree_mut_t)_self;
  tnode_mut_t iter = NULL;
  tnode_mut_t last = NULL;

  u_chk_if(self, NULL);
  u_chk_if(self->len == 0, NULL);

  if (init) {
    return self->iter = NULL;
  }

  if (self->iter == NULL) {
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

  return &iter->data[0];

end:
  return NULL;
}
