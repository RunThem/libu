/* MIT License
 *
 * Copyright (c) 2023 RunThem <iccy.fun@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * */

#include <u/u.h>

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#define U_TASK_STACK_SIZE (0x20'00'00)

#define U_TASK_STATE_RET   0x00
#define U_TASK_STATE_INIT  0x01
#define U_TASK_STATE_DEAD  0x02
#define U_TASK_STATE_READY 0x04
#define U_TASK_STATE_RWAIT 0x08
#define U_TASK_STATE_WWAIT 0x10
#define U_TASK_STATE_TIMER 0x20

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
  ucontext_t ctx; /* 上下文 */
  size_t id;
  int state;
  any_t fun;   /* 协程执行入口 */
  u8_t* stack; /* 栈帧 */
  int fd;      /* 监听的描述符, 一个协程在同一时间只能监听一个描述符 */
  u64_t timer; /* 定时器(单位 1ms) */

  u_node_t next;
} task_t, *task_ref_t;

fn_compe_def(task_t, x.timer == y.timer, x.timer > y.timer);

typedef struct {
  size_t id;
  size_t cnt;
  ucontext_t ctx;
  task_ref_t run;
  int state;
  u_list_t(task_t) tasks; /* #[[list<task_t>]] */
  u_list_t(task_t) dead;
  u_tree_t(int, task_ref_t) rwait; /* #[[tree<int, task_ref_t>]] */
  u_tree_t(int, task_ref_t) wwait;

  u_heap_t(task_ref_t) timer; /* #[[heap<task_ref_t>]] */

  fd_set rfds[2];
  fd_set wfds[2];
  int maxfd;
} scheduler_t, *scheduler_ref_t;

scheduler_t sch = {};

[[gnu::constructor]]
void task_init() {
  sch.id    = 1;
  sch.cnt   = 0;
  sch.run   = nullptr;
  sch.tasks = u_list_new(task_t, next);
  sch.dead  = u_list_new(task_t, next);
  sch.rwait = u_tree_new(int, task_ref_t, fn_cmp(int));
  sch.wwait = u_tree_new(int, task_ref_t, fn_cmp(int));
  sch.timer = u_heap_new(task_ref_t, true, fn_cmp(task_t));

  FD_ZERO(&sch.rfds[0]);
  FD_ZERO(&sch.wfds[0]);

  FD_ZERO(&sch.rfds[1]);
  FD_ZERO(&sch.wfds[1]);

  u_xxx("task init");
}

any_t task_new(any_t fun) {
  task_ref_t self = nullptr;

  self = u_talloc(task_t);
  u_check_expr_null_goto(self);

  self->stack = u_zalloc(U_TASK_STACK_SIZE);
  u_check_expr_null_goto(self->stack);

  getcontext(&self->ctx);
  self->ctx.uc_stack.ss_sp   = self->stack;
  self->ctx.uc_stack.ss_size = U_TASK_STACK_SIZE;
  self->ctx.uc_link          = &sch.ctx;
  self->id                   = sch.id++;
  self->fun                  = fun;
  self->state                = U_TASK_STATE_INIT;

  u_list_put(sch.tasks, self);

  sch.cnt++;

  u_xxx("task new %zu, total %zu", self->id, sch.cnt);

  return self;

end:
  assert(0);

  u_free_if(self);

  return nullptr;
}

inline void task_yield() {
  sch.run->state = U_TASK_STATE_READY;
  swapcontext(&sch.run->ctx, &sch.ctx);
}

static inline void task_switch(int state) {
  sch.run->state = state;
  swapcontext(&sch.run->ctx, &sch.ctx);
}

static inline u64_t task_gettick(u64_t tick) {
  struct timespec tp = {};

  clock_gettime(CLOCK_MONOTONIC, &tp);

  return tp.tv_sec * 1000 + tp.tv_nsec / 1000 / 1000 + tick;
}

static void task_network() {
  task_ref_t task             = nullptr;
  int cnt                     = 0;
  struct timeval timeout      = {};
  struct timeval* timeout_ref = &timeout;

  u_check_args_ret(u_tree_is_empty(sch.rwait) && u_tree_is_empty(sch.wwait));

  sch.rfds[1] = sch.rfds[0];
  sch.wfds[1] = sch.wfds[0];

  if (u_list_is_empty(sch.tasks) && u_heap_is_empty(sch.timer)) {
    timeout_ref = nullptr;
  }

  sch.maxfd = max(u_tree_max(sch.rwait).key, u_tree_max(sch.wwait).key);
  cnt       = select(sch.maxfd + 1, &sch.rfds[1], &sch.wfds[1], nullptr, timeout_ref);

  u_check_expr_goto(cnt <= 0);

  u_xxx("select ret %d, maxfd is %d", cnt, sch.maxfd);

  for (int fd = 0; fd < sch.maxfd; fd++) {
    if (FD_ISSET(fd, &sch.rfds[1])) {
      task = u_tree_pop(sch.rwait, fd);
      u_die_if(task == nullptr, "fd is %d", fd);

      task->state = U_TASK_STATE_READY;
      u_list_put(sch.tasks, task);
      FD_CLR(fd, &sch.rfds[0]);

      u_xxx("task(%zu) <- R %d", task->id, task->fd);
    }

    if (FD_ISSET(fd, &sch.wfds[1])) {
      task = u_tree_pop(sch.wwait, fd);
      u_die_if(task == nullptr, "fd is %d", fd);

      task->state = U_TASK_STATE_READY;
      u_list_put(sch.tasks, task);
      FD_CLR(fd, &sch.wfds[0]);

      u_xxx("task(%zu) <- W %d", task->id, task->fd);
    }
  }

  return;

end:
}

static void task_timer() {
  task_ref_t task             = nullptr;
  int cnt                     = 0;
  u64_t tick                  = 0;
  struct timeval timeout      = {.tv_usec = 10};
  struct timeval* timeout_ref = nullptr;

  u_check_args_ret(u_heap_is_empty(sch.timer));

  tick = task_gettick(0);
  task = u_heap_at(sch.timer);

  if (task->timer > tick) {
    if (u_list_is_empty(sch.tasks) && u_tree_is_empty(sch.rwait) && u_tree_is_empty(sch.wwait)) {
      usleep((task->timer - tick) * 1000);
    }
  }

  while (!u_heap_is_empty(sch.timer)) {
    task = u_heap_at(sch.timer);

    if (task->timer > tick) {
      break;
    }

    u_xxx("task(%zu) timer timeout, cur(%zu), %zu, %d",
          task->id,
          tick,
          task->timer,
          task->timer > tick);

    u_heap_pop(sch.timer);
    u_list_put(sch.tasks, task);
  }
}

void task_loop() {
  task_ref_t task             = nullptr;
  int cnt                     = 0;
  u64_t tick                  = 0;
  struct timeval timeout      = {.tv_usec = 10};
  struct timeval* timeout_ref = nullptr;

  while (true) {
    task = u_list_pop(sch.tasks);
    u_xxx("task(%zu) resume", task->id);

    task->state = 0;

    sch.run = task;
    swapcontext(&sch.ctx, &task->ctx);
    sch.run = nullptr;

    u_xxx("task(%zu) yield, ret %d", task->id, task->state);
    switch (task->state) {
      case U_TASK_STATE_RET: [[fallthrough]];

      /* dead, add to dead queue */
      case U_TASK_STATE_DEAD:
        u_list_put(sch.dead, task);
        sch.cnt--;

        u_xxx("task(%zu) dead", task->id);
        break;

      /* ready, add to ready queue */
      case U_TASK_STATE_READY: u_list_put(sch.tasks, task); break;

      /* read wait */
      case U_TASK_STATE_RWAIT:
        u_tree_put(sch.rwait, task->fd, task);

        FD_SET(task->fd, &sch.rfds[0]);

        u_xxx("task(%zu) -> R %d", task->id, task->fd);
        u_xxx("rwait total %zu", u_tree_len(sch.rwait));
        break;

      /* write wait */
      case U_TASK_STATE_WWAIT:
        u_tree_put(sch.wwait, task->fd, task);

        FD_SET(task->fd, &sch.wfds[0]);

        u_xxx("task(%zu) -> W %d", task->id, task->fd);
        u_xxx("wwait total %zu", u_tree_len(sch.wwait));
        break;

      /* timer */
      case U_TASK_STATE_TIMER:
        u_heap_put(sch.timer, task);

        u_xxx("task(%zu) delay %zu tick", task->id, task->timer);
        break;

      default: u_xxx("default state is %d", task->state); assert(0);
    }

    u_check_args_ret(sch.cnt == 0);

    do {
      u_xxx("--- network ---");
      task_network();
#if 0
      u_xxx("--- timer ---");
      task_timer();
#endif
    } while (u_list_is_empty(sch.tasks));
  }

end:
  u_list_clear(sch.dead, it) {
    u_free(it->stack);
    u_free(it);
  }

  u_xxx("end");
}

bool task_delay(u64_t tick) {
  sch.run->timer = task_gettick(tick);

  task_switch(U_TASK_STATE_TIMER);

  return true;
}

/*
 * rewrite syscall
 * */

int task_socket(int domain, int type, int protocol) {
  int ret   = 0;
  int fd    = 0;
  int reuse = 1;

  fd = socket(domain, type, protocol);
  u_check_expr_goto(fd == -1);

  ret = fcntl(fd, F_SETFL, O_NONBLOCK);
  u_check_expr_goto(ret == -1);

  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, any(&reuse), sizeof(int));

  return fd;

end:
  u_close_if(fd);

  return -1;
}

int task_accept(int fd, struct sockaddr* addr, socklen_t* len) {
  int ret    = 0;
  int sockfd = -1;
  int reuse  = 1;

  do {
    sch.run->fd = fd;
    task_switch(U_TASK_STATE_RWAIT);

    sockfd = accept(fd, addr, len);
    u_check_expr_goto(sockfd < 0 && errno != EAGAIN);
  } while (sockfd < 0);

  ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
  u_check_expr_goto(ret == -1);

  setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, any(&reuse), sizeof(int));

  return sockfd;

end:

  if (errno == ECONNABORTED) {
    u_xxx("accept: ECONNABORTED");
  } else if (errno == EMFILE || errno == ENFILE) {
    u_xxx("accept: EMFILE || ENFILE");
  }

  return -1;
}

int task_connect(int fd, struct sockaddr* name, socklen_t namelen) {
  int ret = -1;

  do {
    ret = connect(fd, name, namelen);

    u_check_expr_goto(ret == -1 &&
                      (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINPROGRESS));
  } while (ret != 0);

  return 0;

end:
  return -1;
}

ssize_t task_read(int fd, void* buf, size_t count) {
  ssize_t recn = 0;

  sch.run->fd = fd;
  task_switch(U_TASK_STATE_RWAIT);

  recn = read(fd, buf, count);

  return recn;
}

ssize_t task_recv(int fd, void* buf, size_t n, int flags) {
  ssize_t recn = 0;

  sch.run->fd = fd;
  task_switch(U_TASK_STATE_RWAIT);

  recn = recv(fd, buf, n, flags);

  return recn;
}

ssize_t task_recvfrom(int fd,
                      void* buf,
                      size_t len,
                      int flags,
                      struct sockaddr* src_addr,
                      socklen_t* addrlen) {
  ssize_t recn = 0;

  sch.run->fd = fd;
  task_switch(U_TASK_STATE_RWAIT);

  recn = recvfrom(fd, buf, len, flags, src_addr, addrlen);
  u_check_expr_goto(recn < 0 && errno == EAGAIN);
  u_check_expr_goto(recn < 0 && errno == ECONNRESET, end2);

  return recn;

end:
  return recn;

end2:
  return 0;
}

ssize_t task_write(int fd, const void* buf, size_t count) {
  ssize_t sent = 0;
  ssize_t ret  = 0;

  ret = write(fd, buf, count);
  u_check_expr_goto(ret == 0);

  sent += ret;

  while (sent < count) {
    sch.run->fd = fd;
    task_switch(U_TASK_STATE_WWAIT);

    ret = write(fd, buf + sent, count - sent);
    u_check_expr_goto(ret == 0);

    sent += ret;
  }

  return sent;

end:
  return ret;
}

ssize_t task_send(int fd, const void* buf, size_t n, int flags) {
  ssize_t sent = 0;
  ssize_t ret  = 0;

  ret = send(fd, buf, n, flags);
  u_check_expr_goto(ret == 0);

  sent += ret;

  while (sent < n) {
    sch.run->fd = fd;
    task_switch(U_TASK_STATE_WWAIT);

    ret = send(fd, buf + sent, n - sent, flags);
    u_check_expr_goto(ret == 0);

    sent += ret;
  }

  return sent;

end:
  return ret;
}

ssize_t task_sendto(int fd,
                    const void* buf,
                    size_t len,
                    int flags,
                    const struct sockaddr* dest_addr,
                    socklen_t addrlen) {
  ssize_t sent = 0;
  ssize_t ret  = 0;

  ret = sendto(fd, buf, len, flags, dest_addr, addrlen);
  u_check_expr_goto(ret == 0);

  sent += ret;

  while (sent < len) {
    sch.run->fd = fd;
    task_switch(U_TASK_STATE_WWAIT);

    ret = sendto(fd, buf, len, flags, dest_addr, addrlen);
    u_check_expr_goto(ret == 0);

    sent += ret;
  }

  return sent;

end:
  return ret;
}
