/* local libs */
#include <u/core/avl.h>
#include <u/core/heap.h>
#include <u/core/list.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/core/vec.h>
#include <u/u.h>
#include <u/util/fs.h>
#include <u/util/obj.h>

/* system libs */
#include <arpa/inet.h>
#include <dirent.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h>
#include <stdalign.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <threads.h>
#include <unistd.h>

/* third libs */
#include <libsock.h>
// #include <backtrace-supported.h>
// #include <backtrace.h>

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
  u64_t tick;
  u64_t period;
  u_timer_fn fun;
  u_timer_arg_t arg;

  u_timer_node_t* parent;
  u_timer_node_t* left;
  u_timer_node_t* right;
};

typedef struct u_timer_t u_timer_t;
struct u_timer_t {
  u64_t tick;

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

  return EXIT_SUCCESS;
}
