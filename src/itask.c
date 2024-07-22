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

#include <sys/timerfd.h>
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
#define U_TASK_STATE_TWAIT 0x20

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
  ucontext_t ctx; /* 上下文 */
  size_t id;
  int state;
  any_t fun;   /* 协程执行入口 */
  u8_t* stack; /* 栈帧 */
  int fd;      /* 被监听的描述符, 一个协程在同一时间只能监听一个描述符 */

  int tstate; /* 定时器状态 */
  int tfd;    /* 定时器描述符 */

  u_node_t next;
} task_t, *task_ref_t;

typedef struct {
  size_t id;
  size_t cnt;
  ucontext_t ctx;

  /* #[[list<task_t>]] */
  u_list_t(task_t) tasks; /* 就绪队列 */
  u_list_t(task_t) dead;  /* 死亡队列 */

  /* #[[tree<int, task_ref_t>]] */
  u_tree_t(int, task_ref_t) rwait; /* 读队列 */
  u_tree_t(int, task_ref_t) wwait; /* 写队列 */
  u_tree_t(int, task_ref_t) twait; /* 定时器队列 */

  fd_set rfds[2];
  fd_set wfds[2];

  int maxfd;
} scheduler_t, *scheduler_ref_t;

static thread_local scheduler_t g_sch = {};
static thread_local task_ref_t g_task = {};

[[gnu::constructor]]
void task_init() {
  g_sch.id    = 1;
  g_sch.cnt   = 0;
  g_sch.tasks = u_list_new(task_t, next);
  g_sch.dead  = u_list_new(task_t, next);
  g_sch.rwait = u_tree_new(int, task_ref_t, fn_cmp(int));
  g_sch.wwait = u_tree_new(int, task_ref_t, fn_cmp(int));
  g_sch.twait = u_tree_new(int, task_ref_t, fn_cmp(int));

  FD_ZERO(&g_sch.rfds[0]);
  FD_ZERO(&g_sch.wfds[0]);

  FD_ZERO(&g_sch.rfds[1]);
  FD_ZERO(&g_sch.wfds[1]);

  u_xxx("task init");
}

any_t task_new(any_t fun) {
  task_ref_t self = nullptr;

  self = u_talloc(task_t);
  u_end_if(self);

  self->stack = u_zalloc(U_TASK_STACK_SIZE);
  u_end_if(self->stack);

  getcontext(&self->ctx);
  self->ctx.uc_stack.ss_sp   = self->stack;
  self->ctx.uc_stack.ss_size = U_TASK_STACK_SIZE;
  self->ctx.uc_link          = &g_sch.ctx;
  self->id                   = g_sch.id++;
  self->fun                  = fun;
  self->state                = U_TASK_STATE_INIT;
  self->tfd                  = timerfd_create(CLOCK_MONOTONIC, 0);

  u_list_put(g_sch.tasks, self);

  g_sch.cnt++;

  u_xxx("task new %zu, total %zu", self->id, g_sch.cnt);

  return self;

end:
  u_free_if(self);

  return nullptr;
}

inline void task_yield() {
  g_task->state = U_TASK_STATE_READY;
  swapcontext(&g_task->ctx, &g_sch.ctx);
}

static inline void task_switch(int state) {
  g_task->state = state;
  swapcontext(&g_task->ctx, &g_sch.ctx);
}

static void task_select() {
  int cnt                     = 0;
  task_ref_t task             = {};
  struct timeval timeout      = {};
  struct timeval* timeout_ref = &timeout;

  switch (g_task->state) {
    case U_TASK_STATE_RWAIT: {
      u_tree_put(g_sch.rwait, g_task->fd, g_task);
      FD_SET(g_task->fd, &g_sch.rfds[0]);

      u_xxx("task(%zu) -> R %d, total %zu", g_task->id, g_task->fd, u_tree_len(g_sch.rwait));
    } break;

    case U_TASK_STATE_WWAIT: {
      u_tree_put(g_sch.wwait, g_task->fd, g_task);
      FD_SET(g_task->fd, &g_sch.wfds[0]);

      u_xxx("task(%zu) -> W %d, total %zu", g_task->id, g_task->fd, u_tree_len(g_sch.wwait));
    } break;

    case U_TASK_STATE_TWAIT: {
      u_tree_put(g_sch.twait, g_task->fd, g_task);
      FD_SET(g_task->fd, &g_sch.rfds[0]);

      u_xxx("task(%zu) -> T %d, total %zu", g_task->id, g_task->fd, u_tree_len(g_sch.twait));
    } break;
  }

  u_end_if(u_tree_is_empty(g_sch.rwait) && u_tree_is_empty(g_sch.wwait) &&
           u_tree_is_empty(g_sch.twait));

  g_sch.rfds[1] = g_sch.rfds[0];
  g_sch.wfds[1] = g_sch.wfds[0];

  if (u_list_is_empty(g_sch.tasks)) {
    timeout_ref = nullptr;
  }

  g_sch.maxfd = max(u_tree_max(g_sch.rwait).key, u_tree_max(g_sch.wwait).key);
  g_sch.maxfd = max(g_sch.maxfd, u_tree_max(g_sch.wwait).key);

  cnt = select(g_sch.maxfd + 1, &g_sch.rfds[1], &g_sch.wfds[1], nullptr, timeout_ref);
  u_end_if(cnt <= 0);

  u_xxx("select io ret %d, maxfd is %d", cnt, g_sch.maxfd);

  for (int fd = 0; fd < g_sch.maxfd + 1; fd++) {
    if (FD_ISSET(fd, &g_sch.rfds[1])) {
      task = u_tree_pop(g_sch.rwait, fd);
      if (task != nullptr) {
        u_list_put(g_sch.tasks, task);
        FD_CLR(fd, &g_sch.rfds[0]);

        u_xxx("task(%zu) <- R %d", task->id, task->fd);
      }

      task = u_tree_pop(g_sch.twait, fd);
      if (task != nullptr) {
        u_list_put(g_sch.tasks, nullptr, task);
        FD_CLR(fd, &g_sch.rfds[0]);

        u_xxx("task(%zu) <- T %d", task->id, task->fd);
      }
    }

    if (FD_ISSET(fd, &g_sch.wfds[1])) {
      task = u_tree_pop(g_sch.wwait, fd);
      u_die_if(task);

      u_list_put(g_sch.tasks, task);
      FD_CLR(fd, &g_sch.wfds[0]);

      u_xxx("task(%zu) <- W %d", task->id, task->fd);
    }
  }

  return;

end:
}

void task_loop() {
  while (true) {
    u_end_if(g_sch.cnt == 0);

    g_task = u_list_pop(g_sch.tasks);
    u_xxx("task(%zu) resume", g_task->id);

    g_task->state = 0;
    swapcontext(&g_sch.ctx, &g_task->ctx);

    u_xxx("task(%zu) yield, ret %d", g_task->id, g_task->state);
    switch (g_task->state) {
      case U_TASK_STATE_RET:;
      case U_TASK_STATE_DEAD: {
        u_list_put(g_sch.dead, g_task);
        g_sch.cnt--;

        u_xxx("task(%zu) dead", g_task->id);
      } break;

      /* ready, add to ready queue */
      case U_TASK_STATE_READY: {
        u_list_put(g_sch.tasks, g_task);
      } break;

      case U_TASK_STATE_RWAIT:
      case U_TASK_STATE_WWAIT:
      case U_TASK_STATE_TWAIT: {
      } break;

      default: {
        u_xxx("task(%zu) default state is %d", g_task->id, g_task->state);
        assert(0);
      } break;
    }

    task_select();
  }

end:
  u_list_clear(g_sch.dead, it) {
    u_free(it->stack);
    u_free(it);
  }

  u_xxx("end");
}

bool task_timer(i64_t ms, i64_t inter_ms) {
  u64_t buf             = 0;
  struct itimerspec its = {};

  u_chk_if(g_task->tstate == 3, false);

  u_end_if(ms == 0 && inter_ms == 0);

  /* init timer */
  if (g_task->tstate == 0) {
    its.it_value.tv_sec  = ms / 1000;
    its.it_value.tv_nsec = (ms % 1000) * 1000 * 1000;

    g_task->tstate = 1;
  } else if (g_task->tstate == 1) {
    its.it_value.tv_sec  = inter_ms / 1000;
    its.it_value.tv_nsec = (inter_ms % 1000) * 1000 * 1000;
  }

  timerfd_settime(g_task->tfd, 0, &its, nullptr);

  g_task->fd = g_task->tfd;
  task_switch(U_TASK_STATE_TWAIT);

  read(g_task->tfd, &buf, sizeof(u64_t));

  return true;

end:
  g_task->tstate = 3;

  return false;
}

int task_socket(int domain, int type, int protocol) {
  int ret   = 0;
  int fd    = 0;
  int reuse = 1;

  fd = socket(domain, type, protocol);
  u_end_if(fd == -1);

  ret = fcntl(fd, F_SETFL, O_NONBLOCK);
  u_end_if(ret == -1);

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
    g_task->fd = fd;
    task_switch(U_TASK_STATE_RWAIT);

    sockfd = accept(fd, addr, len);
    u_end_if(sockfd < 0 && errno != EAGAIN);
  } while (sockfd < 0);

  ret = fcntl(sockfd, F_SETFL, O_NONBLOCK);
  u_end_if(ret == -1);

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

    u_end_if(ret == -1 && (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINPROGRESS));
  } while (ret != 0);

  return 0;

end:
  return -1;
}

ssize_t task_read(int fd, void* buf, size_t count) {
  ssize_t recn = 0;

  g_task->fd = fd;
  task_switch(U_TASK_STATE_RWAIT);

  recn = read(fd, buf, count);

  return recn;
}

ssize_t task_recv(int fd, void* buf, size_t n, int flags) {
  ssize_t recn = 0;

  g_task->fd = fd;
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

  g_task->fd = fd;
  task_switch(U_TASK_STATE_RWAIT);

  recn = recvfrom(fd, buf, len, flags, src_addr, addrlen);
  u_end_if(recn < 0 && errno == EAGAIN);
  u_end_if(recn < 0 && errno == ECONNRESET, end2);

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
  u_end_if(ret == 0);

  sent += ret;

  while (sent < count) {
    g_task->fd = fd;
    task_switch(U_TASK_STATE_WWAIT);

    ret = write(fd, buf + sent, count - sent);
    u_end_if(ret == 0);

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
  u_end_if(ret == 0);

  sent += ret;

  while (sent < n) {
    g_task->fd = fd;
    task_switch(U_TASK_STATE_WWAIT);

    ret = send(fd, buf + sent, n - sent, flags);
    u_end_if(ret == 0);

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
  u_end_if(ret == 0);

  sent += ret;

  while (sent < len) {
    g_task->fd = fd;
    task_switch(U_TASK_STATE_WWAIT);

    ret = sendto(fd, buf, len, flags, dest_addr, addrlen);
    u_end_if(ret == 0);

    sent += ret;
  }

  return sent;

end:
  return ret;
}
