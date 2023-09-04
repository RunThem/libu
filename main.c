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

struct st_t {
  int a;
  char c;
};

int st_cmp(const void* a, const void* b) {
  struct st_t* _a = as(a, struct st_t*);
  struct st_t* _b = as(b, struct st_t*);

  if (_a->a < _b->a) {
    return -1;
  } else if (_a->a > _b->a) {
    return 1;
  }

  return 0;
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  avl(struct st_t) t = avl_new(struct st_t, st_cmp);

  struct st_t st = {.a = 23, .c = 'e'};

  avl_push(t, st);

  struct st_t s = {.a = 23};

  inf("%c", avl_at(t, s).c);

  return 0;
}
