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

#ifndef U_PRINT_H__
#define U_PRINT_H__

#include "keyword.h"
#include "type.h"

#include <errno.h>
#include <stdio.h>

#define print(fmt, ...)   printf(fmt __VA_OPT__(, ) __VA_ARGS__)
#define println(fmt, ...) printf(fmt "\n" __VA_OPT__(, ) __VA_ARGS__)

#ifdef NDEBUG
#  define __display(fmt, ...)
#else
#  define __display(fmt, ...) fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)
#endif

#define inf(fmt, ...)                                                                              \
  __display("> \x1b[02m[%s $%d %s]\x1b[0m {%d}: " fmt "\n",                                        \
            __file__,                                                                              \
            __line__,                                                                              \
            __func__,                                                                              \
            errno __VA_OPT__(, ) __VA_ARGS__)

extern void __printh(u_cstr_t name, cu8_t* mem, size_t size);
extern void __printb(u_cstr_t name, cu8_t* mem, size_t size);

#define printh(mem, size) __printh(#mem, (u8_t*)mem, size);
#define printb(mem, size) __printb(#mem, (u8_t*)mem, size);

#endif /* !U_PRINT_H__ */
