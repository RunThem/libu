#include "uu.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#define UU_DICT_NBL_STACK_MAX 64

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#define uu_chk_if(expr, ...)                                                                       \
  if (expr) {                                                                                      \
    return __VA_ARGS__;                                                                            \
  }                                                                                                \
  do {                                                                                             \
  } while (0)

#define uu_end_if(expr, ...)                                                                       \
  if (expr) {                                                                                      \
    goto __VA_ARGS__;                                                                              \
  }                                                                                                \
  do {                                                                                             \
  } while (0)

/***************************************************************************************************
 * Vec
 **************************************************************************************************/
#undef at
#define at(idx) (self->items + self->itsize * (idx))

struct uu_vec_t;
typedef struct uu_vec_t* uu_vec_mut_t;
typedef const struct uu_vec_t* uu_vec_ref_t;
typedef struct uu_vec_t {
  uint32_t itsize;
  uint32_t len;
  uint32_t cap;
  uint32_t idx;
  void* items;
} uu_vec_t;

void* __uu_vec_init(uint32_t itsize) {
  uu_vec_mut_t self = NULL;

  self = UU_MALLOC(sizeof(uu_vec_t));
  uu_end_if(!self, err0);

  self->items = UU_MALLOC(itsize * 16);
  uu_end_if(!self->items, err1);

  self->itsize = itsize;
  self->cap    = 16;
  self->len    = 0;
  self->idx    = 0;

  return (void*)self;

err1:
  UU_FREE(self);

err0:
  return NULL;
}

void __uu_vec_deinit(void* _self) {
  uu_vec_mut_t self = _self;

  assert(self);

  UU_FREE(self->items);
  UU_FREE(self);
}

uint32_t __uu_vec_len(void* _self) {
  uu_vec_mut_t self = _self;

  assert(self);

  return self->len;
}

void* __uu_vec_at(void* _self, uint32_t idx) {
  uu_vec_mut_t self = _self;

  assert(self);
  assert(idx < self->len);

  return at(idx);
}

void* __uu_vec_insert(void* _self, uint32_t idx) {
  uu_vec_mut_t self = _self;

  assert(self);
  assert(idx <= self->len);

  /* resize */
  if (self->len == self->cap) {
    void* items = realloc(self->items, 2 * self->cap * self->itsize);
    uu_end_if(!items, err0);

    self->cap *= 2;
    self->items = items;
  }

  if (idx != self->len) {
    memmove(at(idx + 1), at(idx), (self->len - idx) * self->itsize);
  }

  self->len++;

  return at(idx);

err0:
  return NULL;
}

void __uu_vec_remove(void* _self, uint32_t idx) {
  uu_vec_mut_t self = _self;

  assert(self);
  assert(idx < self->len);

  if (idx != self->len - 1) {
    memmove(at(idx), at(idx + 1), (self->len - idx - 1) * self->itsize);
  }

  self->len--;
}

void* __uu_vec_each(void* _self, int ev) {
  uu_vec_mut_t self = _self;

  assert(self);

  switch (ev) {
    case 1: self->idx = 0; return NULL;
    case 3: self->idx = self->len; return NULL;

    case 2: return (self->idx == self->len) ? NULL : at(self->idx++);
    case 4: return (self->idx == 0) ? NULL : at(--self->idx);

    default: assert(0);
  }
}

/***************************************************************************************************
 * Dict
 **************************************************************************************************/

struct uu_node_t;
typedef struct uu_node_t* uu_node_mut_t;
typedef const struct uu_node_t* uu_node_ref_t;
typedef struct uu_node_t {
  uu_node_mut_t parent;
  uu_node_mut_t left;
  uu_node_mut_t right;
  uint32_t height;
  uint32_t ksize;

  void* uptr;
  uint8_t key[0];
} uu_node_t;

struct uu_dict_t;
typedef struct uu_dict_t* uu_dict_mut_t;
typedef const struct uu_dict_t* uu_dict_ref_t;
typedef struct uu_dict_t {
  uint32_t ksize;
  uint32_t len;

  uu_dict_cmp_fn cmp_fn;

  uu_node_mut_t root;
  uu_node_mut_t iter;
} uu_dict_t;

static inline uint32_t lh(uu_node_mut_t n) {
  return n->left ? n->left->height : 0;
}

static inline uint32_t rh(uu_node_mut_t n) {
  return n->right ? n->right->height : 0;
}

static inline void height_set(uu_node_mut_t n) {
  uint32_t l = lh(n), r = rh(n);
  n->height = (l > r ? l : r) + 1;
}

static inline void
    __uu_dict_child_replace(uu_dict_mut_t self, uu_node_mut_t p, uu_node_mut_t o, uu_node_mut_t n) {
  if (!p) {
    self->root = n;
  } else {
    *((p->left == o) ? &p->left : &p->right) = n;
  }
}

#define __uu_dict_rotate(self, node, x, y)                                                         \
  ({                                                                                               \
    uu_node_mut_t y = node->y, parent = node->parent;                                              \
                                                                                                   \
    node->y = y->x;                                                                                \
    if (y->x)                                                                                      \
      y->x->parent = node;                                                                         \
                                                                                                   \
    y->x      = node;                                                                              \
    y->parent = parent;                                                                            \
                                                                                                   \
    __uu_dict_child_replace(self, parent, node, y);                                                \
    node->parent = y;                                                                              \
                                                                                                   \
    y;                                                                                             \
  })

#define __uu_dict_fix(self, node, x, y, op)                                                        \
  ({                                                                                               \
    uu_node_mut_t y = node->y;                                                                     \
                                                                                                   \
    if (lh(y) op rh(y)) {                                                                          \
      y = __uu_dict_rotate(self, y, y, x);                                                         \
      height_set(y->y);                                                                            \
      height_set(y);                                                                               \
    }                                                                                              \
                                                                                                   \
    node = __uu_dict_rotate(self, node, x, y);                                                     \
    height_set(node->x);                                                                           \
    height_set(node);                                                                              \
                                                                                                   \
    node;                                                                                          \
  })

uu_node_mut_t __uu_dict_remove_left_and_right(uu_dict_mut_t self, uu_node_mut_t node) {
  uu_node_mut_t onode = node, parent, left, child;

  node = node->right;
  while ((left = node->left)) {
    node = left;
  }

  child  = node->right;
  parent = node->parent;

  if (child) {
    child->parent = parent;
  }

  __uu_dict_child_replace(self, parent, node, child);
  if (node->parent == onode) {
    parent = node;
  }

  node->left   = onode->left;
  node->right  = onode->right;
  node->parent = onode->parent;
  node->height = onode->height;

  __uu_dict_child_replace(self, onode->parent, onode, node);
  onode->left->parent = node;

  if (onode->right) {
    onode->right->parent = node;
  }

  return parent;
}

uu_node_mut_t __uu_dict_remove_left_or_right(uu_dict_mut_t self, uu_node_mut_t node) {
  uu_node_mut_t child, parent;

  child = node->left;
  if (!child) {
    child = node->right;
  }

  parent = node->parent;
  __uu_dict_child_replace(self, parent, node, child);

  if (child) {
    child->parent = parent;
  }

  return parent;
}

void __uu_dict_remove_rebalance(uu_dict_mut_t self, uu_node_mut_t node) {
  int64_t l, r, h, diff;

  while (node) {
    l    = lh(node);
    r    = rh(node);
    h    = (l > r ? l : r) + 1;
    diff = l - r;

    if (node->height != h) {
      node->height = h;
    } else if (diff >= -1 && diff <= 1) {
      break;
    }

    if (diff <= -2) {
      node = __uu_dict_fix(self, node, left, right, >);
    } else if (diff >= 2) {
      node = __uu_dict_fix(self, node, right, left, <);
    }

    node = node->parent;
  }
}

void __uu_dict_insert_rebalance(uu_dict_mut_t self, uu_node_mut_t node) {
  int64_t l, r, h, diff;

  for (node = node->parent; node; node = node->parent) {
    l    = lh(node);
    r    = rh(node);
    h    = (l > r ? l : r) + 1;
    diff = l - r;

    if (node->height == h) {
      break;
    }

    node->height = h;

    if (diff <= -2) {
      node = __uu_dict_fix(self, node, left, right, >);
    } else if (diff >= 2) {
      node = __uu_dict_fix(self, node, right, left, <);
    }
  }
}

void* __uu_dict_init(uint32_t ksize, uu_dict_cmp_fn cmp_fn) {
  uu_dict_mut_t self = NULL;

  assert(cmp_fn);

  self = UU_MALLOC(sizeof(uu_dict_t));
  uu_end_if(!self, err0);

  self->ksize  = ksize;
  self->cmp_fn = cmp_fn;
  self->len    = 0;

  return self;

err0:
  return NULL;
}

void __uu_dict_deinit(void* _self) {
  uu_dict_mut_t self = _self;

  assert(self);
}

uint32_t __uu_dict_len(void* _self) {
  uu_dict_mut_t self = _self;

  assert(self);

  return self->len;
}

void* __uu_dict_at(void* _self, void* key) {
  uu_dict_mut_t self = _self;
  uu_node_mut_t node = self->root;
  int result         = 0;

  assert(self);
  uu_chk_if(self->len == 0, NULL);

  while ((node)) {
    result = self->cmp_fn(key, &node->key[0]);
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  uu_end_if(!node, err0);

  return node->uptr;

err0:
  return NULL;
}

int __uu_dict_remove(void* _self, void* key) {
  uu_dict_mut_t self = _self;
  uu_node_mut_t node = self->root, parent;
  int result         = 0;

  assert(self);
  uu_chk_if(self->len == 0, !0);

  while ((node)) {
    result = self->cmp_fn(key, &node->key[0]);
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  uu_end_if(!node, err0);

  parent = ((node->left && node->right) ? __uu_dict_remove_left_and_right :
                                          __uu_dict_remove_left_or_right)(self, node);

  UU_FREE(node);

  if (parent) {
    __uu_dict_remove_rebalance(self, parent);
  }

  self->len--;

  return !0;

err0:
  return !!0;
}

void* __uu_dict_insert(void* _self, void* key, void* uptr) {
  uu_dict_mut_t self  = _self;
  uu_node_mut_t *link = &self->root, parent = NULL, node;
  int result = 0;

  assert(self);

  while (link[0]) {
    parent = link[0];
    result = self->cmp_fn(key, &parent->key[0]);
    uu_end_if(result == 0, err0);

    link = (result < 0) ? &parent->left : &parent->right;
  }

  node = UU_MALLOC(sizeof(uu_node_t) + self->ksize);

  node->parent = parent;
  node->height = 1;
  node->uptr   = uptr;
  node->left = node->right = NULL;

  *link = node;
  self->len++;

  __uu_dict_insert_rebalance(self, node);

  return &node->key[0];

err0:
  return NULL;
}

int __uu_dict_each(void* _self, int init, void* out[2]) {
  uu_dict_mut_t self = _self;
  uu_node_mut_t iter, last;

  assert(self);
  uu_chk_if(self->len == 0, !!0);

  if (init) {
    self->iter = NULL;
    return !!0;
  }

  if (!self->iter) {
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
      while (1) {
        last = iter;
        iter = iter->parent;

        if (!iter || iter->left == last) {
          break;
        }
      }
    }
  }

  uu_end_if(!iter, err0);

  self->iter = iter;

  out[0] = &iter->key[0];
  out[1] = iter->uptr;

  return !0;

err0:
  return !!0;
}

struct uu_nbl_t;
typedef struct uu_nbl_t* uu_nbl_mut_t;
typedef const struct uu_nbl_t* uu_nbl_ref_t;
typedef struct uu_nbl_t {
  uu_node_ref_t node;
  int i;
} uu_nbl_t;

void __uu_dict_nbl_put(uu_nbl_mut_t nbl, uu_nbl_mut_t* top, uu_nbl_mut_t* bottom) {
  if (*top - *bottom < UU_DICT_NBL_STACK_MAX) {
    (*(*top)++) = *nbl;
  }
}

uu_nbl_mut_t __uu_dict_nbl_pop(uu_nbl_mut_t* top, uu_nbl_mut_t* bottom) {
  return *top > *bottom ? --*top : NULL;
}

void __uu_dict_node_dump(uu_node_ref_t node, uu_dict_dump_fn dump_fn) {
  uu_node_ref_t parent = node->parent;
  if (parent) {
    printf("%c: ", node == parent->left ? 'L' : 'R');
  }

  dump_fn(&node->key[0], node->uptr);

  printf("\n");
}

void __uu_dict_dump(void* _self, uu_dict_dump_fn dump_fn) {
  uu_dict_mut_t self = _self;
  uu_node_ref_t node = self->root;
  uu_nbl_t nbl, nbl_stack[UU_DICT_NBL_STACK_MAX] = {0};
  uu_nbl_mut_t p_nbl = NULL, top = nbl_stack, bottom = nbl_stack;
  int level = 0, sub_index;

  assert(self);

  printf("Dict = {len = %d, height = %d}\n", self->len, self->root->height);

  while (1) {
    if (node) {
      sub_index = p_nbl ? p_nbl->i : 0;
      p_nbl     = NULL;

      if ((!node->left && !node->right) || sub_index == 1) {
        nbl.node = NULL;
        nbl.i    = 0;
        // nbl = (uu_nbl_t){NULL, 0};
      } else {
        nbl.node = node;
        nbl.i    = 1;
      }

      __uu_dict_nbl_put(&nbl, &top, &bottom);

      level++;
      if (sub_index == 0) {
        for (int i = 0; i < level; i++) {
          if (i == level - 1) {
            printf("%-4s", "+---");
          } else {
            printf("%-4s", nbl_stack[i - 1].node != NULL ? "|" : " ");
          }
        }

        __uu_dict_node_dump(node, dump_fn);
      }

      node = sub_index == 0 ? node->left : node->right;
    } else {
      p_nbl = __uu_dict_nbl_pop(&top, &bottom);
      if (!p_nbl) {
        break;
      };

      node = p_nbl->node;
      level--;
    }
  }
};