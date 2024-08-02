/* clang-format off */
#pragma once

#ifndef U_PRI_H__
#  define U_PRI_H__

#include <stddef.h>

#  ifdef __cplusplus
extern "C" {
#  endif
#  ifdef U_PRI


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

#define TREE_DEFINE(K, V, cmp)

#  endif
#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_PRI_H__ */
