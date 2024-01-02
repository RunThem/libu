/* local libs */
#include <math.h>
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
#include <fcntl.h>
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
#include <sys/wait.h>
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

typedef struct st_t {
  int a;
  char c;
} st_t;

fn_compe_def(st_t, x.a == y.a, x.a > y.a);

st_t* u_new_st_t(int a, char c) {
  st_t* self = u_zalloc(sizeof(st_t));
  self->a    = a;
  self->c    = c;

  return self;
}

#define to_str(...)   #__VA_ARGS__
#define $(shell, ...) printf("{" to_str(shell) "}\n" va_opt(, ) __VA_ARGS__)

ret_t code = 0;

#undef $
#define $(...)

#define u_vec(T)    T*
#define u_tbl(K, V) V*
#define u_push(self, ...)
#define u_pop(self, ...)
#define u_clone(self)
#define u_at(self, idx) (*(self))
#define u_size(self)    (sizeof(*(self)))
#define u_erase(self, ...)
#define u_cleanup(self)
#define u_clear(self)
#define u_dump(self)

#define fn(func, ret, ...) ret func(__VA_ARGS__)

#define u_map(self, ...)
#define u_filter(self, ...) (void)self
#define u_sort(self, ...)

#define import(url)

import(std::vector);
import(std::table);

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

#if 1
  infln("hello libu!");

  int a = {};

  // u_sort(arr, [int * a, int * b] {
  //   if (*a == *b) {
  //     return 0;
  //   }
  //   return (*a > *b) ? 1 : -1;
  // });

  $(Debug, Clone)
  struct {
    int a;
    char c;

    int (*get)();
    int (*set)();
  } st;

  u_vec(int) v              = {};
  u_vec(u_vec(int)) l       = {};
  u_vec(u_tbl(int, char)) m = {};

#  undef u_map_t
#  define u_map_t(K, V)                                                                            \
    struct [[gnu::packed]] {                                                                       \
      K key;                                                                                       \
      V val;                                                                                       \
    }*

#  undef u_vec_t
#  define u_vec_t(T)                                                                               \
    struct {                                                                                       \
      T item;                                                                                      \
    }*

  // typedef u_vec_t(u_map_t(int, char)) vmap_t;

#  define _type_of(T) _Generic(typeof(T), vmap_t: "vec<map<int, char>>")
#  define type_of(T)  infln("%s type is %s", #T, _type_of(T))

  u_vec_t(u_map_t(int, char)) i = {0};

  // type_of(i);

#  define u_new(...) u_new_st_t(__VA_ARGS__)

  auto e = u_new(1, 'c');

  u_push(v, 1, 2, 3, 4, 5);

  // u_map(v, [int a, void c] { return a * 2; });

  // int a;
  // u_filter(v, [int a, &l] bool { return a % l == 0; });

  u_str_t s = "print";
  /* clang-format off */
  $((cat main.c - c,n,e | grep "%s" | %s), s, s);
  /* clang-format on */

  int $$ = 0;

  char buf[1024] = {0};

  FILE* fd = popen("ls", "r");
  u_die_if(fd == nullptr);

  u_str_t result = u_str_new(100);

  infln("%s", buf);
#endif

#if 0
  u_str_t file = u_fs_read("test_1.txt");
  u_mem_if(file);

  infln("file size is %zu", u_str_len(&file));

  size_t in_size = u_str_len(&file);
  u8_t* in       = u_zalloc(u_str_len(&file) + 1);
  u_mem_if(in);

  size_t out_size = compressBound(u_str_len(&file));
  u8_t* out       = u_zalloc(out_size);
  u_mem_if(out);

  /* 478 byte */
  auto cmp_status = compress(out, &out_size, as(file, const unsigned char*), u_str_len(&file));
  infln("status is %d, size is %zu", cmp_status, out_size);

  /* 462 */
  // cmp_status = compress2(out, &cmp_size, as(file, const unsigned char*), u_str_len(&file), 10);
  // infln("status is %d, size is %zu", cmp_status, cmp_size);

  cmp_status = uncompress(in, &in_size, out, out_size);
  infln("status is %d, size is %zu", cmp_status, in_size);
#endif

// #undef s
#define u_str_t(_str)                                                                              \
  (((struct {                                                                                      \
     size_t cap;                                                                                   \
     size_t len;                                                                                   \
     char data[sizeof(_str)];                                                                      \
   }){.len = sizeof(_str) - 1, .data = (_str)})                                                    \
       .data)

  u_str_t str = u_str_t("hello");

  str[4] = ' ';

  infln("'%s'", str);

#define key(k, v) k
#define val(k, v) v
#define l(a, ...) key a, val a

  // l((1, 2));

  return EXIT_SUCCESS;
err:
  return EXIT_FAILURE;
}

/*
 * ncat -l -p 8080 -e /bin/cat
 * */
int sopen(const char* program) {
  int fds[2];
  pid_t pid;

  if (socketpair(AF_UNIX, SOCK_STREAM, 0, fds) < 0)
    return -1;

  switch (pid = vfork()) {
    case -1: /* Error */
      close(fds[0]);
      close(fds[1]);
      return -1;
    case 0: /* child */
      close(fds[0]);
      dup2(fds[1], 0);
      dup2(fds[1], 1);
      close(fds[1]);
      execl("/usr/bin/ncat", "ncat", "10.10.10.252", "8080", nullptr);
      _exit(0);
  }

  /* parent */
  close(fds[1]);

  return fds[0];
}
