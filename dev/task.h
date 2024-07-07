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

#pragma once
#ifndef U_TASK_H__
#  define U_TASK_H__

#  include <assert.h>
#  include <fcntl.h>
#  include <sys/socket.h>
#  include <ucontext.h>
#  include <unistd.h>

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 ***************************************************************************************************/
typedef struct {
  size_t id;
  int state;
  any_t fun;      /* 协程执行入口 */
  u8_t* stack;    /* 栈帧 */
  size_t stksize; /* 栈帧大小 */
  ucontext_t ctx; /* 上下文 */
  int fd;         /* 监听的描述符, 一个协程在同一时间只能监听一个描述符 */
} task_t;

typedef struct {
  size_t id;
  size_t cnt;
  ucontext_t ctx;
  task_t* run;
  int state;
  u_list_t(task_t) tasks;
  u_list_t(task_t) dead;
  u_tree_t(int, task_t*) rwait;
  u_tree_t(int, task_t*) wwait;

  fd_set rfds[2];
  fd_set wfds[2];
  int maxfd;
} scheduler_t;

/***************************************************************************************************
 * Let
 ***************************************************************************************************/
extern scheduler_t sch;

/***************************************************************************************************
 * Api
 ***************************************************************************************************/
extern any_t task_new(any_t);

extern void task_loop();

extern int task_socket(int, int, int);

extern int task_accept(int, struct sockaddr*, socklen_t*);

extern int task_connect(int, struct sockaddr*, socklen_t);

extern ssize_t task_read(int, void*, size_t);

extern ssize_t task_recv(int, void*, size_t, int);

extern ssize_t task_recvfrom(int, void*, size_t, int, struct sockaddr*, socklen_t*);

extern ssize_t task_write(int, const void*, size_t);

extern ssize_t task_send(int, const void*, size_t, int);

extern ssize_t task_sendto(int, const void*, size_t, int, const struct sockaddr*, socklen_t);

/***************************************************************************************************
 * iApi
 ***************************************************************************************************/
#  define u_task_new(fun, ...)                                                                     \
    do {                                                                                           \
      task_t* _t = task_new(fun);                                                                  \
                                                                                                   \
      makecontext(&_t->ctx, any(fun), va_size(__VA_ARGS__) va_list(0, __VA_ARGS__));               \
    } while (0)

#  define u_task_loop(start, ...)                                                                  \
    do {                                                                                           \
      u_task_new(start __VA_OPT__(, ) __VA_ARGS__);                                                \
                                                                                                   \
      task_loop();                                                                                 \
    } while (0)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_TASK_H__ */
