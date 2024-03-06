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

#ifndef U_MISC_H__
#  define U_MISC_H__

#  define each(i, num) for (size_t i = 0; (i) < (num); (i)++)

#  define any(p)        ((any_t)(p))
#  define me(type, ...) ((type){__VA_ARGS__})
#  define bit(byte, n)  (((byte) >> (n)) & 1)

/* linux array size macro */
#  define __same_type(a, b)      __builtin_types_compatible_p(typeof(a), typeof(b))
#  define __build_bug_on_zero(e) (sizeof(struct { int : -!!(e); }))
#  define __must_be_array(a)     __build_bug_on_zero(__same_type((a), &(a)[0]))
#  define array_size(arr)        (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

#  define swap(a, b)                                                                               \
    do {                                                                                           \
      auto _swap__tmp = a;                                                                         \
                                                                                                   \
      (a) = (b);                                                                                   \
      (b) = (_swap__tmp);                                                                          \
    } while (0)

#  define min(x, y)                                                                                \
    ({                                                                                             \
      auto __min_x__ = (x);                                                                        \
      auto __min_y__ = (y);                                                                        \
                                                                                                   \
      (void)(&__min_x__ == &__min_y__);                                                            \
                                                                                                   \
      __min_x__ < __min_y__ ? __min_x__ : __min_y__;                                               \
    })

#  define max(x, y)                                                                                \
    ({                                                                                             \
      auto __max_x__ = (x);                                                                        \
      auto __max_y__ = (y);                                                                        \
                                                                                                   \
      (void)(&__max_x__ == &__max_y__);                                                            \
                                                                                                   \
      __max_x__ > __max_y__ ? __max_x__ : __max_y__;                                               \
    })

/* clang-format off */
/*
 *
 * '==' => true
 * '!=' => false
 * */
#  define fn_eq(type, ...)                                                                           \
    va_elseif(va_has(__VA_ARGS__)) (                                                                 \
      fn_eq_##type(__VA_ARGS__)                                                                      \
    )(                                                                                               \
      fn_eq_##type                                                                                   \
    )

/*
 * '>'  => 1
 * '==' => 0
 * '<'  => -1*/
#  define fn_cmp(type, ...)                                                                          \
    va_elseif(va_has(__VA_ARGS__)) (                                                                 \
      fn_cmp_##type(__VA_ARGS__)                                                                     \
    )(                                                                                               \
      fn_cmp_##type                                                                                  \
    )
/* clang-format on */

#  define fn_compe_dec(type)                                                                       \
    extern bool fn_eq_##type(const void*, const void*);                                            \
    extern int fn_cmp_##type(const void*, const void*)

#  define fn_compe_def(type, eq, cmp)                                                              \
    inline bool fn_eq_##type(const void* _x, const void* _y) {                                     \
      type x = *(type*)_x, y = *(type*)_y;                                                         \
      return (eq);                                                                                 \
    }                                                                                              \
                                                                                                   \
    inline int fn_cmp_##type(const void* _x, const void* _y) {                                     \
      type x = *(type*)_x, y = *(type*)_y;                                                         \
      return (eq) ? 0 : ((cmp) ? 1 : -1);                                                          \
    }

fn_compe_dec(char);
fn_compe_dec(int);

fn_compe_dec(byte_t);
fn_compe_dec(i8_t);
fn_compe_dec(u8_t);
fn_compe_dec(i16_t);
fn_compe_dec(u16_t);
fn_compe_dec(i32_t);
fn_compe_dec(u32_t);
fn_compe_dec(i64_t);
fn_compe_dec(u64_t);

fn_compe_dec(size_t);
fn_compe_dec(ssize_t);

#  ifdef __SIZEOF_INT128__
fn_compe_dec(i128_t);
fn_compe_dec(u128_t);
#  endif

#endif /* !U_MISC_H__ */
