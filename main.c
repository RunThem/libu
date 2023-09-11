
#include <threads.h>

/* libs */
#include "avl.h"
#include "buf.h"
// #include "fs.h"
#include "map.h"
#include "obj.h"
#include "queue.h"
// #include "sock.h"
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

/*
 * string
 * */

typedef const char* str;

typedef struct {
  size_t len;
  size_t cap;
  str str;
} string_t;

static map(str, string_t) stbl; /* string table */

static fn_eq_def(str, str, x == y);

void stbl_init() {
  string_t null = {0, 0, nullptr}; /* nullptr */

  stbl = map_new(str, string_t, fn_eq_use(str), MAP_INT_HASH_FN);

  map_push(stbl, nullptr, null);
}

str string_new(str c_str) {
  string_t self = {0};
  size_t len    = 0;

  self.str = u_talloc(sizeof(char) * 16, str);
  u_mem_if(self.str);

  self.len = 0;
  self.cap = 16;

  map_push(stbl, self.str, self);

  return self.str;

err:
  return nullptr;
}

#define string_append(str, arg...) __string_append(str, arg, nullptr)
str __string_append(str self, ...) {
  return nullptr;
}

void inf_bit(const uint8_t* buf, size_t size) {
#define bit(byte, n) (((byte) >> (n)) & 1)
  uint8_t byte = 0;

  fprintf(stderr, "%p, %zu: \n", buf, size);
  for (ssize_t i = as(size, ssize_t) - 1; i >= 0; i--) {
    byte = buf[i];

    fprintf(stderr,
            "    %u%u%u%u%u%u%u%u",
            bit(byte, 7),
            bit(byte, 6),
            bit(byte, 7),
            bit(byte, 4),
            bit(byte, 3),
            bit(byte, 2),
            bit(byte, 1),
            bit(byte, 0));

    if (i != 0 && (i + 1) % 2) {
      fprintf(stderr, "\n");
    } else {
      fprintf(stderr, " ");
    }
  }

  fprintf(stderr, "\n");
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  stbl_init();

  str msg = "hello world";
  inf("msg is %p", msg);

  inf_bit((uint8_t*)&msg, sizeof(any_t));

  inf("%s", msg);

  // msg = (str)((uintptr_t)msg | 0x8000'0000'0000'0000);
  inf_bit((uint8_t*)&msg, sizeof(any_t));

  inf("%s", msg);

  // map_debug(stbl);

  auto str = string_new("hello world");

  inf("%s", str);

  string_append(str, "fjei", 'c');

  return 0;
}
