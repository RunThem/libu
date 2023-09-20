#include "core/avl.h"
#include "core/queue.h"

/* clang-format off */
/*
 *                                    return point
 *                                         +
 *                                         |
 *                                         v
 *     0        8        16       24       32
 *     +--------+--------+--------+--------+----------------+--------+
 *     | itsize |  len   | cmp_fn |  root  |       ...      |  itor  |
 *     +--------+--------+--------+--------+----------------+--------+
 *                                    |         sizeof(T)        NULL
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
typedef struct node_t node_t;
struct node_t {
  node_t* left;
  node_t* right;
  node_t* parent;
  size_t height;
};

#define avl_lh(node)   (((node)->left) ? ((node)->left)->height : 0)
#define avl_rh(node)   (((node)->right) ? ((node)->right)->height : 0)
#define avl_item(node) ((node) + 1)

#define avl_height(node)                                                                           \
  do {                                                                                             \
    (node)->height = max(avl_lh(node), avl_rh(node)) + 1;                                          \
  } while (0)

typedef struct avl_t avl_t;
struct avl_t {
  size_t itsize;
  size_t len;

  avl_cmp_fn cmp_fn;

  node_t* root;
};

#define self_of(self) (assert((self) != nullptr), as((self) - sizeof(avl_t), avl_t*))

static node_t* __avl_node(avl_t* self, node_t* parent, any_t item) {
  node_t* node = u_zalloc(sizeof(node_t) + self->itsize);
  u_mem_if(node);

  node->left   = nullptr;
  node->right  = nullptr;
  node->parent = parent;
  node->height = 1;

  memcpy(node + 1, item, self->itsize);

  return node;

err:
  return nullptr;
}

static void __avl_child_replace(avl_t* self, node_t* parent, node_t* oldnode, node_t* newnode) {
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

static node_t* __avl_rotate_left(avl_t* self, node_t* node) {
  node_t* right  = node->right;
  node_t* parent = node->parent;

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

static node_t* __avl_rotate_right(avl_t* self, node_t* node) {
  node_t* left   = node->left;
  node_t* parent = node->parent;

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

static node_t* __avl_fix_left(avl_t* self, node_t* node) {
  int hl        = 0;
  int hr        = 0;
  node_t* right = node->right;

  hl = avl_lh(right);
  hr = avl_rh(right);
  if (hl > hr) {
    right = __avl_rotate_right(self, right);
    avl_height(right->right);
    avl_height(right);
  }

  node = __avl_rotate_left(self, node);
  avl_height(node->left);
  avl_height(node);

  return node;
}

static node_t* __avl_fix_right(avl_t* self, node_t* node) {
  int hl       = 0;
  int hr       = 0;
  node_t* left = node->left;

  hl = avl_lh(left);
  hr = avl_rh(left);
  if (hl < hr) {
    left = __avl_rotate_left(self, left);
    avl_height(left->left);
    avl_height(left);
  }

  node = __avl_rotate_right(self, node);
  avl_height(node->right);
  avl_height(node);

  return node;
}

static node_t* __avl_pop_left_and_right(avl_t* self, node_t* node) {
  node_t* old    = node;
  node_t* parent = nullptr;
  node_t* left   = nullptr;
  node_t* child  = nullptr;

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

static node_t* __avl_pop_left_or_right(avl_t* self, node_t* node) {
  node_t* child  = nullptr;
  node_t* parent = nullptr;

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

static void __avl_pop_rebalance(avl_t* self, node_t* node) {
  int hl;
  int hr;
  int diff;
  size_t height;

  while (node) {
    hl     = avl_lh(node);
    hr     = avl_rh(node);
    height = max(hl, hr) + 1;
    diff   = hl - hr;

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

static void __avl_push_rebalance(avl_t* self, node_t* node) {
  int hr        = 0;
  int hl        = 0;
  int diff      = 0;
  size_t height = 0;

  for (node = node->parent; node != nullptr; node = node->parent) {
    hl     = avl_lh(node);
    hr     = avl_rh(node);
    height = max(hl, hr) + 1;
    diff   = hl - hr;

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

static void __avl_next(avl_t* self, node_t** itor) {
  node_t* node = *itor;
  node_t* last = nullptr;

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

  *itor = node;
}

static void __avl_prev(avl_t* self, node_t** itor) {
  node_t* node = *itor;
  node_t* last = nullptr;

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

  *itor = node;
}

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __avl_new(size_t itsize, avl_cmp_fn cmp_fn) {
  avl_t* self = nullptr;

  u_assert(itsize == 0);
  u_assert(cmp_fn == nullptr);

  self = u_zalloc(sizeof(avl_t) + itsize + sizeof(any_t));
  u_mem_if(self);

  self->itsize = itsize;
  self->len    = 0;
  self->cmp_fn = cmp_fn;
  self->root   = nullptr;

  return self + 1;

err:
  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __avl_clear(any_t _self) {
  avl_t* self        = self_of(_self);
  node_t* node       = nullptr;
  queue(node_t*) que = nullptr;

  u_noret_if(self->len == 0);

  que = queue_new(node_t*);
  queue_push(que, self->root);

  while (!queue_empty(que)) {
    node = queue_peek(que);

    if (node->left) {
      queue_push(que, node->left);
    }
    if (node->right) {
      queue_push(que, node->right);
    }

    u_free(node);

    queue_pop(que);
  }

  queue_cleanup(que);

  self->len = 0;
}

void __avl_cleanup(any_t _self) {
  avl_t* self = self_of(_self);

  __avl_clear(_self);
  u_free(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __avl_itsize(any_t _self) {
  return self_of(_self)->itsize;
}

size_t __avl_len(any_t _self) {
  return self_of(_self)->len;
}

bool __avl_empty(any_t _self) {
  return self_of(_self)->len == 0;
}

bool __avl_exist(any_t _self) {
  avl_t* self  = self_of(_self);
  node_t* node = self->root;
  any_t item   = _self;
  ret_t result = 0;

  while (node) {
    result = self->cmp_fn(item, avl_item(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  return node != nullptr;
}

void __avl_at(any_t _self) {
  avl_t* self  = self_of(_self);
  node_t* node = self->root;
  any_t item   = _self;
  ret_t result = 0;

  while (node) {
    result = self->cmp_fn(item, avl_item(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  memcpy(item, avl_item(node), self->itsize);
}

void __avl_pop(any_t _self) {
  avl_t* self    = self_of(_self);
  node_t* node   = self->root;
  node_t* parent = nullptr;
  node_t* tmp    = nullptr;
  any_t item     = _self;
  ret_t result   = 0;

  while (node) {
    result = self->cmp_fn(item, avl_item(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  tmp = node;
  if (node->left && node->right) {
    parent = __avl_pop_left_and_right(self, node);
  } else {
    parent = __avl_pop_left_or_right(self, node);
  }

  u_free_if(tmp);
  self->len--;

  if (parent) {
    __avl_pop_rebalance(self, parent);
  }
}

ret_t __avl_push(any_t _self) {
  avl_t* self    = self_of(_self);
  node_t** link  = &self->root;
  node_t* parent = nullptr;
  node_t* node   = nullptr;
  any_t item     = _self;
  ret_t result   = 0;

  while (*link) {
    parent = *link;

    result = self->cmp_fn(item, avl_item(parent));
    u_err_if(result == 0);

    link = (result < 0) ? &(parent->left) : &(parent->right);
  }

  node = __avl_node(self, parent, item);
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
bool __avl_range(any_t _self, bool flag) {
  avl_t* self   = self_of(_self);
  any_t item    = _self;
  node_t** itor = _self + self->itsize;

  u_ret_if(self->len == 0, false);

  ((flag == 1) ? __avl_next : __avl_prev)(self, itor);
  u_ret_if(*itor == nullptr, false);

  memcpy(item, avl_item(*itor), self->itsize);

  return true;
}

#ifndef NDEBUG
static void __avl_debug(node_t* node, int depth, int flag) {
  u_noret_if(node == nullptr);

  for (size_t i = 0; i < depth; i++) {
    printf(" ");
  }

  printf("%c ", (flag == 0) ? '@' : (flag == 1) ? 'L' : 'R');
  printf("{%p, %zu}, %p\n", node, node->height, *as(avl_item(node), any_t*));

  if (node->left) {
    __avl_debug(node->left, depth + 4, 1);
  }
  if (node->right) {
    __avl_debug(node->right, depth + 4, 2);
  }
}

void avl_debug(any_t _self) {
  avl_t* self = self_of(_self);

  u_assert(self == nullptr);

  printf("avl is %p, nodes is %zu\n", self, self->len);

  __avl_debug(self->root, 0, 0);
  printf("\n");
}
#endif
