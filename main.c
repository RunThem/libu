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
#include "vec.h"

// #include <backtrace-supported.h>
// #include <backtrace.h>

#define _typeof(t) __builtin_classify_type(t)

any_t __bt_state = nullptr;

void boo() {
  // backtrace_print((struct backtrace_state*)__bt_state, 0, stderr);
}

void foo() {
  boo();
}

/*
 *
 *     +--------+--------+--------+--------+--------+-----------------+
 *     | itsize |
 len   |  cap   | items  |  item  |      ...        |
 *     +--------+--------+--------+--------+--------+-----------------+
 *
 *
 */

/*
 * +--------+--------+--------+--------+--------+--------------+
 * | itsize |  len   |  cap   | items  |  item  |      ...     |
 * +--------+--------+--------+--------+--------+--------------+
 *
 * */

/*
 * 0        8        16       32       40       48
 * +--------+--------+--------+--------+--------+--------------+
 * | itsize |  len   |  cap   | items  |  item  |     ....     |
 * +--------+--------+--------+--------+--------+--------------+
 *                                     ^   T*
 *                                     |
 *                                     |
 *                                     +
 *
 * */

#define vec3(T)                                                                                    \
  struct {                                                                                         \
    T* item;                                                                                       \
  }*

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  foo();

  vec(int) vv = vec_new(int, 16);

  for (size_t i = 0; i < 100; i++) {
    vec_push_front(vv, i);
  }

  vec_for(vv, i) {
    *vv->item = *vv->item * 2;
  }

  vec_for(vv, i) {
    inf("%zu, %d", i, *vv->item);
  }

  inf("len is %zu", vec_len(vv));

  return 0;
}
