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

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  return 0;
}
