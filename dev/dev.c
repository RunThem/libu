/* local libs */
#include <u/core/avl.h>
#include <u/core/heap.h>
#include <u/core/list.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/set.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/core/vec.h>
#include <u/u.h>
#include <u/util/fs.h>
#include <u/util/obj.h>

/* system libs */
#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h>
#include <regex.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <threads.h>
#include <time.h>
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

typedef struct {
  int a;
  char c;
} st_t;
fn_compe_def(st_t, x.a == y.a, x.a > y.a);

#define to_str(...)   #__VA_ARGS__
#define $(shell, ...) printf("{" to_str(shell) "}\n" va_opt(, ) __VA_ARGS__)

ret_t code = 0;

#undef $
#define $(...)

#define u_vec(T) T*
#define u_push(self, ...)
#define u_pop(self, ...)
#define u_map(self, ...)
#define u_filter(self, ...) (void)self
#define u_clone(self)
#define u_at(self, idx) (*(self))
#define u_size(self)    (sizeof(*(self)))
#define u_erase(self, ...)
#define u_cleanup(self)
#define u_clear(self)
#define u_dump(self)

#define fn(func, ret, ...) ret func(__VA_ARGS__)

fn(hello_num, int, int a) {

  return 1;
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  infln("hello libu!");

  // $(Debug, Clone)
  struct {
    int a;
    char c;

    int (*get)();
  } st;

  u_vec(int) v = {};

  u_push(v, 1, 2, 3, 4, 5);

  u_map(v, [int a, void c] { return a * 2; });

  int a;
  u_filter(v, [int a, &l] bool { return a % l == 0 : true : false; });

  hello_num(u_at(v, 0));
  // hello_num(v.at(0));

#if 0
  u_str_t s = "print";
  /* clang-format off */
  $((cat main.c - c,n,e | grep "%s" | %s), s, s);
  /* clang-format on */

  int $$     = 0;

  char buf[1024] = {0};

  FILE* fd = popen("ls", "r");
  u_die_if(fd == nullptr);

  u_str_t result = u_str_new(100);

  infln("%s", buf);
#endif

  return EXIT_SUCCESS;
}
