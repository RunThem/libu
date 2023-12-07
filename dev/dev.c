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

fn_compe_def(st_t, (x.a == y.a), (x.a > y.a));

#define to_str(...)   #__VA_ARGS__
#define $(shell, ...) printf("{" to_str(shell) "}\n" va_opt(, ) __VA_ARGS__)

int lsexec2(int* in, int* out, char* path, char* arg[]) {
  int fd_i[2];
  int fd_o[2];
  char buf[BUFSIZ];
  int len = 0;

  // 创建两个管道，fd_i用于接管子进程的标准输入，fd_o用于标准输出
  pipe(fd_i);
  pipe(fd_o);

  pid_t p = fork();
  if (p < 0)
    return -1;
  if (p > 0) {       // parent
    close(fd_i[0]);  // 关闭输入管道的读端
    *in = fd_i[1];   // 将写端暴露给调用者，这样调用者可以给子进程发送命令
    close(fd_o[1]);  // 关闭输出管道的写端
    *out = fd_o[0];  // 将读端暴露给调用者，这样调用者可以读取子进程的输出
    return 0;
  } else {           // child
    close(fd_i[1]);  // 关闭输入管道的写端
    dup2(fd_i[0],
         STDIN_FILENO);  // 将标准输入复制到输入管道的读端，这样就可以把父进程的输入转接到标准输入
    close(fd_o[0]);      // 关闭输出管道的读端
    dup2(fd_o[1], STDOUT_FILENO);

    // 将标准输出复制到输出管道的写端，这样父进程就可以通过读端读取标准输出的内容执行需要的命令
    return execv(path, arg);
  }
}

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  infln("hello libu!");

  // u_str_t s = "print";
  /* clang-format off */
  // $((cat main.c - c,n,e | grep "%s" | %s), s, s);
  /* clang-format on */

  // int $$ = 0;

  // int in;
  // int out;

  // char* args[] = {"ncat", "-u", "localhost", "8080", NULL};
  // char* args[] = {"lua", NULL};
  // char* args[] = {"python", NULL};

  // execv("/usr/bin/lua", args);

#if 1
  // int ret = lsexec2(&in, &out, "/usr/bin/ncat", args);
  // int ret = lsexec2(&in, &out, "/usr/bin/lua", args);
  // int ret = lsexec2(&in, &out, "/home/coder/.local/bin/python", args);
  // u_die_if(ret == -1);

  // infln("ret is %d", ret);

  // size_t len     = 0;
  // char buf[1024] = {};

  // sleep(1);

  // len = read(out, buf, arr_len(buf));
  // infln("len is %zu, %s", len, buf);

  // while (true) {
  // len = read(out, buf, arr_len(buf));
  // infln("len is %zu, buf is %s", len, buf);

  // scanf("%s", buf);
  // strcat(buf, "print(\"heljas\")\r\n");
  // write(in, buf, strlen(buf));

  // usleep(10000);

  // bzero(buf, arr_len(buf));

  // len = read(out, buf, arr_len(buf));
  // infln("len is %zu, buf is %s", len, buf);
  // }
#endif

  return EXIT_SUCCESS;
}
