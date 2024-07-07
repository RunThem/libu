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

#ifndef U_IDBG_H__
#  define U_IDBG_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Api
 **************************************************************************************************/
[[gnu::format(printf, 4, 5)]]
extern void dbg_write(int, const char*, int, const char*, ...);

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  ifdef NDEBUG
#    define u_err(fmt, ...)
#    define u_war(fmt, ...)
#    define u_inf(fmt, ...)
#    define u_dbg(fmt, ...)
#  else /* NDEBUG */
#    define u_err(fmt, ...) dbg_write(0, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)
#    define u_war(fmt, ...) dbg_write(1, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)
#    define u_inf(fmt, ...) dbg_write(2, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)
#    define u_dbg(fmt, ...) dbg_write(3, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)
#  endif /* NDEBUG */

#  ifndef LIBU_DEBUG
#    define u_xxx(fmt, ...)
#  else /* !LIBU_DEBUG */
#    define u_xxx(fmt, ...) dbg_write(4, __file__, __line__, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)
#  endif /* !LIBU_DEBUG */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IDBG_H__ */
