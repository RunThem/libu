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
}* u_buf_ref_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern u_buf_ref_t  buf_new     (u8_t*, size_t);
extern void         buf_clear   (u_buf_ref_t);
extern void         buf_cleanup (u_buf_ref_t);
extern size_t       buf_len     (u_buf_ref_t);
extern void         buf_pop     (u_buf_ref_t, any_t, size_t);
extern void         buf_put     (u_buf_ref_t, any_t, size_t);
/* clang-format on */

/***************************************************************************************************
 * iApi buf
 **************************************************************************************************/
#  define u_buf_new(rawbuf, cap)                                                                   \
    ({                                                                                             \
      ;                                                                                            \
      buf_new(rawbuf, cap);                                                                        \
    })

#  define u_buf_len(self)                                                                          \
    ({                                                                                             \
      ;                                                                                            \
      buf_len(self);                                                                               \
    })

#  define u_buf_is_empty(self)                                                                     \
    ({                                                                                             \
      ;                                                                                            \
      0 == buf_len(self);                                                                          \
    })

#  define u_buf_clear(self)                                                                        \
    do {                                                                                           \
      buf_clear(self);                                                                             \
    } while (0)

#  define u_buf_cleanup(self)                                                                      \
    do {                                                                                           \
      buf_cleanup(self);                                                                           \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

/* clang-format off */
#define u_buf_pop(self, ...)                                                                       \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      typeof(u_va_at(0, __VA_ARGS__)) __a   = {};                                                  \
                                                                                                   \
      buf_pop(self, &__a, sizeof(__a)) ;                                                           \
                                                                                                   \
      _a;                                                                                          \
    ) (                                                                                            \
      buf_pop(self, u_va_at(0, __VA_ARGS__), u_va_at(1, __VA_ARGS__));                             \
                                                                                                   \
      true;                                                                                        \
    )                                                                                              \
  })

#define u_buf_put(self, ...)                                                                       \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      typeof(u_va_at(0, __VA_ARGS__)) __a   = u_va_at(0, __VA_ARGS__);                             \
                                                                                                   \
      buf_put(self, &__a, sizeof(__a)) ;                                                           \
    ) (                                                                                            \
      buf_put(self, u_va_at(0, __VA_ARGS__), u_va_at(1, __VA_ARGS__));                             \
    )                                                                                              \
  })
/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IBUF_H__ */
