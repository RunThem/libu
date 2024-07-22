#include <libsock.h>
#include <u/u.h>
#include <sys/timerfd.h>

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

void fun(int ms) {
  while (task_timer(1000, ms)) {
    u_dbg("timeout %d", ms);
  }
}

int _main(int argc, const u_cstr_t argv[]) {

  fun(1000);

  return 0;
}

int main(int argc, const u_cstr_t argv[]) {
  u_task_loop(_main, argc, argv);

  return EXIT_SUCCESS;
}
