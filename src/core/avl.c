#include <u/core/avl.h>
#include <u/core/queue.h>

/* clang-format off */
/*
 *                                             return point
 *                                                  +
 *                                                  |
 *                                                  v
 *     0        8        16       24       32       38
 *     +--------+--------+--------+--------+--------+----------------+
 *     | itsize |  len   | cmp_fn |  root  |  itor  |       ...      |
 *     +--------+--------+--------+--------+--------+----------------+
 *                                    |                sizeof(T)  NULL
 *                                    |                           ^
 *                                    |     0        8        16  |    24       32
 *                                    |     +--------+--------+--------+--------+
 * +-+-+------------------------------+---->|  left  | right  | parent | height |...
 * | | |                                    +--------+--------+--------+--------+
 * | | |                                        |         |
 * | | |                                        |         |
 * | | |          +-----------------------------+         +--------+
 * | | |          |                                                |
 * | | |     0    v   8        16       24       32            0   v    8        16       24       32
 * | | |     +--------+--------+--------+--------+             +--------+--------+--------+--------+
 * | | |     |  left  | right  | parent | height |...          |  left  | right  | parent | height |...
 * | | |     +--------+--------+--------+--------+             +--------+--------+--------+--------+
 * | | |         |        |        |                               |        |        |
 * | | |         v        v        |                               |        v        +--------------------+
 * | | |        ...      ...       |                               |       ...                            |
 * | | |                           |                               |                                      |
 * | | +---------------------------+                           0   v    8        16       24       32     |
 * | |                                                         +---+----+--------+--------+--------+      |
 * | |                                                         |  left  | right  | parent | height |...   |
 * | |                                                         +--------+--------+--------+--------+      |
 * | |                                                             |        |        |                    |
 * | |                                                             v        v        |                    |
 * | |                                                            ...      ...       |                    |
 * | +-------------------------------------------------------------------------------+                    |
 * |                                                                                                      |
 * +------------------------------------------------------------------------------------------------------+
 *
 */
/* clang-format on */

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct avl_node_t avl_node_t;
struct avl_node_t {
  avl_node_t* left;
  avl_node_t* right;
  avl_node_t* parent;
  size_t height;
};

#undef key
#define key(node) (any(node) + sizeof(avl_node_t))

#undef val
#define val(node) (any(node) + sizeof(avl_node_t) + self->ksize)

#undef lh
#define lh(node) (((node)->left) ? ((node)->left)->height : 0)

#undef rh
#define rh(node) (((node)->right) ? ((node)->right)->height : 0)

#undef height
#define height(node)                                                                               \
  do {                                                                                             \
    (node)->height = max(lh(node), rh(node)) + 1;                                                  \
  } while (0)

typedef struct avl_t avl_t;
struct avl_t {
  size_t ksize;
  size_t vsize;
  size_t len;

  u_avl_cmp_fn cmp_fn;

  avl_node_t* root;
  avl_node_t* itor;
  avl_node_t* free;
};

#define selfof(self) (assert((self) != nullptr), as((self) - sizeof(avl_t), avl_t*))

static avl_node_t* __avl_node(avl_t* self, avl_node_t* parent, any_t key, any_t val) {
  avl_node_t* node = nullptr;

  if (self->free != nullptr) {
    node       = self->free;
    self->free = node->parent;
  } else {
    node = u_zalloc(sizeof(avl_node_t) + self->ksize + self->vsize);
    u_mem_if(node);
  }

  node->left   = nullptr;
  node->right  = nullptr;
  node->parent = parent;
  node->height = 1;

  memcpy(key(node), key, self->ksize);
  memcpy(val(node), val, self->vsize);

  return node;

err:
  return nullptr;
}

static void
    __avl_child_replace(avl_t* self, avl_node_t* parent, avl_node_t* oldnode, avl_node_t* newnode) {
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

static avl_node_t* __avl_rotate_left(avl_t* self, avl_node_t* node) {
  avl_node_t* right  = node->right;
  avl_node_t* parent = node->parent;

  node->right = right->left;
  if (right->left) {
    right->left->parent = node;
  }

  right->left   = node;
  right->parent = parent;

  __avl_child_replace(self, parent, node, right);

  node->parent = right;

  return right;
}

static avl_node_t* __avl_rotate_right(avl_t* self, avl_node_t* node) {
  avl_node_t* left   = node->left;
  avl_node_t* parent = node->parent;

  node->left = left->right;
  if (left->right) {
    left->right->parent = node;
  }

  left->right  = node;
  left->parent = parent;

  __avl_child_replace(self, parent, node, left);
  node->parent = left;

  return left;
}

static avl_node_t* __avl_fix_left(avl_t* self, avl_node_t* node) {
  int lh            = 0;
  int rh            = 0;
  avl_node_t* right = node->right;

  lh = lh(right);
  rh = rh(right);
  if (lh > rh) {
    right = __avl_rotate_right(self, right);
    height(right->right);
    height(right);
  }

  node = __avl_rotate_left(self, node);
  height(node->left);
  height(node);

  return node;
}

static avl_node_t* __avl_fix_right(avl_t* self, avl_node_t* node) {
  int lh           = 0;
  int rh           = 0;
  avl_node_t* left = node->left;

  lh = lh(left);
  rh = rh(left);
  if (lh < rh) {
    left = __avl_rotate_left(self, left);
    height(left->left);
    height(left);
  }

  node = __avl_rotate_right(self, node);
  height(node->right);
  height(node);

  return node;
}

static avl_node_t* __avl_pop_left_and_right(avl_t* self, avl_node_t* node) {
  avl_node_t* old    = node;
  avl_node_t* parent = nullptr;
  avl_node_t* left   = nullptr;
  avl_node_t* child  = nullptr;

  node = node->right;
  while ((left = node->left) != nullptr) {
    node = left;
  }

  child  = node->right;
  parent = node->parent;

  if (child) {
    child->parent = parent;
  }

  __avl_child_replace(self, parent, node, child);

  if (node->parent == old) {
    parent = node;
  }

  node->left   = old->left;
  node->right  = old->right;
  node->parent = old->parent;
  node->height = old->height;

  __avl_child_replace(self, old->parent, old, node);
  old->left->parent = node;

  if (old->right) {
    old->right->parent = node;
  }

  return parent;
}

static avl_node_t* __avl_pop_left_or_right(avl_t* self, avl_node_t* node) {
  avl_node_t* child  = nullptr;
  avl_node_t* parent = nullptr;

  child = node->left;
  if (child == nullptr) {
    child = node->right;
  }

  parent = node->parent;
  __avl_child_replace(self, parent, node, child);

  if (child) {
    child->parent = parent;
  }

  return parent;
}

static void __avl_pop_rebalance(avl_t* self, avl_node_t* node) {
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
      node = __avl_fix_left(self, node);
    } else if (diff >= 2) {
      node = __avl_fix_right(self, node);
    }

    node = node->parent;
  }
}

static void __avl_push_rebalance(avl_t* self, avl_node_t* node) {
  int lh        = 0;
  int rh        = 0;
  int diff      = 0;
  size_t height = 0;

  for (node = node->parent; node != nullptr; node = node->parent) {
    lh     = lh(node);
    rh     = rh(node);
    height = max(lh, rh) + 1;
    diff   = lh - rh;

    if (node->height == height) {
      break;
    }

    node->height = height;

    if (diff <= -2) {
      node = __avl_fix_left(self, node);
    } else if (diff >= 2) {
      node = __avl_fix_right(self, node);
    }
  }
}

static void __avl_next(avl_t* self) {
  avl_node_t* node = self->itor;
  avl_node_t* last = nullptr;

  if (node == nullptr) {
    node = self->root;
    while (node->left) {
      node = node->left;
    }
  } else {
    if (node->right) {
      node = node->right;
      while (node->left) {
        node = node->left;
      }
    } else {
      while (true) {
        last = node;
        node = node->parent;

        if (node == nullptr || node->left == last) {
          break;
        }
      }
    }
  }

  self->itor = node;
}

static void __avl_prev(avl_t* self) {
  avl_node_t* node = self->itor;
  avl_node_t* last = nullptr;

  if (node == nullptr) {
    node = self->root;
    while (node->right) {
      node = node->right;
    }
  } else {
    if (node->left) {
      node = node->left;
      while (node->right) {
        node = node->right;
      }
    } else {
      while (true) {
        last = node;
        node = node->parent;

        if (node == nullptr || node->right == last) {
          break;
        }
      }
    }
  }

  self->itor = node;
}

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __avl_new(size_t ksize, size_t vsize, u_avl_cmp_fn cmp_fn) {
  avl_t* self = nullptr;

  u_check_ret(ksize == 0, nullptr);
  u_check_ret(vsize == 0, nullptr);
  u_check_ret(cmp_fn == nullptr, nullptr);

  self = u_zalloc(sizeof(avl_t) + ksize + vsize + sizeof(any_t));
  u_mem_if(self);

  self->ksize  = ksize;
  self->vsize  = vsize;
  self->len    = 0;
  self->cmp_fn = cmp_fn;
  self->root   = nullptr;

  infln("avl new(ksize(%zu), vsize(%zu), cmp_fn(%p))", ksize, vsize, cmp_fn);

  return self + 1;

err:
  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __avl_clear(any_t _self) {
  avl_t* self                = selfof(_self);
  avl_node_t* node           = nullptr;
  u_queue_t(avl_node_t*) que = nullptr;

  u_check_nret(self->len == 0);

  que = u_queue_new(avl_node_t*);
  u_queue_push(que, self->root);

  while (!u_queue_empty(que)) {
    node = u_queue_peek(que);

    if (node->left) {
      u_queue_push(que, node->left);
    }
    if (node->right) {
      u_queue_push(que, node->right);
    }

    u_free(node);

    u_queue_pop(que);
  }

  u_queue_cleanup(que);

  self->len = 0;
}

void __avl_cleanup(any_t _self) {
  avl_t* self      = selfof(_self);
  avl_node_t* node = nullptr;

  __avl_clear(_self);

  while (self->free != nullptr) {
    node       = self->free;
    self->free = node->parent;

    u_free(node);
  }

  u_free(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __avl_ksize(any_t _self) {
  return selfof(_self)->ksize;
}

size_t __avl_vsize(any_t _self) {
  return selfof(_self)->vsize;
}

size_t __avl_len(any_t _self) {
  return selfof(_self)->len;
}

bool __avl_empty(any_t _self) {
  return selfof(_self)->len == 0;
}

bool __avl_exist(any_t _self) {
  avl_t* self      = selfof(_self);
  avl_node_t* node = self->root;
  any_t key        = _self;
  ret_t result     = 0;

  while (node) {
    result = self->cmp_fn(key, key(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  return node != nullptr;
}

void __avl_re(any_t _self) {
  avl_t* self      = selfof(_self);
  avl_node_t* node = self->root;
  any_t key        = _self;
  any_t val        = _self + self->ksize;
  ret_t result     = 0;

  while (node) {
    result = self->cmp_fn(key, key(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  u_nret_if(node == nullptr, "node not exists.");

  memcpy(val(node), val, self->vsize);
}

void __avl_at(any_t _self) {
  avl_t* self      = selfof(_self);
  avl_node_t* node = self->root;
  any_t key        = _self;
  any_t val        = _self + self->ksize;
  ret_t result     = 0;

  while (node) {
    result = self->cmp_fn(key, key(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  u_if(node == nullptr, "node not exists.") {
    bzero(val, self->vsize);
  }
  else {
    memcpy(val, val(node), self->vsize);
  }
}

void __avl_pop(any_t _self) {
  avl_t* self        = selfof(_self);
  avl_node_t* node   = self->root;
  avl_node_t* parent = nullptr;
  any_t key          = _self;
  any_t val          = _self + self->ksize;
  ret_t result       = 0;

  while (node) {
    result = self->cmp_fn(key, key(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  if (node->left && node->right) {
    parent = __avl_pop_left_and_right(self, node);
  } else {
    parent = __avl_pop_left_or_right(self, node);
  }

  memcpy(val, val(node), self->vsize);

  node->parent = self->free;
  self->free   = node;

  self->len--;

  if (parent) {
    __avl_pop_rebalance(self, parent);
  }
}

ret_t __avl_push(any_t _self) {
  avl_t* self        = selfof(_self);
  avl_node_t** link  = &self->root;
  avl_node_t* parent = nullptr;
  avl_node_t* node   = nullptr;
  any_t key          = _self;
  any_t val          = _self + self->ksize;
  ret_t result       = 0;

  while (*link) {
    parent = *link;

    result = self->cmp_fn(key, key(parent));
    u_err_if(result == 0, "avl push node already exists.");

    link = (result < 0) ? &(parent->left) : &(parent->right);
  }

  node = __avl_node(self, parent, key, val);
  u_mem_if(node);

  *link = node;
  self->len++;

  __avl_push_rebalance(self, node);

  return 0;

err:
  return -1;
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
void __avl_range_init(any_t _self) {
  selfof(_self)->itor = nullptr;
}

bool __avl_range(any_t _self, bool flag) {
  avl_t* self = selfof(_self);
  any_t key   = _self;
  any_t val   = _self + self->ksize;

  u_ret_if(self->len == 0, false);

  ((flag == 1) ? __avl_next : __avl_prev)(self);
  u_ret_if(self->itor == nullptr, false);

  memcpy(key, key(self->itor), self->ksize);
  memcpy(val, val(self->itor), self->vsize);

  return true;
}
