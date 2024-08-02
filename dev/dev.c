#include <sys/time.h>
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

#define N 100'0000

#if 1
  /* #[[map<int, int>]] */
  auto t = u_map_new(int, int);

  u_bench("map.put()", N) {
    u_each (i, N) {
      u_map_put(t, i, i);
    }
  }
#endif

  return EXIT_SUCCESS;
}
