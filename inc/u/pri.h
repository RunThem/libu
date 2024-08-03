/* clang-format off */
#pragma once

#ifndef U_PRI_H__
#  define U_PRI_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#  ifdef __cplusplus
extern "C" {
#  endif

#  ifdef U_PRI_MACRO

/* 
 * TREE_DEFINE(intmax_t, void*, k > n->k ? 1 : (k == n->k ? 0 : -1))
 * */
#define TREE_DEFINE(K, V, cmp)                                                                     \
typedef struct node_t*__t;struct node_t{__t l,r,p;int h;K k;V v;};                                 \
typedef struct{int len;__t root;}*tree_t;                                                          \
extern void* calloc(size_t, size_t);extern void free(void*);                                       \
static inline int __lh(__t n){return n->l?n->l->h:0;}static inline int __rh(__t n){return n->r?n->r\
->h:0;}static inline int __max(int a,int b){return a>b?a:b;}static inline void __height(__t n){n->h\
=__max(__lh(n),__rh(n))+1;}static inline void __child_replace(tree_t S,__t p,__t o,__t n){if       \
(p==NULL){S->root=n;}else{if(p->l==o)p->l=n;else p->r=n;}}static inline __t __rotate_left(tree_t S,\
__t n){__t r=n->r,p=n->p;n->r=r->l;if(r->l)r->l->p=n;r->l=n;r->p=p;__child_replace(S,p,n,r);n->p=r;\
return r;}static inline __t __rotate_right(tree_t S,__t n){__t l=n->l,p=n->p;n->l=l->r;if(l->r)l->r\
->p=n;l->r=n;l->p=p;__child_replace(S,p,n,l);n->p=l;return l;}static inline __t __fix_left(tree_t S\
,__t n){__t r=n->r;int lh=__lh(r),rh=__rh(r);if(lh>rh){r=__rotate_right(S,r);__height(r->r);       \
__height(r);}n=__rotate_left(S,n);__height(n->l);__height(n);return n;}static inline __t           \
__fix_right(tree_t S,__t n){__t l=n->l;int lh=__lh(l),rh=__rh(l);if(lh<rh){l=__rotate_left(S,l);   \
__height(l->l);__height(l);}n=__rotate_right(S,n);__height(n->r);__height(n);return n;}static __t  \
__pop_and(tree_t S,__t n){__t o=n,p,l,c;n=n->r;while((l = n->l)){n=l;}c=n->r;p=n->p;if(c)c->p=p;   \
__child_replace(S,p,n,c);if(n->p==o)p=n;n->l=o->l;n->r=o->r;n->p=o->p;n->h=o->h;__child_replace(S, \
o->p,o,n);o->l->p=n;if(o->r)o->r->p=n;return p;}static __t __pop_or(tree_t S,__t n){__t c,p;c=n->l;\
if(!c)c=n->r;p=n->p;__child_replace(S,p,n,c);if(c)c->p=p;return p;}static void __pop_rebalance(    \
tree_t S,__t n){int lh,rh,d,h;while(n){lh=__lh(n);rh=__rh(n);h=__max(lh,rh)+1;d=lh-rh;if(n->h!=h)n \
->h=h;else if(d>=-1&&d<=-1)break;if(d<=-2)n=__fix_left(S,n);else if(d>=2)n=__fix_right(S,n);n=n->p;\
}}static void __put_rebalance(tree_t S,__t n){int lh,rh,d,h;for(n=n->p;n;n=n->p){lh=__lh(n);rh=__rh\
(n);h=__max(lh,rh)+1;d=lh-rh;if(n->h==h)break;n->h=h;if(d<=-2)n=__fix_left(S,n);else if(d>=2)n=    \
__fix_right(S,n);}} static tree_t tree_new(){tree_t S=(tree_t)calloc(sizeof(*S),1);S->len=0;S->root\
=NULL;return S;} static void tree_clear(tree_t S){__t n=NULL,h=S->root,t=S->root;while(h){n=h;if(n \
->l){t->p=n->l;t=t->p;t->p=NULL;}if(n->r){t->p=n->r;t=t->p;t->p=NULL;}h=h->p;free(n);}S->len=0;S-> \
root=NULL;}static void tree_cleanup(tree_t S){tree_clear(S);free(S);}static int tree_len(tree_t S){\
return S->len;}static bool tree_is_empty(tree_t S){return S->len==0;}static bool tree_is_exist(    \
tree_t S,K k){__t n=S->root; int t;while(n){t=cmp;if(t==0)break;n=(t<0)?n->l:n->r;}return n;}      \
static V tree_at(tree_t S,K k){__t n=S->root;V v={};int t;while(n){t=cmp;if(t==0)break;n=(t<0)?n->l\
:n->r;}return n?n->v:v;} static void tree_re(tree_t S,K k,V v){__t n=S->root;int t;while(n){t=cmp; \
if(t==0)break;n=(t<0)?n->l:n->r;}if(n)n->v=v;}static V tree_pop(tree_t S,K k){__t n=S->root,p=NULL;\
V v={};int t;while(n){t=cmp;if(t==0)break;n=(t<0)?n->l:n->r;}if(n)return v;if(n->l&&n->r)p=        \
__pop_and(S,n);else p=__pop_or(S,n);if(p)__pop_rebalance(S,p);S->len--;v=n->v;free(n);return v;}   \
static void tree_put(tree_t S,K k,V v){__t* l=&S->root;__t n=NULL,p;int t;while(l[0]){n=l[0];t=cmp;\
if(t==0)return;l=(t<0)?&(n->l):&(n->r);}p=n;n=calloc(sizeof(*n),1);if(!n)return;n->l=NULL;n->r=NULL\
;n->p=p;n->h=1;n->k=k;n->v=v;l[0]=n;S->len++;__put_rebalance(S,n);}

#  else 

#  ifndef U_PRI_ALLOC
#  define U_PRI_ALLOC(size) calloc(size, 1)
#  endif

#  ifndef U_PRI_FREE
#  define U_PRI_FREE(ptr) free(ptr)
#  endif

#  ifndef U_PRI_TREE_USERDATA
#  define U_PRI_TREE_USERDATA
#  endif

typedef struct tnode_t* tnode_t;
struct tnode_t { tnode_t l, r, p; int h; U_PRI_TREE_USERDATA; char u[0]; };
typedef struct { int len; tnode_t root; }* tree_t;

static inline int __lh(tnode_t n) { return n->l ? n->l->h : 0; }
static inline int __rh(tnode_t n) { return n->r ? n->r->h : 0; }
static inline int __max(int a, int b) { return a > b ? a : b; }
static inline void __height(tnode_t n) { n->h = __max(__lh(n), __rh(n)) + 1; }
static inline void __child_replace(tree_t T, tnode_t p, tnode_t o, tnode_t n) {
  if (p == NULL) { T->root = n; } else { if (p->l == o) { p->l = n; } else { p->r = n; } } }
static inline tnode_t __rotate_left(tree_t T, tnode_t n) {
  tnode_t r = n->r, p = n->p; n->r = r->l; if (r->l) { r->l->p = n; }
  r->l = n; r->p = p; __child_replace(T, p, n, r); n->p = r; return r; }
static inline tnode_t __rotate_right(tree_t T, tnode_t n) {
  tnode_t l = n->l, p = n->p; n->l = l->r; if (l->r) { l->r->p = n; }
  l->r = n; l->p = p; __child_replace(T, p, n, l); n->p = l; return l; }
static inline tnode_t __fix_left(tree_t T, tnode_t n) {
  tnode_t r = n->r; int lh = __lh(r), rh = __rh(r);
  if (lh > rh) { r = __rotate_right(T, r); __height(r->r); __height(r); }
  n = __rotate_left(T, n); __height(n->l); __height(n); return n; }
static inline tnode_t __fix_right(tree_t T, tnode_t n) {
  tnode_t l = n->l; int lh = __lh(l), rh = __rh(l);
  if (lh < rh) { l = __rotate_left(T, l); __height(l->l); __height(l); }
  n = __rotate_right(T, n); __height(n->r); __height(n); return n; }
static tnode_t __pop_and(tree_t T, tnode_t n) {
  tnode_t o = n, p, l, c; n = n->r; while ((l = n->l)) { n = l; }
  c = n->r; p = n->p; if (c) { c->p = p; } __child_replace(T, p, n, c);
  if (n->p == o) { p = n; } n->l = o->l; n->r = o->r; n->p = o->p; n->h = o->h;
  __child_replace(T, o->p, o, n); o->l->p = n;
  if (o->r) { o->r->p = n; } return p; }
static tnode_t __pop_or(tree_t T, tnode_t n) {
  tnode_t c, p; c = n->l;
  if (!c) { c = n->r; } p = n->p; __child_replace(T, p, n, c);
  if (c) { c->p = p; } return p; }
static void __pop_rebalance(tree_t T, tnode_t n) {
  int lh, rh, d, h; while (n) {
  lh = __lh(n); rh = __rh(n); h  = __max(lh, rh) + 1; d  = lh - rh;
  if (n->h != h) { n->h = h; } else if (d >= -1 && d <= -1) { break; }
  if (d <= -2) { n = __fix_left(T, n); } else if (d >= 2) { n = __fix_right(T, n); }
  n = n->p; } }
static void __put_rebalance(tree_t T, tnode_t n) {
  int lh, rh, d, h; for (n = n->p; n; n = n->p) {
  lh = __lh(n); rh = __rh(n); h  = __max(lh, rh) + 1; d  = lh - rh;
  if (n->h == h) { break; } n->h = h;
  if (d <= -2) { n = __fix_left(T, n); } else if (d >= 2) { n = __fix_right(T, n); } } }

static inline tree_t tree_new() {
  tree_t T = (tree_t)U_PRI_ALLOC(sizeof(*T)); T->len   = 0; T->root  = NULL; return T; }
static inline void tree_del(tree_t T) { U_PRI_FREE(T); }

static inline tnode_t tree_new_node(size_t s) {
  tnode_t n = U_PRI_ALLOC(sizeof(*n) + s); n->h = 1; n->p = n->l = n->r = NULL; return n; }
static inline void tree_del_node(tnode_t n) { U_PRI_FREE(n); }

static inline int tree_cmp_fn(tnode_t x, tnode_t y);

static inline tnode_t tree_at(tree_t T, tnode_t n) {
  tnode_t l = T->root; int t; while (l) { t = tree_cmp_fn(n, l);
  if (t == 0) { return l; } l = (t < 0) ? l->l : l->r; } return NULL; }
static inline void tree_pop(tree_t T, tnode_t n) {
  tnode_t p = NULL; p = (n->l && n->r ? __pop_and : __pop_or)(T, n);
  if (p) { __pop_rebalance(T, p); } T->len--; }
static inline bool tree_put(tree_t T, tnode_t n) {
  tnode_t *l = &T->root, p = NULL; int t = 0; while (l[0]) { p = l[0];
  t = tree_cmp_fn(n, p); if (t == 0) { return false; } l = (t < 0) ? &p->l : &p->r;
  } n->p = p; l[0] = n; T->len++; __put_rebalance(T, n); return true; }

static inline tnode_t tree_tear(tree_t T, tnode_t* next) {
  tnode_t n = *next, p; if (!n) { if (!T->root) { return NULL; } n = T->root; }
  while (true) { if (n->l) { n = n->l; } else if (n->r) { n = n->r; } else { break; } }
  p = n->p; if (!p) { *next   = NULL; T->root = NULL, T->len = 0; return n; }
  if (p->l == n) { p->l = NULL; } else { p->r = NULL; } n->h  = 0; *next = p; return n; }

static inline tnode_t tree_first(tree_t T) {
  tnode_t n = T->root; if (!n) { return NULL; } while (n->l) { n = n->l; } return n; }
static inline tnode_t tree_last(tree_t T) {
  tnode_t n = T->root; if (!n) { return NULL; } while (n->r) { n = n->r; } return n; }
static inline tnode_t tree_next(tnode_t n) {
  tnode_t last; if (n) { if (n->r) { n = n->r; while (n->l) { n = n->l; } } else { while (true) {
  last = n; n    = n->p; if (!n) { break; } if (n->l == last) { break; } } } } return n; }
static inline tnode_t tree_prev(tnode_t n) {
  tnode_t last; if (n) { if (n->l) { n = n->l; while (n->r) { n = n->r; } } else { while (true) {
  last = n; n    = n->p; if (!n) { break; } if (n->r == last) { break; } } } } return n; }

#  ifdef U_PRI_DEBUG
static inline void tree_node_dump(tnode_t n);
typedef struct { tnode_t n; int i; } tnodebl_t, *tnodebl_ref_t;
static void nbl_put(tnodebl_ref_t nbl, tnodebl_ref_t* top, tnodebl_ref_t* bottom) {
  if (*top - *bottom < 64) { (*(*top)++) = *nbl; } }
static tnodebl_ref_t nbl_pop(tnodebl_ref_t* top, tnodebl_ref_t* bottom) {
  return *top > *bottom ? --*top : NULL; }
static bool nbl_is_empty(tnodebl_ref_t top, tnodebl_ref_t bottom) {
  return top == bottom; }
static bool is_leaf(tnode_t node) {
  return node->l == NULL && node->r == NULL; }
static void nbl_dump(tnode_t node) {
  tnode_t parent = nullptr; parent = node->p; if (parent != NULL) {
    putchar(node == parent->l ? 'L' : 'R'); } tree_node_dump(node); }
static void tree_dump(tree_t self) {
  int level    = 0, sub_index; tnode_t node = self->root;
  tnodebl_t nbl, nbl_stack[64] = {};
  tnodebl_ref_t p_nbl = NULL, top = NULL, bottom = NULL; top = bottom = nbl_stack;
  printf("%p len is %d, height is %d\n", self, self->len, self->root->h);
  while (true) {
  if (node != NULL) { sub_index = p_nbl != NULL ? p_nbl->i : 0; p_nbl = NULL;
  if (is_leaf(node) || sub_index == 1) { nbl.n = NULL; nbl.i = 0;
  } else { nbl.n = node; nbl.i = 1; } nbl_put(&nbl, &top, &bottom); level++;
  /* draw lines as long as sub_idx is the first one */
  if (sub_index == 0) { for (int i = 0; i < level; i++) {
  if (i == level - 1) { printf("%-4s", "+---");
  } else { printf("%-4s", nbl_stack[i - 1].n != NULL ? "|" : " "); } } nbl_dump(node); }
  node = sub_index == 0 ? node->l : node->r; } else { p_nbl = nbl_pop(&top, &bottom);
  if (!p_nbl) { break; }; node = p_nbl->n; level--; } } }
#  endif

#  endif
#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_PRI_H__ */
