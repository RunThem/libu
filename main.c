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
// #include "avl.h"
// #include "stack.h"

// #include <backtrace-supported.h>
// #include <backtrace.h>
#undef inf
#define inf(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

#define _typeof(t) __builtin_classify_type(t)

any_t __bt_state = nullptr;

void boo() {
  // backtrace_print((struct backtrace_state*)__bt_state, 0, stderr);
}

#define __same_type(a, b)    __builtin_types_compatible_p(typeof(a), typeof(b))
#define BUILD_BUG_ON_ZERO(e) (sizeof(struct { int : -!!(e); }))
#define __must_be_array(a)   BUILD_BUG_ON_ZERO(__same_type((a), &(a)[0]))

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  int a[123] = {0};

  printf("%zu", ARRAY_SIZE(a));

  return 0;
}
