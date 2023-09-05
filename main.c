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
// #include "obj.h"
#include "avl.h"
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

typedef struct st_t {
  int a;
  char c;
} st_t;

fn_cmp_dec(st_t) {
  st_t* x = as(_x, st_t*);
  st_t* y = as(_y, st_t*);

  if (x->a < y->a) {
    return -1;
  } else if (x->a > y->a) {
    return 1;
  }

  return 0;
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  avl(st_t) t = avl_new(st_t, fn_cmp_use(st_t));

  st_t st = {.a = 23, .c = 'e'};

  avl_push(t, st);

  st_t s = {.a = 23};

  inf("%zu", avl_len(t));

  inf("%c", avl_at(t, s).c);

  return 0;
}
