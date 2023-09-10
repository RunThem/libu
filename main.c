
#include <threads.h>

/* libs */
#include "avl.h"
#include "buf.h"
#include "fs.h"
#include "map.h"
#include "obj.h"
#include "queue.h"
#include "sock.h"
#include "stack.h"
#include "str.h"
#include "u.h"
#include "vec.h"

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

/* tpool - thread pool */
typedef fnt(tpool_task_fn, void, any_t);
typedef any_t tpool_task_arg_t;

typedef struct {
  tpool_task_fn fn;
  tpool_task_arg_t args;
} tpool_task_t;

typedef struct {
  mtx_t mtx;
  cnd_t cnd;
  thrd_t thrd;
  queue(tpool_task_t) tasks;
} tpool_worker_t;

int tpool_worker_start(any_t args) {

  return 0;
}

typedef struct {
  size_t len;
  size_t cap;
  c_str c_str;

  void (*push_c_str)(const c_str);

}* s2_t;

void str_push_c_str(const c_str c_str);

s2_t str_new() {
  s2_t self = nullptr;

  self = u_talloc(sizeof(*self), s2_t);

  self->c_str = u_talloc(sizeof(char) * 16, c_str);

  self->len = 0;
  self->cap = 16;

  obj_method(self, push_c_str, str_push_c_str);

  return self;
}

void str_push_c_str(const c_str c_str) {
  obj_self(s2_t, self);

  inf_hex(self, sizeof(*self));
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  auto s = str_new();

  inf_hex(s, sizeof(*s));

  s->push_c_str("32");

  return 0;
}
