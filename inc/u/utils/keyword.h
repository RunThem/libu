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

#ifndef U_KEYWORD_H__
#  define U_KEYWORD_H__

#  ifdef __cplusplus
extern "C" {
#  endif

#  define __file__ __FILE__
#  define __line__ __LINE__
#  define __func__ __func__

#  define auto __auto_type

#  define pub        /* public */
#  define pri static /* private */

#  if __has_builtin(__builtin_classify_type)
#    define typeclassify(t) (__builtin_classify_type(t))
#    define is_ptr(t)       (typeclassify(t) == 5)
#  else
#    error "!__has_builtin(__builtin_classify_type)"
#  endif

#  if __has_builtin(__builtin_types_compatible_p)
#    define typeeq(t1, t2) (__builtin_types_compatible_p(typeof(t1), typeof(t2)))
#  else
#    error "!__has_builtin(__builtin_types_compatible_p)"
#  endif

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_KEYWORD_H__ */
