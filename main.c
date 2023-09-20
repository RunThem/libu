

#include <stdatomic.h>
#include <sys/time.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

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

#if 0
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

#  define string_append(str, arg...) __string_append(str, arg, nullptr)
str __string_append(str self, ...) {
  return nullptr;
}

#endif

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

atomic_flag flag = ATOMIC_FLAG_INIT;

int func(void*) {

  while (atomic_flag_test_and_set(&flag)) {
    inf("lock");
  };

  inf("unlock");
  atomic_flag_clear(&flag);

  return 0;
}

int test_str(void*) {
  str s   = nullptr;
  str tmp = nullptr;

  for (size_t i = 100'0000; i > 0; i--) {
    if (i % 2 == 0) {
      s = str_from("hello");
    } else {
      s = str_from(s);
    }

    assert(s != tmp);
    assert(!strcmp(s, "hello"));

    tmp = s;
  }

  return 0;
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

#if 0
   stbl_init();

  str msg = "hello world";
  inf("msg is %p", msg);

  inf_bit((uint8_t*)&msg, sizeof(any_t));

  inf("%s", msg);

  // msg = (str)((uintptr_t)msg | 0x8000'0000'0000'0000);
  inf_bit((uint8_t*)&msg, sizeof(any_t));

  inf("%s", msg);
#endif

  // map_debug(stbl);

#if 0
  auto str = string_new("hello world");

  inf("%s", str);

  string_append(str, "fjei", 'c');
#endif

#if 0
  thrd_t th1 = {};

  atomic_flag_test_and_set(&flag);

  thrd_create(&th1, func, nullptr);

  sleep(3);

  atomic_flag_clear(&flag);

  int res;

  thrd_join(th1, &res);
#endif

  stbl_init();

#if 1
  auto s2 = str_from("hello world!");
  inf("s2 is %p, %s", s2, s2);

  auto s3 = str_fromf("<%s>", s2);
  inf("s3 is %p, %s", s3, s3);

  str_cleanup(s2);
  str_cleanup(s3);

#endif

  stbl_cleanup();
#if 0
  thrd_t th1 = {};
  thrd_t th2 = {};
  thrd_t th3 = {};
  thrd_t th4 = {};

  thrd_create(&th1, test_str, nullptr);
  thrd_create(&th2, test_str, nullptr);
  thrd_create(&th3, test_str, nullptr);
  thrd_create(&th4, test_str, nullptr);

  /*
   * 1 thread:
   * ________________________________________________________
   * Executed in  381.55 millis    fish           external
   * usr time  344.19 millis    0.00 millis  344.19 millis
   * sys time   37.66 millis    1.65 millis   36.02 millis
   *
   * 2 thread:
   * ________________________________________________________
   * Executed in    1.19 secs    fish           external
   * usr time    2.36 secs    1.26 millis    2.36 secs
   * sys time    0.00 secs    0.35 millis    0.00 secs
   *
   * 3 thread:
   * ________________________________________________________
   * Executed in    2.02 secs    fish           external
   * usr time    5.91 secs    0.00 millis    5.91 secs
   * sys time    0.01 secs    1.77 millis    0.01 secs
   *
   * 4 thread:
   * ________________________________________________________
   * Executed in    3.09 secs    fish           external
   * usr time   12.09 secs    1.28 millis   12.08 secs
   * sys time    0.03 secs    0.36 millis    0.03 secs
   * */

  int res;

  thrd_join(th1, &res);
  thrd_join(th2, &res);
  thrd_join(th3, &res);
  thrd_join(th4, &res);
#endif

  mi_stats_print(nullptr);

  return 0;
}
