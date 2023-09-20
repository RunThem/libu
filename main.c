#include <stdatomic.h>
#include <sys/time.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

/* libs */
#include "core/avl.h"
#include "core/map.h"
#include "core/queue.h"
#include "core/stack.h"
#include "core/str.h"
#include "core/vec.h"
#include "u.h"
#include "util/buf.h"
#include "util/obj.h"

// #include "fs.h"
// #include "sock.h"

// #include <backtrace-supported.h>
// #include <backtrace.h>

// #undef inf
// #define inf(fmt, ...) fprintf(stderr, fmt "\n", ##__VA_ARGS__)

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
  u_queue_t(tpool_task_t) tasks;
} tpool_worker_t;

int tpool_worker_start(any_t args) {

  return 0;
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
  // stbl_init();

  // auto s2 = str_from("hello world!");
  // inf("s2 is %p, %s", s2, s2);
  // inf("%d", errno);

  // auto s3 = str_fromf("<%s>", s2);
  // inf("s3 is %p, %s", s3, s3);

  // str_cleanup(s2);
  // str_cleanup(s3);

  // inf("%d", errno);

  // stbl_cleanup();
  // inf("%d", errno);

  // inf("%d", errno);
  // (void)mi_calloc(1, 1);
  // inf("%d", errno);

  infln("hello");
  infln("%s", "hello");

  errln("hello");
  errln("%s", "hello");

  return 0;
}
