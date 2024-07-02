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

#ifndef U_IBUF_H__
#  define U_IBUF_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
  bool alloc_flag;
  size_t len;
  size_t cap;

  u8_t* buf;
  u8_t* __rawbuf;
} u_buf_t;

/***************************************************************************************************
 * Api
 ***************************************************************************************************/
extern void buf_init(u_buf_t*, u8_t*, size_t);

extern void buf_clear(u_buf_t*);

extern void buf_cleanup(u_buf_t*);

extern size_t buf_len(u_buf_t*);

extern void buf_skip(u_buf_t*, size_t);

extern void buf_pop(u_buf_t*, any_t, size_t);

extern void buf_put(u_buf_t*, any_t, size_t);

/***************************************************************************************************
 * iApi buf
 **************************************************************************************************/
/* clang-format off */
#define u_buf_init(u, ...)                                                                         \
  do {                                                                                             \
    va_elseif(va_size_is(0, __VA_ARGS__)) (                                                        \
      buf_init(u, nullptr, 32);                                                                    \
    )(                                                                                             \
      va_elseif(va_size_is(1, __VA_ARGS__)) (                                                      \
        buf_init(u, nullptr, va_at(0, __VA_ARGS__));                                               \
      )(                                                                                           \
        buf_init(u, va_at(0, __VA_ARGS__), va_at(1, __VA_ARGS__));                                 \
      )                                                                                            \
    )                                                                                              \
  } while (0)
/* clang-format on */

#  define u_buf_len(u)                                                                             \
    ({                                                                                             \
      ;                                                                                            \
      buf_len(u);                                                                                  \
    })

#  define u_buf_is_empty(u)                                                                        \
    ({                                                                                             \
      ;                                                                                            \
      0 == buf_len(u);                                                                             \
    })

#  define u_buf_clear(u)                                                                           \
    do {                                                                                           \
      buf_clear(u);                                                                                \
    } while (0)

#  define u_buf_cleanup(u)                                                                         \
    do {                                                                                           \
      buf_cleanup(u);                                                                              \
    } while (0)

#  define u_buf_skip(u, len)                                                                       \
    do {                                                                                           \
      buf_skip(u, len);                                                                            \
    } while (0)

/* clang-format off */
#define u_buf_pop(u, ...)                                                                          \
  ({                                                                                               \
    va_elseif(va_size_is(1, __VA_ARGS__)) (                                                        \
      typeof(va_at(0, __VA_ARGS__)) _a   = {};                                                     \
                                                                                                   \
      buf_pop(u, &_a, sizeof(_a)) ;                                                                \
                                                                                                   \
      _a;                                                                                          \
    ) (                                                                                            \
      buf_pop(u, va_at(0, __VA_ARGS__), va_at(1, __VA_ARGS__));                                    \
                                                                                                   \
      true;                                                                                        \
    )                                                                                              \
  })

#define u_buf_put(u, ...)                                                                          \
  ({                                                                                               \
    va_elseif(va_size_is(1, __VA_ARGS__)) (                                                        \
      typeof(va_at(0, __VA_ARGS__)) _a   = va_at(0, __VA_ARGS__);                                  \
                                                                                                   \
      buf_put(u, &_a, sizeof(_a)) ;                                                                \
    ) (                                                                                            \
      buf_pop(u, va_at(0, __VA_ARGS__), va_at(1, __VA_ARGS__));                                    \
    )                                                                                              \
  })
/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IBUF_H__ */
