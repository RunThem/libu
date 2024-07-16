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

#ifndef U_DEBUG_H__
#  define U_DEBUG_H__

#  ifdef __cplusplus
extern "C" {
#  endif

#  define u_die_if(expr, ...)                                                                      \
    if (expr) {                                                                                    \
      exit(EXIT_FAILURE);                                                                          \
    }

#  define u_check_args_ret(expr, ...)                                                              \
    if (expr) {                                                                                    \
      u_xxx("<%s> ", #expr);                                                                       \
                                                                                                   \
      return __VA_OPT__(__VA_ARGS__);                                                              \
    }

#  define u_check_args_null_ret(ptr, ...)                                                          \
    if ((ptr) == nullptr) {                                                                        \
      u_xxx("<%s == null> ", #ptr);                                                                \
                                                                                                   \
      return __VA_OPT__(__VA_ARGS__);                                                              \
    }

#  define u_check_expr_goto(expr, ...)                                                             \
    if (expr) {                                                                                    \
      u_xxx("<%s> ", #expr);                                                                       \
                                                                                                   \
      goto u_va_0th(end, __VA_ARGS__);                                                             \
    }

#  define u_check_expr_null_goto(ptr, ...)                                                         \
    if ((ptr) == nullptr) {                                                                        \
      u_xxx("<%s == null> ", #ptr);                                                                \
                                                                                                   \
      goto u_va_0th(end, __VA_ARGS__);                                                             \
    }

#  define u_free_if(mem)                                                                           \
    if (mem != nullptr) {                                                                          \
      u_free(mem);                                                                                 \
    }

#  define u_close_if(fd)                                                                           \
    if (fd >= 0) {                                                                                 \
      close(fd);                                                                                   \
    }

#  define u_fclose_if(fp)                                                                          \
    if (fp != nullptr) {                                                                           \
      fclose(fp);                                                                                  \
    }

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_DEBUG_H__ */
