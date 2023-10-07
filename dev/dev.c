#include <u/core/avl.h>
#include <u/core/head.h>
#include <u/core/list.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/core/vec.h>
#include <u/u.h>
#include <u/util/buf.h>
#include <u/util/fs.h>
#include <u/util/obj.h>
#include <u/util/sock.h>

// #include <backtrace-supported.h>
// #include <backtrace.h>
#include <threads.h>

#define _typeof(t) __builtin_classify_type(t)

any_t __bt_state = nullptr;
void boo() {
  // backtrace_print((struct backtrace_state*)__bt_state, 0, stderr);
}

#if 0
typedef any_t u_timer_arg_t;
typedef fnt(u_timer_fn, void, u_timer_arg_t);

typedef struct u_timer_node_t u_timer_node_t;
struct u_timer_node_t {
  uint64_t tick;
  uint64_t period;
  u_timer_fn fun;
  u_timer_arg_t arg;

  u_timer_node_t* parent;
  u_timer_node_t* left;
  u_timer_node_t* right;
};

typedef struct u_timer_t u_timer_t;
struct u_timer_t {
  uint64_t tick;

  u_timer_node_t* root;
};

void fun(u_timer_arg_t arg) {
  printf("%s\n", "hello");
}
#endif

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  infln("hello libu!");

#if 0
  u_timer_node_t tack = {.tick = 3, .period = 1, .fun = fun, .arg = nullptr};
  u_timer_t tm        = {.tick = 0, .root = &tack};

  while (true) {
    if (tm.tick > tm.root->tick) {
      tm.root->fun(tm.root->arg);
    }

    usleep(1000 * 1000); /* 0.01 */
    tm.tick++;
  }
#endif

  u_head_t(int) h = head_new(int, fn_cmp_use(int), U_HEAD_MIN);

  head_push(h, 4);
  head_push(h, 2);
  head_push(h, 7);
  head_push(h, 9);
  head_push(h, 1);
  head_push(h, 5);
  head_push(h, 10);
  head_push(h, 3);
  head_push(h, 2);

  for (size_t i = 0; i < 9; i++) {
    infln("%d", head_pop(h));
  }

  return 0;
}
