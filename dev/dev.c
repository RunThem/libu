#include <libsock.h>
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

int _main(int argc, const u_cstr_t argv[]) {

  return 0;
}

int main(int argc, const u_cstr_t argv[]) {

  /* #[[vec<int>]] */
  u_vec_t(int) v = u_vec_new(int);

  /* 1500 ns switch */
  // u_task_loop(_main, argc, argv);

  return EXIT_SUCCESS;
}
