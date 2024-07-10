#include <u/u.h>
#include <sys/queue.h>

#define CO_ARGS_NUM   3 /* rdi, rsi, rdx */
#define CO_INVALID_FD -1

typedef uint64_t reg_t;

typedef struct co_t {
  jmp_buf ctx;

  size_t id;
  int state;
  any_t fun;               /* 协程执行入口 */
  reg_t args[CO_ARGS_NUM]; /* 协程参数 */
  u8_t* stack;             /* 栈帧 */
  u8_t* rawstack;          /* 指向栈帧内存 */
  size_t stksize;          /* 栈帧大小 */
  int fd; /* 监听的描述符, 一个协程在同一时间只能监听一个描述符 */

  u_node_t next;
} co_t;

typedef struct {
  jmp_buf ctx;

  size_t id;
  size_t cnt;
  int state;
  reg_t regs[CO_ARGS_NUM]; /* 参数缓存 */

  co_t* run;

  /* #[[list<co_t>]] */
  u_list_t(co_t) ready;
  u_list_t(co_t) dead;

  /* #[[tree<int, co_t*>]] */
  u_tree_t(int, co_t*) rwait;
  u_tree_t(int, co_t*) wwait;

  fd_set rfds[2];
  fd_set wfds[2];
  int maxfd;
} scheduler_t;

scheduler_t loop = {};

[[gnu::constructor]]
void co_init() {
  loop.id    = 1;
  loop.cnt   = 0;
  loop.run   = nullptr;
  loop.ready = u_list_new(co_t, next);
  loop.dead  = u_list_new(co_t, next);
  loop.rwait = u_tree_new(int, co_t*, fn_cmp(int));
  loop.wwait = u_tree_new(int, co_t*, fn_cmp(int));

  FD_ZERO(&loop.rfds[0]);
  FD_ZERO(&loop.wfds[0]);

  FD_ZERO(&loop.rfds[1]);
  FD_ZERO(&loop.wfds[1]);

  u_dbg("task init");
}

/*
 * @param stack         the stack               (rdi)
 * @param func          the func                (rsi)
 * @param rdi           the func args           (rdx)
 * @param rsi           the func args           (rcx)
 * @param rdx           the func args           (r8)
 * */
int __co_switch(void* stack, void* func, reg_t rdi, reg_t rsi, reg_t rdx);

__asm__(".text                                               \n"
        ".align 8                                            \n"
        ".globl  __co_exit                                   \n"
        ".type   __co_exit %function                         \n"
        ".hidden __co_exit                                   \n"
        "__co_exit:                                          \n"
        "     lea loop(%rip), %rdi                           \n"
        "     mov $0x1, %esi                                 \n"
        "                                                    \n"
        "     call longjmp                                   \n"
        "                                                    \n"
        "                                                    \n"
        ".text                                               \n"
        ".align 8                                            \n"
        ".globl  __co_switch                                 \n"
        ".type   __co_switch %function                       \n"
        ".hidden __co_switch                                 \n"
        "__co_switch:                                        \n"
        "     # 16-align for the stack top address           \n"
        "     movabs $-16, %rax                              \n"
        "     andq %rax, %rdi                                \n"
        "                                                    \n"
        "     # switch to the new stack                      \n"
        "     movq %rdi, %rsp                                \n"
        "                                                    \n"
        "     # save exit function                           \n"
        "     leaq __co_exit(%rip), %rax                     \n"
        "     pushq %rax                                     \n"
        "                                                    \n"
        "     # save entry function args                     \n"
        "     movq %rsi, %rax                                \n"
        "     movq %rdx, %rdi                                \n"
        "     movq %rcx, %rsi                                \n"
        "     movq %r8,  %rdx                                \n"
        "                                                    \n"
        "     # jum entry function                           \n"
        "     jmp *%rax                                      \n");

void co_new(void* fun, ...) {
  co_t* co = nullptr;

  __asm__ volatile("movq %%rsi, %0\n\t"
                   "movq %%rdx, %1\n\t"
                   "movq %%rcx, %2\n\t"
                   : "=m"(loop.regs[0]), "=m"(loop.regs[1]), "=m"(loop.regs[2])
                   :
                   : "rsi", "rdx", "rcx");

  co = u_list_pop(loop.dead);
  if (co == nullptr) {
    co = u_talloc(co_t);
    u_nil_if(co);
  }

  co->stksize  = 8192 * 4;
  co->rawstack = u_zalloc(co->stksize);
  u_nil_if(co->rawstack);

  co->id      = loop.id++;
  co->fun     = fun;
  co->args[0] = loop.regs[0];
  co->args[1] = loop.regs[1];
  co->args[2] = loop.regs[2];

  loop.cnt++;

  u_list_put(loop.ready, co);

  u_dbg("loop.size(%zu)", loop.cnt);

  return;

err:
  u_free_if(co);
}

void co_yield (int state) {
  if (setjmp(loop.run->ctx) == 0) {
    longjmp(loop.ctx, state);
  }
}

void co_loop() {
  co_t* co                    = nullptr;
  int state                   = 0;
  int cnt                     = 0;
  struct timeval timeout      = {};
  struct timeval* timeout_ref = nullptr;

  u_dbg("loop.start");

  while (true) {
    co = u_list_pop(loop.ready);
    if (co == nullptr) {
      goto end;
    }

    u_dbg("co(%zu) resume", co->id);

    loop.run = co;
    state    = setjmp(loop.ctx);
    if (state == 0) {
      if (co->stack == nullptr) {
        co->stack = co->rawstack + co->stksize - 16;
        __co_switch(co->stack, co->fun, co->args[0], co->args[1], co->args[1]);
      } else {
        longjmp(co->ctx, 0);
      }
    }

    u_dbg("co(%zu) yield, ret %d", co->id, state);

    /* flag { 1(dead), 2(ready), 3(rwait), 4(wwait) } */
    switch (state) {
      case 1:
        u_list_put(loop.dead, co);
        loop.cnt--;
        break;

      case 2:
        u_list_put(loop.ready, co);
        break;

        /* read wait */
      case 3:
        u_tree_put(loop.rwait, co->fd, co);
        FD_SET(co->fd, &loop.rfds[0]);
        loop.maxfd = max(loop.maxfd, co->fd);

        u_dbg("co(%zu) -> R %d", co->id, co->fd);
        u_dbg("rwait total %zu", u_tree_len(loop.rwait));
        break;

      /* write wait */
      case 4:
        u_tree_put(loop.wwait, co->fd, co);
        FD_SET(co->fd, &loop.wfds[0]);
        loop.maxfd = max(loop.maxfd, co->fd);

        u_dbg("co(%zu) -> W %d", co->id, co->fd);
        u_dbg("wwait total %zu", u_tree_len(loop.wwait));
        break;

      default: assert(0);
    }

    if (u_tree_is_empty(loop.rwait) && u_tree_is_empty(loop.wwait)) {
      continue;
    }

    /* net fd */
    loop.rfds[1] = loop.rfds[0];
    loop.wfds[1] = loop.wfds[0];

    timeout_ref = u_list_is_empty(loop.ready) ? nullptr : &timeout;
    cnt         = select(loop.maxfd + 1, &loop.rfds[1], &loop.wfds[1], nullptr, timeout_ref);

    u_dbg("select ret %d, maxfd is %d", cnt, loop.maxfd);

    if (cnt > 0) {
      for (int fd = 0; fd <= loop.maxfd; fd++) {
        if (FD_ISSET(fd, &loop.rfds[1])) {
          co = u_tree_pop(loop.rwait, fd);
          u_die_if(co == nullptr, "fd is %d", fd);

          u_list_put(loop.ready, co);
          FD_CLR(fd, &loop.rfds[0]);

          u_dbg("co(%zu) <- R %d", co->id, co->fd);

          goto reset_maxfd;
        }

        if (FD_ISSET(fd, &loop.wfds[1])) {
          co = u_tree_pop(loop.wwait, fd);
          u_die_if(co == nullptr, "fd is %d", fd);

          u_list_put(loop.ready, co);
          FD_CLR(fd, &loop.wfds[0]);

          u_dbg("co(%zu) <- W %d", co->id, co->fd);

          goto reset_maxfd;
        }

        continue;

reset_maxfd:
        if (loop.maxfd == fd) {
          loop.maxfd = 0;
          u_tree_for (loop.rwait, _fd, _task) {
            loop.maxfd = max(loop.maxfd, _fd);
          }

          u_tree_for (loop.wwait, _fd, _task) {
            loop.maxfd = max(loop.maxfd, _fd);
          }
        }
      }
    }
  }

end:
  u_dbg("loop.end");
}
