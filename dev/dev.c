#include <u/core/avl.h>
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

#define _typeof(t) __builtin_classify_type(t)

any_t __bt_state = nullptr;
void boo() {
  // backtrace_print((struct backtrace_state*)__bt_state, 0, stderr);
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  infln("hello libu!");

  u_vec_t(int) v = u_vec_new(int);

  each(i, 30) {
    u_vec_push_back(v, i);
  }

  each(i, 30) {
    u_vec_push_front(v, i);
  }

  u_vec_for(v, i) {
    infln("[%d] = %d", i, *v->item);
  }

  u_vec_cleanup(v);

  return 0;
}
