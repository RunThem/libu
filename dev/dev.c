#include <u/u.h>
#include <u/pri.h>

static inline int tree_cmp_fn(tnode_t x, tnode_t y) {
  auto a = *(int*)(x->u);
  auto b = *(int*)(y->u);
  return a > b ? 1 : (a == b ? 0 : -1);
}

static inline void tree_node_dump(tnode_t n) {
  printf("{ %d }\n", *(int*)(n->u));
}

/*
 * namespace
 *
 * ua: arr
 * uv: vec
 * um: map
 * ut: avl
 * ul: lst
 * us: str
 * ub: buf
 * uf: file
 * ug: log
 * un: net
 * */

/* 全新版本的字符串原始实现 */
typedef char* u_string_t[2]; /* {raw string pointer, string data pointer} */

int main(int argc, const u_cstr_t argv[]) {

#define N 100

  tree_t l = tree_new();

  u_bench("tree.put", N) {
    u_each (i, N) {
      tnode_t n     = tree_new_node(sizeof(int));
      *(int*)(n->u) = i;

      tree_add(l, n);
    }
  }

  tree_dump(l);

#if 1
  /* #[[map<int, int>]] */
  /*
   * N = 100'0000 (release)
   * Total time: 0s, 278ms, 275us, 681ns. Average time: 278ns/1000000
   * Total time: 0s, 283ms, 520us, 869ns. Average time: 283ns/1000000
   * Total time: 0s, 285ms, 333us, 494ns. Average time: 285ns/1000000
   * Total time: 0s, 283ms, 527us, 868ns. Average time: 283ns/1000000
   * Total time: 0s, 296ms, 21us, 854ns. Average time: 296ns/1000000
   * */

  auto t = u_map_new(int, int);

  u_bench("map.put()", N) {
    u_each (i, N) {
      u_map_put(t, i, i);
    }
  }
#endif

  return EXIT_SUCCESS;
}
