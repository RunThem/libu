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

#include "print.h"
#include "type.h"

#include <errno.h>
#include <setjmp.h>
#include <stddef.h>

#define U_ERR_MSG_SIZE 2048

typedef struct {
  bool is_err;
  jmp_buf label;
  str_t file;
  str_t func;
  str_t expr;
  size_t line;
  error_t error;
  int id;
  char msg[U_ERR_MSG_SIZE];
} __err__t;

extern thread_local __err__t __err__;

/* clang-format off */
#define try      for (bzero(&__err__, sizeof(__err__)); !setjmp(__err__.label);)
#define catch(e) for (auto e = __err__; e.is_err; e.is_err = false)
#define panic(_expr, _id, args...)                                                                 \
  do {                                                                                             \
    __err__.is_err = true;                                                                         \
    __err__.file   = __file__;                                                                     \
    __err__.func   = __func__;                                                                     \
    __err__.line   = __line__;                                                                     \
    __err__.expr   = #_expr;                                                                       \
    __err__.id     = _id;                                                                          \
    __err__.error  = errno;                                                                        \
                                                                                                   \
    va_if(va_has(args)) (                                                                          \
      snprintf(__err__.msg, U_ERR_MSG_SIZE, args);                                                 \
    )                                                                                              \
                                                                                                   \
    longjmp(__err__.label, 1);                                                                     \
  } while (0)
