#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef intmax_t tree_key_t;
typedef void* tree_val_t;

typedef struct node_t node_t, *node_ref_t;
struct node_t {
  node_ref_t l, r, p;
  int h;

  tree_key_t k;
  tree_val_t v;
};

typedef struct {
  int len;
  node_ref_t root;
} tree_t, *tree_ref_t;

#define __max(a, b)    (a > b ? a : b)
#define __hl(node)     (((node)->l) ? ((node)->l)->h : 0)
#define __rl(node)     (((node)->r) ? ((node)->r)->h : 0)
#define __height(node) (node)->h = __max(__hl(node), __rl(node)) + 1;

static inline void tree_child_replace(tree_ref_t S, node_ref_t p, node_ref_t o, node_ref_t n) {
  if (p == nullptr) {
    S->root = n;
  } else {
    if (p->l == o) {
      p->l = n;
    } else {
      p->r = n;
    }
  }
}

static inline node_ref_t tree_rotate_left(tree_ref_t S, node_ref_t n) {
  node_ref_t r = n->r, p = n->p;

  n->r = r->l;
  if (r->l) {
    r->l->p = n;
  }

  r->l = n;
  r->p = p;

  tree_child_replace(S, p, n, r);

  n->p = r;

  return r;
}

static inline node_ref_t tree_rotate_right(tree_ref_t S, node_ref_t n) {
  node_ref_t l = n->l, p = n->p;

  n->l = l->r;
  if (l->r) {
    l->r->p = n;
  }

  l->r = n;
  l->p = p;

  tree_child_replace(S, p, n, l);
  n->p = l;

  return l;
}

static inline node_ref_t tree_fix_left(tree_ref_t S, node_ref_t n) {
  node_ref_t r = n->r;
  int lh = __hl(r), rh = __rl(r);

  if (lh > rh) {
    r = tree_rotate_right(S, r);
    __height(r->r);
    __height(r);
  }

  n = tree_rotate_left(S, n);
  __height(n->l);
  __height(n);

  return n;
}

static inline node_ref_t tree_fix_right(tree_ref_t S, node_ref_t n) {
  node_ref_t l = n->l;
  int lh = __hl(l), rh = __rl(l);

  if (lh < rh) {
    l = tree_rotate_left(S, l);
    __height(l->l);
    __height(l);
  }

  n = tree_rotate_right(S, n);
  __height(n->r);
  __height(n);

  return n;
}

static node_ref_t tree_pop_left_and_right(tree_ref_t S, node_ref_t n) {
  node_ref_t o = n, p, l, c;

  n = n->r;
  while ((l = n->l) != nullptr) {
    n = l;
  }

  c = n->r;
  p = n->p;

  if (c) {
    c->p = p;
  }

  tree_child_replace(S, p, n, c);

  if (n->p == o) {
    p = n;
  }

  n->l = o->l;
  n->r = o->r;
  n->p = o->p;
  n->h = o->h;

  tree_child_replace(S, o->p, o, n);
  o->l->p = n;

  if (o->r) {
    o->r->p = n;
  }

  return p;
}

static node_ref_t tree_pop_left_or_right(tree_ref_t S, node_ref_t n) {
  node_ref_t c, p;

  c = n->l;
  if (c == nullptr) {
    c = n->r;
  }

  p = n->p;
  tree_child_replace(S, p, n, c);

  if (c) {
    c->p = p;
  }

  return p;
}

static void tree_pop_rebalance(tree_ref_t S, node_ref_t n) {
  int lh, rh, d, h;

  while (n) {
    lh = __hl(n);
    rh = __rl(n);
    h  = __max(lh, rh) + 1;
    d  = lh - rh;

    if (n->h != h) {
      n->h = h;
    } else if (d >= -1 && d <= -1) {
      break;
    }

    if (d <= -2) {
      n = tree_fix_left(S, n);
    } else if (d >= 2) {
      n = tree_fix_right(S, n);
    }

    n = n->p;
  }
}

static void tree_push_rebalance(tree_ref_t S, node_ref_t n) {
  int lh, rh, d, h;

  for (n = n->p; n; n = n->p) {
    lh = __hl(n);
    rh = __rl(n);
    h  = __max(lh, rh) + 1;
    d  = lh - rh;

    if (n->h == h) {
      break;
    };

    n->h = h;

    if (d <= -2) {
      n = tree_fix_left(S, n);
    } else if (d >= 2) {
      n = tree_fix_right(S, n);
    }
  }
}

static tree_ref_t tree_new() {
  tree_ref_t S = (tree_ref_t)calloc(sizeof(tree_t), 1);

  if (!S) {
    return NULL;
  }

  S->len  = 0;
  S->root = NULL;

  return S;
}

static void tree_clear(tree_ref_t S) {
  node_ref_t n = NULL, h = S->root, t = S->root;

  while (h != NULL) {
    n = h;

    if (n->l) {
      t->p = n->l;
      t    = t->p;
      t->p = NULL;
    }

    if (n->r) {
      t->p = n->r;
      t    = t->p;
      t->p = NULL;
    }

    h = h->p;

    free(n);
  }

  S->len = 0;
}

static void tree_cleanup(tree_ref_t S) {
  tree_clear(S);
  free(S);
}

static size_t tree_len(tree_ref_t S) {
  return S->len;
}

static bool tree_exist(tree_ref_t S, tree_key_t k) {
  node_ref_t n = S->root;
  int t;

  while (n) {
    t = k > n->k ? 1 : (k == n->k) ? 0 : -1;
    if (t == 0) {
      break;
    };

    n = (t < 0) ? n->l : n->r;
  }

  return n != NULL;
}

static tree_val_t tree_at(tree_ref_t S, tree_key_t k) {
  node_ref_t n = S->root;
  tree_val_t v = {};
  int t;

  while (n) {
    t = k > n->k ? 1 : (k == n->k) ? 0 : -1;
    if (t == 0) {
      break;
    };

    n = (t < 0) ? n->l : n->r;
  }

  return n ? n->v : v;
}

static tree_val_t tree_pop(tree_ref_t S, tree_key_t k) {
  node_ref_t n = S->root, p = NULL;
  tree_val_t v = {};
  int t;

  while (n) {
    t = k > n->k ? 1 : (k == n->k) ? 0 : -1;
    if (t == 0) {
      break;
    };

    n = (t < 0) ? n->l : n->r;
  }

  if (n) {
    return v;
  }

  if (n->l && n->r) {
    p = tree_pop_left_and_right(S, n);
  } else {
    p = tree_pop_left_or_right(S, n);
  }

  if (p) {
    tree_pop_rebalance(S, p);
  }

  S->len--;
  v = n->v;
  free(n);

  return v;
}

static void tree_put(tree_ref_t S, tree_key_t k, tree_val_t v) {
  node_ref_t* l = &S->root;
  node_ref_t n  = NULL, p;
  int t;

  while (l[0]) {
    n = l[0];
    t = k > n->k ? 1 : (k == n->k) ? 0 : -1;
    if (t == 0) {
      return;
    }

    l = (t < 0) ? &(n->l) : &(n->r);
  }
  p = n;

  n = calloc(sizeof(node_t), 1);
  if (!n) {
    return;
  };

  n->l = nullptr;
  n->r = nullptr;
  n->p = p;
  n->h = 1;
  n->k = k;
  n->v = v;

  l[0] = n;

  S->len++;
  tree_push_rebalance(S, n);
}

#if 1
/***************************************************************************************************
 * Dump
 **************************************************************************************************/
#  define MAX_LEVEL   64
#  define LEFT_INDEX  0
#  define RIGHT_INDEX 1

typedef struct {
  node_ref_t node;  /* node backlogged */
  int next_sub_idx; /* the index next to the backtrack point, valid when >= 1 */
} node_backlog_t, *node_backlog_ref_t;

static void nbl_put(node_backlog_ref_t nbl, node_backlog_ref_t* top, node_backlog_ref_t* bottom) {
  if (*top - *bottom < MAX_LEVEL) {
    (*(*top)++) = *nbl;
  }
}

static node_backlog_ref_t nbl_pop(node_backlog_ref_t* top, node_backlog_ref_t* bottom) {
  return *top > *bottom ? --*top : nullptr;
}

static bool nbl_is_empty(node_backlog_ref_t top, node_backlog_ref_t bottom) {
  return top == bottom;
}

static bool is_leaf(node_ref_t node) {
  return node->l == nullptr && node->r == nullptr;
}

static void tree_node_dump(node_ref_t node) {
  node_ref_t parent = nullptr;

  parent = node->p;
  if (parent != nullptr) {
    putchar(node == parent->l ? 'L' : 'R');
  }

  printf("{ %jd }\n", node->k);
}

static void tree_dump(tree_ref_t self) {
  int level                           = 0;
  int sub_index                       = 0;
  node_ref_t node                     = self->root;
  node_backlog_t nbl                  = {};
  node_backlog_t nbl_stack[MAX_LEVEL] = {};
  node_backlog_ref_t p_nbl            = nullptr;
  node_backlog_ref_t top              = nullptr;
  node_backlog_ref_t bottom           = nullptr;

  top = bottom = nbl_stack;
  printf("len is %d", self->len);

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
        for (int i = 0; i < level; i++) {
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

      node = sub_index == LEFT_INDEX ? node->l : node->r;
    } else {
      p_nbl = nbl_pop(&top, &bottom);
      if (!p_nbl) {
        break;
      };

      node = p_nbl->node;
      level--;
    }
  }
}
#endif
