/* libs */
// #include "buf.h"
// #include "fs.h"
// #include "list.h"
// #include "map.h"
// #include "queue.h"
// #include "sock.h"
// #include "stack.h"
// #include "str.h"
#include "u.h"
// #include "vec.h"
// #include "obj.h"

// #include <backtrace-supported.h>
// #include <backtrace.h>
#undef inf
#define inf(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

#define _typeof(t) __builtin_classify_type(t)

any_t __bt_state = nullptr;

void boo() {
  // backtrace_print((struct backtrace_state*)__bt_state, 0, stderr);
}

#include "avl.h"
int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  avl(int) t = nullptr;

  t = avl_new(int, fn_cmp_use(int32));

  avl_push(t, 4);
  avl_push(t, 87);
  avl_push(t, 23);
  avl_push(t, 39);
  avl_push(t, 46);
  avl_push(t, 35);
  avl_push(t, 31);
  avl_push(t, 56);
  avl_push(t, 30);
  avl_push(t, 21);
  avl_push(t, 25);
  avl_push(t, 72);
  avl_push(t, 65);
  avl_push(t, 54);
  avl_push(t, 44);
  avl_push(t, 80);
  avl_push(t, 55);
  avl_push(t, 18);
  avl_push(t, 28);
  avl_push(t, 34);
  avl_push(t, 6);
  avl_push(t, 33);
  avl_push(t, 82);

  avl_debug(t);

  avl_clear(t);

#if 0
  avl_pop(t, 4);
  avl_pop(t, 87);
  avl_pop(t, 23);
  avl_pop(t, 39);
  avl_pop(t, 46);
  avl_pop(t, 35);
  avl_pop(t, 31);
  avl_pop(t, 56);
  avl_pop(t, 30);
  avl_pop(t, 21);
  avl_pop(t, 25);
  avl_pop(t, 72);
  avl_pop(t, 65);
  avl_pop(t, 54);
  avl_pop(t, 44);
  avl_pop(t, 80);
  avl_pop(t, 55);
  avl_pop(t, 18);
  avl_pop(t, 28);
  avl_pop(t, 34);
  avl_pop(t, 6);
  avl_pop(t, 33);
  avl_pop(t, 82);
#endif

  return 0;
}
