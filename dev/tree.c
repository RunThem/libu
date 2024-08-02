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

#include "tree.h"

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct node_t node_t, *node_ref_t;
struct node_t {
  node_ref_t left;
  node_ref_t right;
  node_ref_t parent;
  size_t height;

  tree_key_t key;
  tree_val_t val;
};

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
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
pri void
    tree_child_replace(tree_ref_t self, node_ref_t parent, node_ref_t oldnode, node_ref_t newnode) {
  if (parent == nullptr) {
    self->root = newnode;
  } else {
    if (parent->left == oldnode) {
      parent->left = newnode;
    } else {
      parent->right = newnode;
    }
  }
}

pri node_ref_t tree_rotate_left(tree_ref_t self, node_ref_t node) {
  node_ref_t right  = node->right;
  node_ref_t parent = node->parent;

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

pri node_ref_t tree_rotate_right(tree_ref_t self, node_ref_t node) {
  node_ref_t left   = node->left;
  node_ref_t parent = node->parent;

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

pri node_ref_t tree_fix_left(tree_ref_t self, node_ref_t node) {
  int lh           = 0;
  int rh           = 0;
  node_ref_t right = node->right;

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

pri node_ref_t tree_fix_right(tree_ref_t self, node_ref_t node) {
  int lh          = 0;
  int rh          = 0;
  node_ref_t left = node->left;

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

pri node_ref_t tree_pop_left_and_right(tree_ref_t self, node_ref_t node) {
  node_ref_t old    = node;
  node_ref_t parent = nullptr;
  node_ref_t left   = nullptr;
  node_ref_t child  = nullptr;

  node = node->right;
  while ((left = node->left) != nullptr) {
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

pri node_ref_t tree_pop_left_or_right(tree_ref_t self, node_ref_t node) {
  node_ref_t child  = nullptr;
  node_ref_t parent = nullptr;

  child = node->left;
  if (child == nullptr) {
    child = node->right;
  }

  parent = node->parent;
  tree_child_replace(self, parent, node, child);

  if (child) {
    child->parent = parent;
  }

  return parent;
}

pri void tree_pop_rebalance(tree_ref_t self, node_ref_t node) {
  int lh;
  int rh;
  int diff;
  size_t height;

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

pri void tree_push_rebalance(tree_ref_t self, node_ref_t node) {
  int lh        = 0;
  int rh        = 0;
  int diff      = 0;
  size_t height = 0;

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

pub any_t tree_new() {
  tree_ref_t self = nullptr;

  self = u_zalloc(sizeof(tree_t));
  u_end_if(self);

  self->len  = 0;
  self->root = nullptr;

  return self;

end:
  return nullptr;
}

pub void tree_clear(tree_ref_t self) {
  node_ref_t node = nullptr;
  node_ref_t head = self->root;
  node_ref_t tail = self->root;

  while (head != nullptr) {
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

pub void tree_cleanup(tree_ref_t self) {
  node_ref_t node = nullptr;

  tree_clear(self);
  u_free(self);
}

pub size_t tree_len(tree_ref_t self) {
  return self->len;
}

pub bool tree_exist(tree_ref_t self, tree_key_t key) {
  node_ref_t node = self->root;
  ret_t result    = 0;

  while (node) {
    result = key > node->key ? 1 : (key == node->key) ? 0 : -1;
    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  return node != nullptr;
}

pub any_t tree_at(tree_ref_t self, tree_key_t key) {
  node_ref_t node = self->root;
  ret_t result    = 0;

  while (node) {
    result = key > node->key ? 1 : (key == node->key) ? 0 : -1;
    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  u_end_if(node);

  return node->val;

end:
  return nullptr;
}

pub tree_val_t tree_pop(tree_ref_t self, tree_key_t key) {
  node_ref_t node   = self->root;
  node_ref_t parent = nullptr;
  ret_t result      = 0;
  tree_val_t val    = nullptr;

  while (node) {
    result = key > node->key ? 1 : (key == node->key) ? 0 : -1;
    u_brk_if(result == 0);

    node = (result < 0) ? node->left : node->right;
  }

  u_end_if(node);

  if (node->left && node->right) {
    parent = tree_pop_left_and_right(self, node);
  } else {
    parent = tree_pop_left_or_right(self, node);
  }

  if (parent) {
    tree_pop_rebalance(self, parent);
  }

  self->len--;
  val = node->val;
  u_free(val);

  return val;

end:
  return nullptr;
}

pub void tree_put(tree_ref_t self, tree_key_t key, tree_val_t val) {
  node_ref_t* link  = (node_ref_t*)&self->root;
  node_ref_t parent = nullptr;
  node_ref_t node   = nullptr;
  ret_t result      = 0;

  while (link[0]) {
    parent = link[0];
    result = key > parent->key ? 1 : (key == parent->key) ? 0 : -1;
    u_end_if(result == 0);

    link = (result < 0) ? &(parent->left) : &(parent->right);
  }

  node = u_zalloc(sizeof(node_t));
  u_end_if(node);

  node->left   = nullptr;
  node->right  = nullptr;
  node->parent = parent;
  node->height = 1;
  node->key    = key;
  node->val    = val;

  link[0] = node;

  self->len++;
  tree_push_rebalance(self, node);

  return;

end:
}

/***************************************************************************************************
 * Dump
 **************************************************************************************************/
#define MAX_LEVEL   64
#define LEFT_INDEX  0
#define RIGHT_INDEX 1

typedef struct {
  node_ref_t node;  /* node backlogged */
  int next_sub_idx; /* the index next to the backtrack point, valid when >= 1 */
} node_backlog_t, *node_backlog_ref_t;

pri inline void
    nbl_put(node_backlog_ref_t nbl, node_backlog_ref_t* top, node_backlog_ref_t* bottom) {
  if (*top - *bottom < MAX_LEVEL) {
    (*(*top)++) = *nbl;
  }
}

pri inline node_backlog_ref_t nbl_pop(node_backlog_ref_t* top, node_backlog_ref_t* bottom) {
  return *top > *bottom ? --*top : nullptr;
}

pri inline bool nbl_is_empty(node_backlog_ref_t top, node_backlog_ref_t bottom) {
  return top == bottom;
}

pri inline bool is_leaf(node_ref_t node) {
  return node->left == nullptr && node->right == nullptr;
}

pri inline void tree_node_dump(node_ref_t node) {
  node_ref_t parent = nullptr;

  u_chk_if(node);

  parent = node->parent;
  if (parent != nullptr) {
    putchar(node == parent->left ? 'L' : 'R');
  }

  printf("{ %jd }\n", node->key);
}

pub void tree_dump(tree_ref_t self) {
  int level                           = 0;
  int sub_index                       = 0;
  node_ref_t node                     = self->root;
  node_backlog_t nbl                  = {};
  node_backlog_t nbl_stack[MAX_LEVEL] = {};
  node_backlog_ref_t p_nbl            = nullptr;
  node_backlog_ref_t top              = nullptr;
  node_backlog_ref_t bottom           = nullptr;

  top = bottom = nbl_stack;

  while (true) {
    if (node != nullptr) {
      sub_index = p_nbl != nullptr ? p_nbl->next_sub_idx : LEFT_INDEX;
      p_nbl     = nullptr;

      if (is_leaf(node) || sub_index == RIGHT_INDEX) {
        nbl.node         = nullptr;
        nbl.next_sub_idx = LEFT_INDEX;
      } else {
        nbl.node         = node;
        nbl.next_sub_idx = RIGHT_INDEX;
      }

      nbl_put(&nbl, &top, &bottom);
      level++;

      /* draw lines as long as sub_idx is the first one */
      if (sub_index == LEFT_INDEX) {
        u_each (i, level) {
          if (i == level - 1) {
            printf("%-4s", "+---");
          } else {
            if (nbl_stack[i - 1].node != nullptr) {
              printf("%-4s", "|");
            } else {
              printf("%-4s", " ");
            }
          }
        }

        tree_node_dump(node);
      }

      node = sub_index == LEFT_INDEX ? node->left : node->right;
    } else {
      p_nbl = nbl_pop(&top, &bottom);
      u_brk_if(p_nbl);

      node = p_nbl->node;
      level--;
    }
  }
}
