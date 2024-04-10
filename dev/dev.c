#define uvec_def (bool, int)
#define umap_def ((int, bool), (int, char))
#include <u/u.h>

/* system libs */

#include <stdlib.h>

#if 1
#  include <arpa/inet.h>
#  include <dirent.h>
#  include <fcntl.h>
#  include <net/ethernet.h>
#  include <net/if.h>
#  include <netinet/if_ether.h>
#  include <netinet/in.h>
#  include <netinet/ip.h>
#  include <netinet/ip_icmp.h>
#  include <netpacket/packet.h>
#  include <regex.h>
#  include <sys/ioctl.h>
#  include <sys/resource.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <sys/wait.h>
#  include <threads.h>
#  include <ucontext.h>
#  include <unistd.h>
#endif

ret_t code = 0;

/*
 * namespace
 *
 * ua:
 * uv: vec
 * um: map
 * ut: avl
 * ul: lst
 * us: str
 * ub: buf
 * uf: file
 * ug: log
 * un: net
 * */

#define mtx_block(mtx) for (bool _ = true; _ && !mtx_lock(mtx); _ = false, mtx_unlock(mtx))

#define STACK_SIZE 1024

ucontext_t uctx_main        = {};
ucontext_t uctx_fun1        = {};
ucontext_t uctx_fun2        = {};
char fun1_stack[STACK_SIZE] = {};
char fun2_stack[STACK_SIZE] = {};

void fun1() {
  println("fun 1 running");
  swapcontext(&uctx_fun1, &uctx_fun2);
  println("fun 1 ending");
}

void fun2() {
  println("fun 2 running");
  swapcontext(&uctx_fun2, &uctx_fun1);
  println("fun 2 ending");
}

int main(int argc, const u_cstr_t argv[]) {
  char str[] = "hello";
  int result = typeeq(char[], str);
  infln("result is %d", result);

  infln("%lu", sizeof(enum {T, F}));

  getcontext(&uctx_fun1);
  uctx_fun1.uc_link          = &uctx_main;
  uctx_fun1.uc_stack.ss_sp   = fun1_stack;
  uctx_fun1.uc_stack.ss_size = STACK_SIZE;
  makecontext(&uctx_fun1, fun1, 0);

  getcontext(&uctx_fun2);
  uctx_fun2.uc_link          = &uctx_main;
  uctx_fun2.uc_stack.ss_sp   = fun2_stack;
  uctx_fun2.uc_stack.ss_size = STACK_SIZE;
  makecontext(&uctx_fun2, fun2, 0);

  swapcontext(&uctx_main, &uctx_fun1);

  println("Main program ending");

  return EXIT_SUCCESS;
err:
  errln("failed.");

  return EXIT_FAILURE;
}
