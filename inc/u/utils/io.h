
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

#ifndef U_IO_H__
#define U_IO_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "misc.h"
#include "type.h"

#include <stdarg.h>

#define __vsnprintf_map(v) __builtin_basetypeid(v), v
extern size_t __vsnprintf(u_cstr_t buf, size_t maxl, u_cstr_t fmt, va_list va);

extern size_t __printf(u_cstr_t fmt, ...);
#define u_printf(fmt, ...) __printf(fmt __VA_OPT__(, ) va_map(__vsnprintf_map, __VA_ARGS__))

// extern void __printh(u_cstr_t name, const u8_t* mem, size_t size);
// extern void __printb(u_cstr_t name, const u8_t* mem, size_t size);

// #define printh(mem, size) __printh(#mem, (u8_t*)mem, size);
// #define printb(mem, size) __printb(#mem, (u8_t*)mem, size);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_IO_H__ */
