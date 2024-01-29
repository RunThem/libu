#include <u/core.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct node_t node_t;
struct node_t {
  node_t* left;
  node_t* right;
  node_t* parent;
  size_t height;
};

typedef struct avl_t avl_t;
struct avl_t {
  bool flags[4];
  size_t ksize;
  size_t vsize;
  size_t len;

  u_cmp_fn cmp_fn;

  node_t* root;
  node_t* iter;
  node_t* free_nodes;
};

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
static node_t* avl_node(avl_t* self, node_t* parent, any_t key, any_t val) {
  node_t* node = nullptr;

  if (self->free_nodes != nullptr) {
    node             = self->free_nodes;
    self->free_nodes = node->parent;
  } else {
    node = u_zalloc(sizeof(node_t) + self->ksize + self->vsize);
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

static void avl_child_replace(avl_t* self, node_t* parent, node_t* oldnode, node_t* newnode) {
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

static node_t* avl_rotate_left(avl_t* self, node_t* node) {
  node_t* right  = node->right;
  node_t* parent = node->parent;

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

static node_t* avl_rotate_right(avl_t* self, node_t* node) {
  node_t* left   = node->left;
  node_t* parent = node->parent;

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

static node_t* avl_fix_left(avl_t* self, node_t* node) {
  int lh        = 0;
  int rh        = 0;
  node_t* right = node->right;

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

static node_t* avl_fix_right(avl_t* self, node_t* node) {
  int lh       = 0;
  int rh       = 0;
  node_t* left = node->left;

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

static node_t* avl_pop_left_and_right(avl_t* self, node_t* node) {
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

static node_t* avl_pop_left_or_right(avl_t* self, node_t* node) {
  node_t* child  = nullptr;
  node_t* parent = nullptr;

  child = node->left;
  if (child == nullptr) {
    child = node->right;
  }

  parent = node->parent;
  avl_child_replace(self, parent, node, child);

  if (child) {
    child->parent = parent;
  }

  return parent;
}

static void avl_pop_rebalance(avl_t* self, node_t* node) {
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
      node = avl_fix_left(self, node);
    } else if (diff >= 2) {
      node = avl_fix_right(self, node);
    }

    node = node->parent;
  }
}

static void avl_push_rebalance(avl_t* self, node_t* node) {
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
      node = avl_fix_left(self, node);
    } else if (diff >= 2) {
      node = avl_fix_right(self, node);
    }
  }
}

static void avl_next(avl_t* self) {
  node_t* node = self->iter;
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

  self->iter = node;
}

static void avl_prev(avl_t* self) {
  node_t* node = self->iter;
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

  self->iter = node;
}

u_avl_t avl_new(size_t ksize, size_t vsize, u_cmp_fn cmp_fn) {
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

  return as(self, u_avl_t);

err:
  return nullptr;
}

void avl_clear(u_avl_t _self) {
  avl_t* self          = as(_self, avl_t*);
  node_t* node         = nullptr;
  u_vec(node_t*) nodes = u_new(nodes);

  u_check_nret(self->len == 0);

  u_put(nodes, -1, self->root);

  while (!u_isempty(nodes)) {
    node = u_at(nodes, 0);

    if (node->left) {
      u_put(nodes, -1, node->left);
    }
    if (node->right) {
      u_put(nodes, -1, node->right);
    }

    u_free(node);

    u_pop(nodes, 0);
  }

  u_cleanup(nodes);

  self->len = 0;
}

void avl_cleanup(u_avl_t _self) {
  avl_t* self  = as(_self, avl_t*);
  node_t* node = nullptr;

  u_check_nret(self == nullptr);

  avl_clear(_self);

  while (self->free_nodes != nullptr) {
    node             = self->free_nodes;
    self->free_nodes = node->parent;

    u_free(node);
  }

  u_free(self);
}

size_t avl_len(u_avl_t _self) {
  avl_t* self = as(_self, avl_t*);

  u_check_ret(self == nullptr, 0);

  return self->len;
}

bool avl_isexist(u_avl_t _self, any_t key) {
  avl_t* self  = as(_self, avl_t*);
  node_t* node = self->root;
  ret_t result = 0;

  u_check_ret(self == nullptr, false);
  u_check_ret(key == nullptr, false);

  while (node) {
    result = self->cmp_fn(key, key(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  return node != nullptr;
}

void avl_re(u_avl_t _self, any_t key, any_t val) {
  avl_t* self  = as(_self, avl_t*);
  node_t* node = self->root;
  ret_t result = 0;

  u_check_nret(self == nullptr);
  u_check_nret(key == nullptr);
  u_check_nret(val == nullptr);

  while (node) {
    result = self->cmp_fn(key, key(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  u_nret_if(node == nullptr, "node not exists.");

  memcpy(key(node), key, self->ksize);
  memcpy(val(node), val, self->vsize);
}

void avl_at(u_avl_t _self, any_t key, any_t val) {
  avl_t* self  = as(_self, avl_t*);
  node_t* node = self->root;
  ret_t result = 0;

  u_check_nret(self == nullptr);
  u_check_nret(key == nullptr);
  u_check_nret(val == nullptr);

  while (node) {
    result = self->cmp_fn(key, key(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  u_if(node == nullptr, "node not exists.") {
    bzero(key, self->ksize);
    bzero(val, self->vsize);
  }
  else {
    memcpy(key, key(node), self->ksize);
    memcpy(val, val(node), self->vsize);
  }
}

void avl_pop(u_avl_t _self, any_t key, any_t val) {
  avl_t* self    = as(_self, avl_t*);
  node_t* node   = self->root;
  node_t* parent = nullptr;
  ret_t result   = 0;

  u_check_nret(self == nullptr);
  u_check_nret(key == nullptr);
  u_check_nret(val == nullptr);

  while (node) {
    result = self->cmp_fn(key, key(node));
    if (result == 0) {
      break;
    }

    node = (result < 0) ? node->left : node->right;
  }

  if (node->left && node->right) {
    parent = avl_pop_left_and_right(self, node);
  } else {
    parent = avl_pop_left_or_right(self, node);
  }

  memcpy(key, key(node), self->ksize);
  memcpy(val, val(node), self->vsize);

  node->parent     = self->free_nodes;
  self->free_nodes = node;

  self->len--;

  if (parent) {
    avl_pop_rebalance(self, parent);
  }
}

void avl_put(u_avl_t _self, any_t key, any_t val) {
  avl_t* self    = as(_self, avl_t*);
  node_t** link  = &self->root;
  node_t* parent = nullptr;
  node_t* node   = nullptr;
  ret_t result   = 0;

  u_check_nret(self == nullptr);
  u_check_nret(key == nullptr);
  u_check_nret(val == nullptr);

  while (*link) {
    parent = *link;

    result = self->cmp_fn(key, key(parent));
    u_err_if(result == 0, "avl push node already exists.");

    link = (result < 0) ? &(parent->left) : &(parent->right);
  }

  node = avl_node(self, parent, key, val);
  u_mem_if(node);

  *link = node;
  self->len++;

  avl_push_rebalance(self, node);

  return;

err:
}

bool avl_for_init(u_avl_t _self, bool flag) {
  avl_t* self = as(_self, avl_t*);

  u_check_ret(self == nullptr, false);
  u_check_ret(self->len == 0, false);

  self->iter     = nullptr;
  self->flags[0] = !self->flags[0];
  self->flags[1] = flag;

  return self->flags[0];
}

bool avl_for(u_avl_t _self, any_t key, any_t val) {
  avl_t* self = as(_self, avl_t*);

  u_check_ret(self == nullptr, false);

  (self->flags[1] ? avl_next : avl_prev)(self);
  u_ret_if(self->iter == nullptr, false);

  memcpy(key, key(self->iter), self->ksize);
  memcpy(val, val(self->iter), self->vsize);

  return true;
}
