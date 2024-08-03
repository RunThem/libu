#include <u/u.h>
#include <mimalloc.h>

#define U_PRI_ALLOC(size) mi_calloc(size, 1)
#define U_PRI_FREE(ptr)   mi_free(ptr)

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

pri inline u_hash_t hash(cu8_t* ptr, size_t len) {
  return (u_hash_t) * (int*)(ptr);
}

int main(int argc, const u_cstr_t argv[]) {

#define N 100'0000

#if 1
  /* #[[map<int, int>]] */
  auto m = u_map_new(int, int, nullptr);
  /*
   * N = 100'0000 (release, hash(x) = x)
   *
   * Total time:   0s,  86ms, 823us,  18ns. Average time:    86ns/1000000
   * Total time:   0s,  82ms,  98us, 804ns. Average time:    82ns/1000000
   * Total time:   0s, 102ms, 306us, 412ns. Average time:   102ns/1000000
   * Total time:   0s,  88ms, 380us, 525ns. Average time:    88ns/1000000
   * Total time:   0s,  84ms, 515us, 995ns. Average time:    84ns/1000000
   *
   * N = 100'0000 (release, xxhash.XXH64)
   *
   * Total time:   0s, 223ms, 706us, 433ns. Average time:   223ns/1000000
   * Total time:   0s, 254ms, 678us,  31ns. Average time:   254ns/1000000
   * Total time:   0s, 214ms, 508us, 634ns. Average time:   214ns/1000000
   * Total time:   0s, 216ms, 430us, 124ns. Average time:   216ns/1000000
   * Total time:   0s, 256ms, 143us, 588ns. Average time:   256ns/1000000
   * */

  u_bench("map.put()", N) {
    u_each (i, N) {
      u_map_put(m, i, -i);
    }
  }

#endif

  tree_t l = tree_new();
  /*
   * N = 100'0000 (release)
   *
   * Total time: 0s, 105ms, 437us, 631ns. Average time: 105ns/1000000
   * Total time: 0s, 119ms, 876us, 233ns. Average time: 119ns/1000000
   * Total time: 0s, 113ms, 466us, 177ns. Average time: 113ns/1000000
   * Total time: 0s, 105ms, 613us, 140ns. Average time: 105ns/1000000
   * Total time: 0s, 116ms, 110us, 969ns. Average time: 116ns/1000000
   *
   * N = 100'0000 (release, mimalloc.malloc)
   *
   * Total time:   0s,  45ms, 162us,  67ns. Average time:    45ns/1000000
   * Total time:   0s,  50ms, 465us, 129ns. Average time:    50ns/1000000
   * Total time:   0s,  56ms, 305us, 450ns. Average time:    56ns/1000000
   * Total time:   0s,  49ms, 889us, 106ns. Average time:    49ns/1000000
   * Total time:   0s,  54ms, 303us, 477ns. Average time:    54ns/1000000
   *
   * N = 100'0000 (release, mimalloc.calloc)
   *
   * Total time:   0s,  43ms, 253us, 267ns. Average time:    43ns/1000000
   * Total time:   0s,  50ms,   6us, 334ns. Average time:    50ns/1000000
   * Total time:   0s,  46ms, 921us,  73ns. Average time:    46ns/1000000
   * Total time:   0s,  48ms, 620us, 173ns. Average time:    48ns/1000000
   * Total time:   0s,  63ms, 230us, 877ns. Average time:    63ns/1000000
   * */

  u_bench("tree.put", N) {
    u_each (i, N) {
      tnode_t n     = tree_new_node(sizeof(int));
      *(int*)(n->u) = i;

      tree_put(l, n);
    }
  }

  printf("%d", l->root->h);

  /*
   * N = 100 (release)
   *
   * Total time: 0s, 0ms, 9us, 55ns. Average time: 90ns/100
   * Total time: 0s, 0ms, 6us, 308ns. Average time: 63ns/100
   * Total time: 0s, 0ms, 8us, 649ns. Average time: 86ns/100
   * Total time: 0s, 0ms, 8us, 333ns. Average time: 83ns/100
   * Total time: 0s, 0ms, 9us, 585ns. Average time: 95ns/100
   * */

  // tree_dump(l);

#if 0
  /* #[[map<int, int>]] */
  /*
   * N = 100'0000 (release)
   *
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
