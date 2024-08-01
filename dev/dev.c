#include <u/u.h>

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
  /* #[[tree<int, int>]] */
  auto t = u_tree_new(int, int, fn_cmp(int));

#define N 100'0000

  u_bench("tree.put()", N) {
    u_each (i, N) {
      u_tree_put(t, i, i);
    }
  }

  return EXIT_SUCCESS;
}
