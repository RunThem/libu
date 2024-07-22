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
  int fd;      /* 被监听的描述符, 一个协程在同一时间只能监听一个描述符 */

  int timer_fd;          /* 定时器描述符 */
  struct itimerspec its; /* 定时器时间戳 */

  u_node_t next;
} task_t, *task_ref_t;

typedef struct {
  size_t id;
  size_t cnt;
  ucontext_t ctx;
  task_ref_t run;
  int state;

  /* #[[list<task_t>]] */
  u_list_t(task_t) tasks; /* 就绪队列 */
  u_list_t(task_t) dead;  /* 死亡队列 */

  /* #[[tree<int, task_ref_t>]] */
  u_tree_t(int, task_ref_t) rwait; /* 读队列 */
  u_tree_t(int, task_ref_t) wwait; /* 写队列 */
  u_tree_t(int, task_ref_t) twait; /* 定时队列 */

  fd_set rfds[2];
  fd_set wfds[2];
  int maxfd;
} scheduler_t, *scheduler_ref_t;

static thread_local scheduler_t sch = {};

[[gnu::constructor]]
void task_init() {
  sch.id    = 1;
  sch.cnt   = 0;
  sch.run   = nullptr;
  sch.tasks = u_list_new(task_t, next);
  sch.dead  = u_list_new(task_t, next);
  sch.rwait = u_tree_new(int, task_ref_t, fn_cmp(int));
  sch.wwait = u_tree_new(int, task_ref_t, fn_cmp(int));
  sch.twait = u_tree_new(int, task_ref_t, fn_cmp(int));

  FD_ZERO(&sch.rfds[0]);
  FD_ZERO(&sch.wfds[0]);

  FD_ZERO(&sch.rfds[1]);
  FD_ZERO(&sch.wfds[1]);

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
  self->ctx.uc_link          = &sch.ctx;
  self->id                   = sch.id++;
  self->fun                  = fun;
  self->state                = U_TASK_STATE_INIT;

  if (self->timer_fd == 0) {
    self->timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
  }

  u_list_put(sch.tasks, self);

  sch.cnt++;

  u_xxx("task new %zu, total %zu", self->id, sch.cnt);

  return self;

end:
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

static inline void task_add_read_event() {
  task_ref_t task = sch.run;

  u_tree_put(sch.rwait, task->fd, task);
  FD_SET(task->fd, &sch.rfds[0]);

  u_xxx("task(%zu) -> R %d", task->id, task->fd);
  u_xxx("rwait total %zu", u_tree_len(sch.rwait));
}

static inline void task_del_read_event(int fd) {
  task_ref_t task = sch.run;

  u_chk_if(u_tree_is_empty(sch.rwait));

  task = u_tree_pop(sch.rwait, fd);
  u_end_if(task);

  task->state = U_TASK_STATE_READY;
  u_list_put(sch.tasks, task);
  FD_CLR(fd, &sch.rfds[0]);

  u_xxx("task(%zu) <- R %d", task->id, task->fd);

  return;

end:
}

static inline void task_add_write_event() {
  task_ref_t task = sch.run;

  u_tree_put(sch.wwait, task->fd, task);
  FD_SET(task->fd, &sch.wfds[1]);

  u_xxx("task(%zu) -> W %d", task->id, task->fd);
  u_xxx("wwait total %zu", u_tree_len(sch.wwait));
}

static inline void task_del_write_event(int fd) {
  task_ref_t task = sch.run;

  u_chk_if(u_tree_is_empty(sch.wwait));

  task = u_tree_pop(sch.wwait, fd);
  u_end_if(task, end);

  task->state = U_TASK_STATE_READY;
  u_list_put(sch.tasks, task);
  FD_CLR(fd, &sch.wfds[0]);

  u_xxx("task(%zu) <- W %d", task->id, task->fd);

  return;

end:
}

static inline void task_add_timer_event() {
  task_ref_t task = sch.run;

  u_chk_if(u_tree_is_exist(sch.twait, task->timer_fd));

  u_tree_put(sch.twait, task->fd, task);
  FD_SET(task->fd, &sch.rfds[0]);

  u_xxx("task(%zu) -> T %d", task->id, task->fd);
  u_xxx("twait total %zu", u_tree_len(sch.twait));
}

static inline void task_del_timer_event(int fd) {
  task_ref_t task = sch.run;

  u_chk_if(u_tree_is_empty(sch.twait));

  task = u_tree_at(sch.twait, fd);
  u_end_if(task);

  if (task->its.it_interval.tv_sec == 0 && task->its.it_interval.tv_nsec == 0) {
    FD_CLR(fd, &sch.rfds[0]);
    u_tree_pop(sch.twait, fd);
  }

  task->state = U_TASK_STATE_READY;
  u_list_put(sch.tasks, task);

  u_xxx("task(%zu) <- T %d", task->id, task->fd);

  return;

end:
}

static void task_select() {
  task_ref_t task             = nullptr;
  int cnt                     = 0;
  struct timeval timeout      = {};
  struct timeval* timeout_ref = &timeout;

  u_chk_if(u_tree_is_empty(sch.rwait) && u_tree_is_empty(sch.wwait) && u_tree_is_empty(sch.twait));

  sch.rfds[1] = sch.rfds[0];
  sch.wfds[1] = sch.wfds[0];

  if (u_list_is_empty(sch.tasks)) {
    timeout_ref = nullptr;
  }

  sch.maxfd = max(u_tree_max(sch.rwait).key, u_tree_max(sch.wwait).key);
  sch.maxfd = max(sch.maxfd, u_tree_max(sch.twait).key);

  cnt = select(sch.maxfd + 1, &sch.rfds[1], &sch.wfds[1], nullptr, timeout_ref);

  u_end_if(cnt <= 0);

  u_xxx("select ret %d, maxfd is %d", cnt, sch.maxfd);

  for (int fd = 0; fd < sch.maxfd + 1; fd++) {
    if (FD_ISSET(fd, &sch.rfds[1])) {
      task_del_read_event(fd);
      task_del_timer_event(fd);
    }

    if (FD_ISSET(fd, &sch.wfds[1])) {
      task_del_write_event(fd);
    }
  }

  return;

end:
}

void task_loop() {
  task_ref_t task = nullptr;

  while (true) {
    u_end_if(sch.cnt == 0);

    task = u_list_pop(sch.tasks);
    u_xxx("task(%zu) resume", task->id);

    task->state = 0;

    sch.run = task;
    swapcontext(&sch.ctx, &task->ctx);

    u_xxx("task(%zu) yield, ret %d", task->id, task->state);
    switch (task->state) {
      case U_TASK_STATE_RET: [[fallthrough]];
      case U_TASK_STATE_DEAD:
        u_list_put(sch.dead, task);
        sch.cnt--;

        u_xxx("task(%zu) dead", task->id);
        break;

      /* ready, add to ready queue */
      case U_TASK_STATE_READY: u_list_put(sch.tasks, task); break;

      case U_TASK_STATE_RWAIT: task_add_read_event(); break;
      case U_TASK_STATE_WWAIT: task_add_write_event(); break;
      case U_TASK_STATE_TIMER: task_add_timer_event(); break;

      default: u_xxx("default state is %d", task->state); assert(0);
    }

    task_select();
  }

end:
  u_list_clear(sch.dead, it) {
    u_free(it->stack);
    u_free(it);
  }

  u_xxx("end");
}

void task_delay(i64_t sec, i64_t nsec) {
  u64_t buf = 0;

  bzero(&sch.run->its, sizeof(struct itimerspec));

  sch.run->its.it_value.tv_sec  = sec;
  sch.run->its.it_value.tv_nsec = nsec;
  timerfd_settime(sch.run->timer_fd, 0, &sch.run->its, nullptr);

  sch.run->fd = sch.run->timer_fd;
  task_switch(U_TASK_STATE_TIMER);

  read(sch.run->timer_fd, &buf, sizeof(u64_t));
}

bool task_timer_start(i64_t sec, i64_t nsec, i64_t inter_sec, i64_t inter_nsec) {
  u64_t buf = 0;

  if (!u_tree_is_exist(sch.twait, sch.run->timer_fd)) {
    bzero(&sch.run->its, sizeof(struct itimerspec));

    sch.run->its.it_value.tv_sec     = sec;
    sch.run->its.it_value.tv_nsec    = nsec;
    sch.run->its.it_interval.tv_sec  = inter_sec;
    sch.run->its.it_interval.tv_nsec = inter_nsec;
    timerfd_settime(sch.run->timer_fd, 0, &sch.run->its, nullptr);
  }

  sch.run->fd = sch.run->timer_fd;
  task_switch(U_TASK_STATE_TIMER);

  read(sch.run->timer_fd, &buf, sizeof(u64_t));

  return true;
}

void task_timer_stop() {
  bzero(&sch.run->its, sizeof(struct itimerspec));

  u_tree_pop(sch.twait, sch.run->timer_fd);
  FD_CLR(sch.run->timer_fd, &sch.rfds[0]);
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
    sch.run->fd = fd;
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
    sch.run->fd = fd;
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
    sch.run->fd = fd;
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
    sch.run->fd = fd;
    task_switch(U_TASK_STATE_WWAIT);

    ret = sendto(fd, buf, len, flags, dest_addr, addrlen);
    u_end_if(ret == 0);

    sent += ret;
  }

  return sent;

end:
  return ret;
}
