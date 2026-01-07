/* clang-format on */
#pragma once

#ifndef U_TREE_H__
#  define U_TREE_H__

#  include <stddef.h>
#  include <stdio.h>
#  include <stdlib.h>

#  ifdef __cplusplus
extern "C" {
#  endif

#  define U_TREE_DEFINE(prefix, alloc, free, ...)                                                  \
    typedef struct prefix##node_t* prefix##node_t;                                                 \
    struct prefix##node_t {                                                                        \
      prefix##node_t l, r, p;                                                                      \
      int h;                                                                                       \
      __VA_ARGS__;                                                                                 \
      char u[0];                                                                                   \
    };                                                                                             \
                                                                                                   \
    typedef int (*prefix##tree_cmp_fn)(prefix##node_t, prefix##node_t);                            \
                                                                                                   \
    typedef struct {                                                                               \
      int len;                                                                                     \
      prefix##tree_cmp_fn cmp_fn;                                                                  \
      prefix##node_t root;                                                                         \
    }* prefix##tree_t;                                                                             \
                                                                                                   \
    static inline int __##prefix##tree_left_height(prefix##node_t n) {                             \
      return n->l ? n->l->h : 0;                                                                   \
    }                                                                                              \
                                                                                                   \
    static inline int __##prefix##tree_right_height(prefix##node_t n) {                            \
      return n->r ? n->r->h : 0;                                                                   \
    }                                                                                              \
                                                                                                   \
    static inline void __##prefix##tree_set_height(prefix##node_t n) {                             \
      int lh = __##prefix##tree_left_height(n);                                                    \
      int rh = __##prefix##tree_right_height(n);                                                   \
                                                                                                   \
      n->h = (lh > rh ? lh : rh) + 1;                                                              \
    }                                                                                              \
                                                                                                   \
    static inline void __##prefix##tree_child_replace(prefix##tree_t T,                            \
                                                      prefix##node_t p,                            \
                                                      prefix##node_t o,                            \
                                                      prefix##node_t n) {                          \
      if (p == NULL) {                                                                             \
        T->root = n;                                                                               \
      } else {                                                                                     \
        if (p->l == o) {                                                                           \
          p->l = n;                                                                                \
        } else {                                                                                   \
          p->r = n;                                                                                \
        }                                                                                          \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t __##prefix##tree_rotate_left(prefix##tree_t T,                    \
                                                              prefix##node_t n) {                  \
      prefix##node_t r = n->r, p = n->p;                                                           \
                                                                                                   \
      n->r = r->l;                                                                                 \
      if (r->l) {                                                                                  \
        r->l->p = n;                                                                               \
      }                                                                                            \
                                                                                                   \
      r->l = n;                                                                                    \
      r->p = p;                                                                                    \
                                                                                                   \
      __##prefix##tree_child_replace(T, p, n, r);                                                  \
                                                                                                   \
      n->p = r;                                                                                    \
                                                                                                   \
      return r;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t __##prefix##tree_rotate_right(prefix##tree_t T,                   \
                                                               prefix##node_t n) {                 \
      prefix##node_t l = n->l, p = n->p;                                                           \
                                                                                                   \
      n->l = l->r;                                                                                 \
      if (l->r) {                                                                                  \
        l->r->p = n;                                                                               \
      }                                                                                            \
                                                                                                   \
      l->r = n;                                                                                    \
      l->p = p;                                                                                    \
                                                                                                   \
      __##prefix##tree_child_replace(T, p, n, l);                                                  \
                                                                                                   \
      n->p = l;                                                                                    \
                                                                                                   \
      return l;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t __##prefix##tree_fix_left(prefix##tree_t T, prefix##node_t n) {   \
      prefix##node_t r = n->r;                                                                     \
      int lh = __##prefix##tree_left_height(r), rh = __##prefix##tree_right_height(r);             \
                                                                                                   \
      if (lh > rh) {                                                                               \
        r = __##prefix##tree_rotate_right(T, r);                                                   \
        __##prefix##tree_set_height(r->r);                                                         \
        __##prefix##tree_set_height(r);                                                            \
      }                                                                                            \
                                                                                                   \
      n = __##prefix##tree_rotate_left(T, n);                                                      \
      __##prefix##tree_set_height(n->l);                                                           \
      __##prefix##tree_set_height(n);                                                              \
                                                                                                   \
      return n;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t __##prefix##tree_fix_right(prefix##tree_t T, prefix##node_t n) {  \
      prefix##node_t l = n->l;                                                                     \
      int lh = __##prefix##tree_left_height(l), rh = __##prefix##tree_right_height(l);             \
                                                                                                   \
      if (lh < rh) {                                                                               \
        l = __##prefix##tree_rotate_left(T, l);                                                    \
        __##prefix##tree_set_height(l->l);                                                         \
        __##prefix##tree_set_height(l);                                                            \
      }                                                                                            \
                                                                                                   \
      n = __##prefix##tree_rotate_right(T, n);                                                     \
      __##prefix##tree_set_height(n->r);                                                           \
      __##prefix##tree_set_height(n);                                                              \
                                                                                                   \
      return n;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t __##prefix##tree_pop_left_and_right(prefix##tree_t T,             \
                                                                     prefix##node_t n) {           \
      prefix##node_t o = n, p, l, c;                                                               \
                                                                                                   \
      n = n->r;                                                                                    \
      while ((l = n->l)) {                                                                         \
        n = l;                                                                                     \
      }                                                                                            \
                                                                                                   \
      c = n->r;                                                                                    \
      p = n->p;                                                                                    \
                                                                                                   \
      if (c) {                                                                                     \
        c->p = p;                                                                                  \
      }                                                                                            \
                                                                                                   \
      __##prefix##tree_child_replace(T, p, n, c);                                                  \
      if (n->p == o) {                                                                             \
        p = n;                                                                                     \
      }                                                                                            \
                                                                                                   \
      n->l = o->l;                                                                                 \
      n->r = o->r;                                                                                 \
      n->p = o->p;                                                                                 \
      n->h = o->h;                                                                                 \
                                                                                                   \
      __##prefix##tree_child_replace(T, o->p, o, n);                                               \
                                                                                                   \
      o->l->p = n;                                                                                 \
      if (o->r) {                                                                                  \
        o->r->p = n;                                                                               \
      }                                                                                            \
                                                                                                   \
      return p;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t __##prefix##tree_pop_left_or_right(prefix##tree_t T,              \
                                                                    prefix##node_t n) {            \
      prefix##node_t c, p;                                                                         \
                                                                                                   \
      c = n->l;                                                                                    \
      if (!c) {                                                                                    \
        c = n->r;                                                                                  \
      }                                                                                            \
                                                                                                   \
      p = n->p;                                                                                    \
      __##prefix##tree_child_replace(T, p, n, c);                                                  \
                                                                                                   \
      if (c) {                                                                                     \
        c->p = p;                                                                                  \
      }                                                                                            \
                                                                                                   \
      return p;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline void __##prefix##tree_pop_rebalance(prefix##tree_t T, prefix##node_t n) {        \
      int lh, rh, d, h;                                                                            \
                                                                                                   \
      while (n) {                                                                                  \
        lh = __##prefix##tree_left_height(n);                                                      \
        rh = __##prefix##tree_right_height(n);                                                     \
        h  = (lh > rh ? lh : rh) + 1;                                                              \
        d  = lh - rh;                                                                              \
                                                                                                   \
        if (n->h != h) {                                                                           \
          n->h = h;                                                                                \
        } else if (d >= -1 && d <= 1) {                                                            \
          break;                                                                                   \
        }                                                                                          \
                                                                                                   \
        if (d <= -2) {                                                                             \
          n = __##prefix##tree_fix_left(T, n);                                                     \
        } else if (d >= 2) {                                                                       \
          n = __##prefix##tree_fix_right(T, n);                                                    \
        }                                                                                          \
                                                                                                   \
        n = n->p;                                                                                  \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    static inline void __##prefix##tree_put_rebalance(prefix##tree_t T, prefix##node_t n) {        \
      int lh, rh, d, h;                                                                            \
                                                                                                   \
      for (n = n->p; n; n = n->p) {                                                                \
        lh = __##prefix##tree_left_height(n);                                                      \
        rh = __##prefix##tree_right_height(n);                                                     \
        h  = (lh > rh ? lh : rh) + 1;                                                              \
        d  = lh - rh;                                                                              \
                                                                                                   \
        if (n->h == h) {                                                                           \
          break;                                                                                   \
        }                                                                                          \
                                                                                                   \
        n->h = h;                                                                                  \
                                                                                                   \
        if (d <= -2) {                                                                             \
          n = __##prefix##tree_fix_left(T, n);                                                     \
        } else if (d >= 2) {                                                                       \
          n = __##prefix##tree_fix_right(T, n);                                                    \
        }                                                                                          \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    static inline prefix##tree_t prefix##tree_new(prefix##tree_cmp_fn cmp_fn) {                    \
      prefix##tree_t T = (prefix##tree_t)alloc(sizeof(*T));                                        \
      T->len           = 0;                                                                        \
      T->root          = NULL;                                                                     \
      T->cmp_fn        = cmp_fn;                                                                   \
      return T;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline void prefix##tree_del(prefix##tree_t T) {                                        \
      free(T);                                                                                     \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t prefix##tree_new_node(size_t s) {                                 \
      prefix##node_t n = alloc(sizeof(*n) + s);                                                    \
      n->h             = 1;                                                                        \
      n->p = n->l = n->r = NULL;                                                                   \
                                                                                                   \
      return n;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline void prefix##tree_del_node(prefix##node_t n) {                                   \
      free(n);                                                                                     \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t prefix##tree_at(prefix##tree_t T, prefix##node_t n) {             \
      prefix##node_t l = T->root;                                                                  \
      int t;                                                                                       \
                                                                                                   \
      while (l) {                                                                                  \
        t = T->cmp_fn(n, l);                                                                       \
        if (t == 0) {                                                                              \
          return l;                                                                                \
        }                                                                                          \
                                                                                                   \
        l = (t < 0) ? l->l : l->r;                                                                 \
      }                                                                                            \
                                                                                                   \
      return NULL;                                                                                 \
    }                                                                                              \
                                                                                                   \
    static inline void prefix##tree_pop(prefix##tree_t T, prefix##node_t n) {                      \
      prefix##node_t p = NULL;                                                                     \
      p                = (n->l && n->r ? __##prefix##tree_pop_left_and_right :                     \
                                         __##prefix##tree_pop_left_or_right)(T, n);                \
                                                                                                   \
      if (p) {                                                                                     \
        __##prefix##tree_pop_rebalance(T, p);                                                      \
      }                                                                                            \
                                                                                                   \
      T->len--;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline bool prefix##tree_put(prefix##tree_t T, prefix##node_t n) {                      \
      prefix##node_t *l = &T->root, p = NULL;                                                      \
      int t = 0;                                                                                   \
                                                                                                   \
      while (l[0]) {                                                                               \
        p = l[0];                                                                                  \
        t = T->cmp_fn(n, p);                                                                       \
        if (t == 0) {                                                                              \
          return false;                                                                            \
        }                                                                                          \
                                                                                                   \
        l = (t < 0) ? &p->l : &p->r;                                                               \
      }                                                                                            \
                                                                                                   \
      n->p = p;                                                                                    \
      l[0] = n;                                                                                    \
                                                                                                   \
      T->len++;                                                                                    \
                                                                                                   \
      __##prefix##tree_put_rebalance(T, n);                                                        \
                                                                                                   \
      return true;                                                                                 \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t prefix##tree_tear(prefix##tree_t T, prefix##node_t* next) {       \
      prefix##node_t n = *next, p;                                                                 \
                                                                                                   \
      if (!n) {                                                                                    \
        if (!T->root) {                                                                            \
          return NULL;                                                                             \
        }                                                                                          \
        n = T->root;                                                                               \
      }                                                                                            \
                                                                                                   \
      while (true) {                                                                               \
        if (n->l) {                                                                                \
          n = n->l;                                                                                \
        } else if (n->r) {                                                                         \
          n = n->r;                                                                                \
        } else {                                                                                   \
          break;                                                                                   \
        }                                                                                          \
      }                                                                                            \
                                                                                                   \
      p = n->p;                                                                                    \
      if (!p) {                                                                                    \
        *next   = NULL;                                                                            \
        T->root = NULL, T->len = 0;                                                                \
        return n;                                                                                  \
      }                                                                                            \
                                                                                                   \
      if (p->l == n) {                                                                             \
        p->l = NULL;                                                                               \
      } else {                                                                                     \
        p->r = NULL;                                                                               \
      }                                                                                            \
                                                                                                   \
      n->h  = 0;                                                                                   \
      *next = p;                                                                                   \
                                                                                                   \
      return n;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t prefix##tree_first(prefix##tree_t T) {                            \
      prefix##node_t n = T->root;                                                                  \
      if (!n) {                                                                                    \
        return NULL;                                                                               \
      }                                                                                            \
                                                                                                   \
      while (n->l) {                                                                               \
        n = n->l;                                                                                  \
      }                                                                                            \
                                                                                                   \
      return n;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t prefix##tree_last(prefix##tree_t T) {                             \
      prefix##node_t n = T->root;                                                                  \
                                                                                                   \
      if (!n) {                                                                                    \
        return NULL;                                                                               \
      }                                                                                            \
                                                                                                   \
      while (n->r) {                                                                               \
        n = n->r;                                                                                  \
      }                                                                                            \
                                                                                                   \
      return n;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t prefix##tree_next(prefix##node_t n) {                             \
      prefix##node_t last;                                                                         \
                                                                                                   \
      if (n) {                                                                                     \
        if (n->r) {                                                                                \
          n = n->r;                                                                                \
          while (n->l) {                                                                           \
            n = n->l;                                                                              \
          }                                                                                        \
        } else {                                                                                   \
          while (true) {                                                                           \
            last = n;                                                                              \
            n    = n->p;                                                                           \
                                                                                                   \
            if (!n) {                                                                              \
              break;                                                                               \
            }                                                                                      \
                                                                                                   \
            if (n->l == last) {                                                                    \
              break;                                                                               \
            }                                                                                      \
          }                                                                                        \
        }                                                                                          \
      }                                                                                            \
                                                                                                   \
      return n;                                                                                    \
    }                                                                                              \
                                                                                                   \
    static inline prefix##node_t prefix##tree_prev(prefix##node_t n) {                             \
      prefix##node_t last;                                                                         \
                                                                                                   \
      if (n) {                                                                                     \
        if (n->l) {                                                                                \
          n = n->l;                                                                                \
          while (n->r) {                                                                           \
            n = n->r;                                                                              \
          }                                                                                        \
        } else {                                                                                   \
          while (true) {                                                                           \
            last = n;                                                                              \
            n    = n->p;                                                                           \
                                                                                                   \
            if (!n) {                                                                              \
              break;                                                                               \
            }                                                                                      \
                                                                                                   \
            if (n->r == last) {                                                                    \
              break;                                                                               \
            }                                                                                      \
          }                                                                                        \
        }                                                                                          \
      }                                                                                            \
                                                                                                   \
      return n;                                                                                    \
    }                                                                                              \
                                                                                                   \
    typedef void (*prefix##tree_node_dump_fn)(prefix##node_t);                                     \
                                                                                                   \
    typedef struct {                                                                               \
      prefix##node_t n;                                                                            \
      int i;                                                                                       \
    } prefix##tree_nodebl_t, *prefix##tree_nodebl_ref_T;                                           \
                                                                                                   \
    static void __##prefix##tree_nbl_put(prefix##tree_nodebl_ref_T nbl,                            \
                                         prefix##tree_nodebl_ref_T* top,                           \
                                         prefix##tree_nodebl_ref_T* bottom) {                      \
      if (*top - *bottom < 64) {                                                                   \
        (*(*top)++) = *nbl;                                                                        \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    static prefix##tree_nodebl_ref_T __##prefix##tree_nbl_pop(prefix##tree_nodebl_ref_T* top,      \
                                                              prefix##tree_nodebl_ref_T* bottom) { \
      return *top > *bottom ? --*top : NULL;                                                       \
    }                                                                                              \
                                                                                                   \
    static bool __##prefix##tree_node_is_leaf(prefix##node_t node) {                               \
      return node->l == NULL && node->r == NULL;                                                   \
    }                                                                                              \
                                                                                                   \
    static void __##prefix##tree_node_dump(prefix##node_t node,                                    \
                                           prefix##tree_node_dump_fn dump_fn) {                    \
      prefix##node_t parent = node->p;                                                             \
                                                                                                   \
      if (parent != NULL) {                                                                        \
        printf("%c: ", node == parent->l ? 'L' : 'R');                                             \
      }                                                                                            \
                                                                                                   \
      dump_fn(node);                                                                               \
      printf("\n");                                                                                \
    }                                                                                              \
                                                                                                   \
    static void prefix##tree_dump(mtree_t self, prefix##tree_node_dump_fn dump_fn) {               \
      int level           = 0, sub_index;                                                          \
      prefix##node_t node = self->root;                                                            \
      prefix##tree_nodebl_t nbl, nbl_stack[64] = {};                                               \
      prefix##tree_nodebl_ref_T p_nbl = NULL, top = nbl_stack, bottom = nbl_stack;                 \
                                                                                                   \
      printf("%p len is %d, height is %d\n", self, self->len, self->root->h);                      \
                                                                                                   \
      while (true) {                                                                               \
        if (node != NULL) {                                                                        \
          sub_index = p_nbl != NULL ? p_nbl->i : 0;                                                \
          p_nbl     = NULL;                                                                        \
                                                                                                   \
          if (__##prefix##tree_node_is_leaf(node) || sub_index == 1) {                             \
            nbl.n = NULL;                                                                          \
            nbl.i = 0;                                                                             \
          } else {                                                                                 \
            nbl.n = node;                                                                          \
            nbl.i = 1;                                                                             \
          }                                                                                        \
                                                                                                   \
          __##prefix##tree_nbl_put(&nbl, &top, &bottom);                                           \
          level++;                                                                                 \
                                                                                                   \
          /* draw lines as long as sub_idx is the first one */                                     \
          if (sub_index == 0) {                                                                    \
            for (int i = 0; i < level; i++) {                                                      \
              if (i == level - 1) {                                                                \
                printf("%-4s", "+---");                                                            \
              } else {                                                                             \
                printf("%-4s", nbl_stack[i - 1].n != NULL ? "|" : " ");                            \
              }                                                                                    \
            }                                                                                      \
                                                                                                   \
            __##prefix##tree_node_dump(node, dump_fn);                                             \
          }                                                                                        \
                                                                                                   \
          node = sub_index == 0 ? node->l : node->r;                                               \
        } else {                                                                                   \
          p_nbl = __##prefix##tree_nbl_pop(&top, &bottom);                                         \
                                                                                                   \
          if (!p_nbl) {                                                                            \
            break;                                                                                 \
          };                                                                                       \
                                                                                                   \
          node = p_nbl->n;                                                                         \
          level--;                                                                                 \
        }                                                                                          \
      }                                                                                            \
    }

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_TREE_H__ */
