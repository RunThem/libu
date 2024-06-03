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

#ifndef U_MISC_H__
#define U_MISC_H__

#include "type.h"

#include <errno.h>
#include <setjmp.h>

/***************************************************************************************************
 * Misc macro
 **************************************************************************************************/
#define any(p)        ((any_t)(p))
#define ch(c)         ((char)(#c[0]))
#define me(type, ...) ((type){__VA_ARGS__})
#define bit(byte, n)  (((byte) >> (n)) & 1)

/* clang-format off */
#define each(i, n, ...) va_elseif(va_size_is(1, __VA_ARGS__)) (                                    \
      for (size_t i = n; i < va_at(0, __VA_ARGS__); i++)                                           \
    )(                                                                                             \
      for (size_t i = 0; i < n; i++)                                                               \
    )
/* clang-format on */

#define mtx_if(mtx) for (bool _ = true; _ && !mtx_lock(mtx); _ = false, mtx_unlock(mtx))

#define align_of(addr, size) ({ ((addr) + (size)-1) & (~((size)-1)); })

#define container_of(ptr, type, member)                                                            \
  ({                                                                                               \
    const typeof(((type*)0)->member)* _container_of__mptr = any(ptr);                              \
    (type*)((char*)_container_of__mptr - offsetof(type, member));                                  \
  })

/* linux array size macro */
#define __same_type(a, b)      __builtin_types_compatible_p(typeof(a), typeof(b))
#define __build_bug_on_zero(e) (sizeof(struct { int : -!!(e); }))
#define __must_be_array(a)     __build_bug_on_zero(__same_type((a), &(a)[0]))
#define arrlen(arr)            (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))

/***************************************************************************************************
 * Try catch
 **************************************************************************************************/
#define U_ERR_MSG_SIZE 2048

typedef struct {
  bool is_err;
  jmp_buf label;
  u_cstr_t file;
  u_cstr_t func;
  u_cstr_t expr;
  size_t line;
  error_t error;
  int id;
  char msg[U_ERR_MSG_SIZE];
} __err__t;

extern thread_local __err__t __err__;

/* clang-format off */
#define try      for (bzero(&__err__, sizeof(__err__)); !setjmp(__err__.label);)
#define catch(e) for (auto e = __err__; e.is_err; e.is_err = false)
#define panic(_expr, _id, args...)                                                               \
  do {                                                                                           \
    __err__.is_err = true;                                                                       \
    __err__.file   = __file__;                                                                   \
    __err__.func   = __func__;                                                                   \
    __err__.line   = __line__;                                                                   \
    __err__.expr   = #_expr;                                                                     \
    __err__.id     = _id;                                                                        \
    __err__.error  = errno;                                                                      \
                                                                                                 \
    va_if(va_has(args)) (                                                                        \
      snprintf(__err__.msg, U_ERR_MSG_SIZE, args);                                               \
    )                                                                                            \
                                                                                                 \
    longjmp(__err__.label, 1);                                                                   \
  } while (0)
/* clang-format on */

/***************************************************************************************************
 * Swap
 **************************************************************************************************/
#define swap(a, b)                                                                                 \
  do {                                                                                             \
    auto _swap__tmp = a;                                                                           \
                                                                                                   \
    (a) = (b);                                                                                     \
    (b) = (_swap__tmp);                                                                            \
  } while (0)

/***************************************************************************************************
 * Compe
 **************************************************************************************************/
#define min(x, y)                                                                                  \
  ({                                                                                               \
    auto __min_x__ = (x);                                                                          \
    auto __min_y__ = (y);                                                                          \
                                                                                                   \
    (void)(&__min_x__ == &__min_y__);                                                              \
                                                                                                   \
    __min_x__ < __min_y__ ? __min_x__ : __min_y__;                                                 \
  })

#define max(x, y)                                                                                  \
  ({                                                                                               \
    auto __max_x__ = (x);                                                                          \
    auto __max_y__ = (y);                                                                          \
                                                                                                   \
    (void)(&__max_x__ == &__max_y__);                                                              \
                                                                                                   \
    __max_x__ > __max_y__ ? __max_x__ : __max_y__;                                                 \
  })

/* clang-format off */
/*
 *
 * '==' => true
 * '!=' => false
 * */
#define fn_eq(type, ...)                                                                           \
  va_elseif(va_has(__VA_ARGS__)) (                                                                 \
    fn_eq_##type(__VA_ARGS__)                                                                      \
  )(                                                                                               \
    fn_eq_##type                                                                                   \
  )

/*
 * '>'  => 1
 * '==' => 0
 * '<'  => -1*/
#define fn_cmp(type, ...)                                                                          \
  va_elseif(va_has(__VA_ARGS__)) (                                                                 \
    fn_cmp_##type(__VA_ARGS__)                                                                     \
  )(                                                                                               \
    fn_cmp_##type                                                                                  \
  )
/* clang-format on */

#define fn_compe_dec(type)                                                                         \
  extern bool fn_eq_##type(cany_t, cany_t);                                                        \
  extern int fn_cmp_##type(cany_t, cany_t)

#define fn_compe_def(type, eq, cmp)                                                                \
  bool fn_eq_##type(cany_t _x, cany_t _y) {                                                        \
    type x = *(type*)_x, y = *(type*)_y;                                                           \
    return (eq);                                                                                   \
  }                                                                                                \
                                                                                                   \
  int fn_cmp_##type(cany_t _x, cany_t _y) {                                                        \
    type x = *(type*)_x, y = *(type*)_y;                                                           \
    return (eq) ? 0 : ((cmp) ? 1 : -1);                                                            \
  }

fn_compe_dec(char);
fn_compe_dec(int);
fn_compe_dec(uint);

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

#ifdef __SIZEOF_INT128__
fn_compe_dec(i128_t);
fn_compe_dec(u128_t);
#endif

#endif /* !U_MISC_H__ */
