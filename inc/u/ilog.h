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

#ifndef U_ILOG_H__
#define U_ILOG_H__

#include <u/utils/keyword.h>
#include <u/utils/va.h>

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern void log_init(const char*);

extern void log_deinit();

extern void log_write(int, const char*, int, const char*, ...);

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#define u_log_init(...)                                                                            \
  do {                                                                                             \
    log_init(va_0th(nullptr, __VA_ARGS__));                                                        \
  } while (0)

#define u_log_deinit()                                                                             \
  do {                                                                                             \
    log_deinit();                                                                                  \
  } while (0)

#define u_err(fmt, ...) log_write(0, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)
#define u_war(fmt, ...) log_write(1, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)
#define u_inf(fmt, ...) log_write(2, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)
#define u_dbg(fmt, ...) log_write(2, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)

#endif /* !U_ILOG_H__ */
