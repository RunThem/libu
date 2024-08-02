#include <sys/time.h>
#include <u/u.h>

#include "tree.h"

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

#define tree_node_def(name, K, V, cmp)                                                             \
  typedef struct tree_node_##name##_t tree_node_##name##_t, *tree_node_##name##_ref_t;             \
  struct tree_node_##name##_t {                                                                    \
    tree_node_##name##_ref_t left;                                                                 \
    tree_node_##name##_ref_t right;                                                                \
    tree_node_##name##_ref_t parent;                                                               \
    size_t height;                                                                                 \
                                                                                                   \
    K key;                                                                                         \
    V val;                                                                                         \
  };

tree_node_def(iv, u_hash_t, tree_val_t, );

/* 全新版本的字符串原始实现 */
typedef char* u_string_t[2]; /* {raw string pointer, string data pointer} */

int main(int argc, const u_cstr_t argv[]) {

#define N 100'0000
  struct timespec s  = {};
  struct timespec e  = {};
  struct timezone tz = {0, 0};
  struct timeval sv  = {};
  struct timeval ev  = {};

  auto t = tree_new();
  u_bench("int.any_t -> tree", N) {
    u_each (i, N) {
      tree_put(t, i, (any_t)(intptr_t)i);
    }
  }

  t = tree_new();
  u_each (i, 24) {
    tree_put(t, i, (any_t)(intptr_t)i);
  }

  tree_dump(t);

#if 1
  {
    /* #[[tree<int, int>]] */
    auto t = u_tree_new(int, int, fn_cmp(int));

    u_bench("tree.put()", N) {
      u_each (i, N) {
        u_tree_put(t, i, i);
      }
    }
  }
#endif

  return EXIT_SUCCESS;
}
