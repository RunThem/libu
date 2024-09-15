/* clang-format on */
#pragma once

#ifndef U_PRI_H__
#  define U_PRI_H__

#  include <stddef.h>
#  include <stdio.h>
#  include <stdlib.h>

#  ifdef __cplusplus
extern "C" {
#  endif

#  ifndef U_PRI_ALLOC
#    define U_PRI_ALLOC(size) calloc(size, 1)
#  endif

#  ifndef U_PRI_FREE
#    define U_PRI_FREE(ptr) free(ptr)
#  endif

#  ifndef U_PRI_TREE_USERDATA
#    define U_PRI_TREE_USERDATA
#  endif

typedef struct tnode_t* tnode_t;
struct tnode_t {
  tnode_t l, r, p;
  int h;
  U_PRI_TREE_USERDATA;
  char u[0];
};

typedef int (*tree_cmp_fn)(tnode_t x, tnode_t y);

typedef struct {
  int len;
  tree_cmp_fn cmp_fn;
  tnode_t root;
}* tree_t;

static inline int __tree_left_height(tnode_t n) {
  return n->l ? n->l->h : 0;
}

static inline int __tree_right_height(tnode_t n) {
  return n->r ? n->r->h : 0;
}

static inline void __tree_set_height(tnode_t n) {
  int lh = __tree_left_height(n);
  int rh = __tree_right_height(n);

  n->h = (lh > rh ? lh : rh) + 1;
}

static inline void __tree_child_replace(tree_t T, tnode_t p, tnode_t o, tnode_t n) {
  if (p == NULL) {
    T->root = n;
  } else {
    if (p->l == o) {
      p->l = n;
    } else {
      p->r = n;
    }
  }
}

static inline tnode_t __tree_rotate_left(tree_t T, tnode_t n) {
  tnode_t r = n->r, p = n->p;

  n->r = r->l;
  if (r->l) {
    r->l->p = n;
  }

  r->l = n;
  r->p = p;

  __tree_child_replace(T, p, n, r);

  n->p = r;

  return r;
}

static inline tnode_t __tree_rotate_right(tree_t T, tnode_t n) {
  tnode_t l = n->l, p = n->p;

  n->l = l->r;
  if (l->r) {
    l->r->p = n;
  }

  l->r = n;
  l->p = p;

  __tree_child_replace(T, p, n, l);

  n->p = l;

  return l;
}

static inline tnode_t __tree_fix_left(tree_t T, tnode_t n) {
  tnode_t r = n->r;
  int lh = __tree_left_height(r), rh = __tree_right_height(r);

  if (lh > rh) {
    r = __tree_rotate_right(T, r);
    __tree_set_height(r->r);
    __tree_set_height(r);
  }

  n = __tree_rotate_left(T, n);
  __tree_set_height(n->l);
  __tree_set_height(n);

  return n;
}

static inline tnode_t __tree_fix_right(tree_t T, tnode_t n) {
  tnode_t l = n->l;
  int lh = __tree_left_height(l), rh = __tree_right_height(l);

  if (lh < rh) {
    l = __tree_rotate_left(T, l);
    __tree_set_height(l->l);
    __tree_set_height(l);
  }

  n = __tree_rotate_right(T, n);
  __tree_set_height(n->r);
  __tree_set_height(n);

  return n;
}

static inline tnode_t __tree_pop_left_and_right(tree_t T, tnode_t n) {
  tnode_t o = n, p, l, c;

  n = n->r;
  while ((l = n->l)) {
    n = l;
  }

  c = n->r;
  p = n->p;

  if (c) {
    c->p = p;
  }

  __tree_child_replace(T, p, n, c);
  if (n->p == o) {
    p = n;
  }

  n->l = o->l;
  n->r = o->r;
  n->p = o->p;
  n->h = o->h;

  __tree_child_replace(T, o->p, o, n);

  o->l->p = n;
  if (o->r) {
    o->r->p = n;
  }

  return p;
}

static inline tnode_t __tree_pop_left_or_right(tree_t T, tnode_t n) {
  tnode_t c, p;

  c = n->l;
  if (!c) {
    c = n->r;
  }

  p = n->p;
  __tree_child_replace(T, p, n, c);

  if (c) {
    c->p = p;
  }

  return p;
}

static inline void __tree_pop_rebalance(tree_t T, tnode_t n) {
  int lh, rh, d, h;

  while (n) {
    lh = __tree_left_height(n);
    rh = __tree_right_height(n);
    h  = (lh > rh ? lh : rh) + 1;
    d  = lh - rh;

    if (n->h != h) {
      n->h = h;
    } else if (d >= -1 && d <= -1) {
      break;
    }

    if (d <= -2) {
      n = __tree_fix_left(T, n);
    } else if (d >= 2) {
      n = __tree_fix_right(T, n);
    }

    n = n->p;
  }
}

static inline void __tree_put_rebalance(tree_t T, tnode_t n) {
  int lh, rh, d, h;

  for (n = n->p; n; n = n->p) {
    lh = __tree_left_height(n);
    rh = __tree_right_height(n);
    h  = (lh > rh ? lh : rh) + 1;
    d  = lh - rh;

    if (n->h == h) {
      break;
    }

    n->h = h;

    if (d <= -2) {
      n = __tree_fix_left(T, n);
    } else if (d >= 2) {
      n = __tree_fix_right(T, n);
    }
  }
}

static inline tree_t tree_new(tree_cmp_fn cmp_fn) {
  tree_t T  = (tree_t)U_PRI_ALLOC(sizeof(*T));
  T->len    = 0;
  T->root   = NULL;
  T->cmp_fn = cmp_fn;
  return T;
}

static inline void tree_del(tree_t T) {
  U_PRI_FREE(T);
}

static inline tnode_t tree_new_node(size_t s) {
  tnode_t n = U_PRI_ALLOC(sizeof(*n) + s);
  n->h      = 1;
  n->p = n->l = n->r = NULL;

  return n;
}

static inline void tree_del_node(tnode_t n) {
  U_PRI_FREE(n);
}

static inline tnode_t tree_at(tree_t T, tnode_t n) {
  tnode_t l = T->root;
  int t;

  while (l) {
    t = T->cmp_fn(n, l);
    if (t == 0) {
      return l;
    }

    l = (t < 0) ? l->l : l->r;
  }

  return NULL;
}

static inline void tree_pop(tree_t T, tnode_t n) {
  tnode_t p = NULL;
  p         = (n->l && n->r ? __tree_pop_left_and_right : __tree_pop_left_or_right)(T, n);

  if (p) {
    __tree_pop_rebalance(T, p);
  }

  T->len--;
}

static inline bool tree_put(tree_t T, tnode_t n) {
  tnode_t *l = &T->root, p = NULL;
  int t = 0;

  while (l[0]) {
    p = l[0];
    t = T->cmp_fn(n, p);
    if (t == 0) {
      return false;
    }

    l = (t < 0) ? &p->l : &p->r;
  }

  n->p = p;
  l[0] = n;

  T->len++;

  __tree_put_rebalance(T, n);

  return true;
}

static inline tnode_t tree_tear(tree_t T, tnode_t* next) {
  tnode_t n = *next, p;

  if (!n) {
    if (!T->root) {
      return NULL;
    }
    n = T->root;
  }

  while (true) {
    if (n->l) {
      n = n->l;
    } else if (n->r) {
      n = n->r;
    } else {
      break;
    }
  }

  p = n->p;
  if (!p) {
    *next   = NULL;
    T->root = NULL, T->len = 0;
    return n;
  }

  if (p->l == n) {
    p->l = NULL;
  } else {
    p->r = NULL;
  }

  n->h  = 0;
  *next = p;

  return n;
}

static inline tnode_t tree_first(tree_t T) {
  tnode_t n = T->root;
  if (!n) {
    return NULL;
  }

  while (n->l) {
    n = n->l;
  }

  return n;
}

static inline tnode_t tree_last(tree_t T) {
  tnode_t n = T->root;

  if (!n) {
    return NULL;
  }

  while (n->r) {
    n = n->r;
  }

  return n;
}

static inline tnode_t tree_next(tnode_t n) {
  tnode_t last;

  if (n) {
    if (n->r) {
      n = n->r;
      while (n->l) {
        n = n->l;
      }
    } else {
      while (true) {
        last = n;
        n    = n->p;

        if (!n) {
          break;
        }

        if (n->l == last) {
          break;
        }
      }
    }
  }

  return n;
}

static inline tnode_t tree_prev(tnode_t n) {
  tnode_t last;

  if (n) {
    if (n->l) {
      n = n->l;
      while (n->r) {
        n = n->r;
      }
    } else {
      while (true) {
        last = n;
        n    = n->p;

        if (!n) {
          break;
        }

        if (n->r == last) {
          break;
        }
      }
    }
  }

  return n;
}

#  ifdef U_PRI_DEBUG
static inline void tree_node_dump(tnode_t n);

typedef struct {
  tnode_t n;
  int i;
} tnodebl_t, *tnodebl_ref_t;

static void nbl_put(tnodebl_ref_t nbl, tnodebl_ref_t* top, tnodebl_ref_t* bottom) {
  if (*top - *bottom < 64) {
    (*(*top)++) = *nbl;
  }
}

static tnodebl_ref_t nbl_pop(tnodebl_ref_t* top, tnodebl_ref_t* bottom) {
  return *top > *bottom ? --*top : NULL;
}

static bool nbl_is_empty(tnodebl_ref_t top, tnodebl_ref_t bottom) {
  return top == bottom;
}

static bool is_leaf(tnode_t node) {
  return node->l == NULL && node->r == NULL;
}

static void nbl_dump(tnode_t node) {
  tnode_t parent = nullptr;
  parent         = node->p;
  if (parent != NULL) {
    putchar(node == parent->l ? 'L' : 'R');
  }
  tree_node_dump(node);
}

static void tree_dump(tree_t self) {
  int level    = 0, sub_index;
  tnode_t node = self->root;
  tnodebl_t nbl, nbl_stack[64] = {};
  tnodebl_ref_t p_nbl = NULL, top = NULL, bottom = NULL;
  top = bottom = nbl_stack;
  printf("%p len is %d, height is %d\n", self, self->len, self->root->h);
  while (true) {
    if (node != NULL) {
      sub_index = p_nbl != NULL ? p_nbl->i : 0;
      p_nbl     = NULL;
      if (is_leaf(node) || sub_index == 1) {
        nbl.n = NULL;
        nbl.i = 0;
      } else {
        nbl.n = node;
        nbl.i = 1;
      }
      nbl_put(&nbl, &top, &bottom);
      level++;
      /* draw lines as long as sub_idx is the first one */
      if (sub_index == 0) {
        for (int i = 0; i < level; i++) {
          if (i == level - 1) {
            printf("%-4s", "+---");
          } else {
            printf("%-4s", nbl_stack[i - 1].n != NULL ? "|" : " ");
          }
        }
        nbl_dump(node);
      }
      node = sub_index == 0 ? node->l : node->r;
    } else {
      p_nbl = nbl_pop(&top, &bottom);
      if (!p_nbl) {
        break;
      };
      node = p_nbl->n;
      level--;
    }
  }
}
#  endif

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_PRI_H__ */
