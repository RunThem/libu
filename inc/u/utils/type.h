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

#ifndef U_TYPE_H__
#  define U_TYPE_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/* clang-format off */
typedef int ret_t;

typedef char* u_cstr_t;

typedef char        byte_t;
typedef void*       any_t;
typedef const void* cany_t;

typedef int8_t   i8_t;
typedef uint8_t  u8_t;
typedef int16_t  i16_t;
typedef uint16_t u16_t;
typedef int32_t  i32_t;
typedef uint32_t u32_t;
typedef int64_t  i64_t;
typedef uint64_t u64_t;

typedef float       f32_t;
typedef double      f64_t;
typedef long double f128_t;

#  ifdef __SIZEOF_INT128__
typedef __int128_t  i128_t;
typedef __uint128_t u128_t;
#  endif
/* clang-format on */

#  define fnt(n, r, ...) r (*(n))(__VA_ARGS__)

/*
 * a >  b  ->  1
 * a == b  ->  0
 * a <  b  -> -1
 * */
typedef fnt(u_cmp_fn, int, cany_t, cany_t);
typedef fnt(u_eq_fn, bool, cany_t, cany_t);

// typedef struct {
// }* u_str_t;

typedef u64_t u_hash_t;

typedef fnt(u_hash_fn, u_hash_t, const u8_t*, size_t);

typedef struct {
  u32_t type;
  u32_t len;
  u8_t val[];
} u_tlv_t;

typedef struct {
  u32_t len;
  u8_t buf[];
} u_msg_t;

typedef enum {
  U_ORDER_DESCEND = -1,
  U_ORDER_ASCEND  = 1,
} u_order_e;

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_TYPE_H__ */
