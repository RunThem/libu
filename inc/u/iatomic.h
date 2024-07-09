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

#ifndef U_IATOMIC_H__
#  define U_IATOMIC_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct atomic_flag u_atomic_flag_t;

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
/* clang-format off */
#define u_once_call_def(fun) once_flag _##fun##__once_call_flag = ONCE_FLAG_INIT;

#define u_once_call(fun)                                                                           \
  do {                                                                                             \
    call_once(&_##fun##__once_call_flag, fun);                                                     \
  } while (0)

#define u_atomic_init(obj, v)                                                                      \
  do {                                                                                             \
    atomic_init(obj, v);                                                                           \
  } while (0)

#define u_atomic_put(obj, v, ...)                                                                  \
  do {                                                                                             \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_store_explicit(obj, v, u_va_at(0, __VA_ARGS__));                                      \
    ) (                                                                                            \
      atomic_store(obj, v);                                                                        \
    )                                                                                              \
  } while (0)


#define u_atomic_pop(obj, ...)                                                                     \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_load_explicit(obj, u_va_at(0, __VA_ARGS__));                                          \
    ) (                                                                                            \
      atomic_load(obj);                                                                            \
    )                                                                                              \
  })

#define u_atomic_swap(obj, v, ...)                                                                 \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_exchange_explicit(obj, v, u_va_at(0, __VA_ARGS__));                                   \
    ) (                                                                                            \
      atomic_exchange(obj, v);                                                                     \
    )                                                                                              \
  })

#define u_atomic_cswap(obj, c, v, ...)                                                             \
  ({                                                                                               \
    typeof_unqual(*(obj)) _ = c;                                                                   \
                                                                                                   \
    u_va_elseif(u_va_cnt_is(2, __VA_ARGS__)) (                                                     \
      atomic_compare_exchange_strong_explicit(obj,                                                 \
                                              &_,                                                  \
                                              v,                                                   \
                                              u_va_at(0, __VA_ARGS__),                             \
                                              u_va_at(1, __VA_ARGS__));                            \
    ) (                                                                                            \
      atomic_compare_exchange_strong(obj, &_, v);                                                  \
    )                                                                                              \
  })

#define u_atomic_add(obj, v, ...)                                                                  \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_fetch_add_explicit(obj, v, u_va_at(0, __VA_ARGS__));                                  \
    ) (                                                                                            \
      atomic_fetch_add(obj, v);                                                                    \
    )                                                                                              \
  })

#define u_atomic_sub(obj, v, ...)                                                                  \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_fetch_sub_explicit(obj, v, u_va_at(0, __VA_ARGS__));                                  \
    ) (                                                                                            \
      atomic_fetch_sub(obj, v);                                                                    \
    )                                                                                              \
  })

#define u_atomic_and(obj, v, ...)                                                                  \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_fetch_and_explicit(obj, v, u_va_at(0, __VA_ARGS__));                                  \
    ) (                                                                                            \
      atomic_fetch_and(obj, v);                                                                    \
    )                                                                                              \
  })

#define u_atomic_or(obj, v, ...)                                                                   \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_fetch_or_explicit(obj, v, u_va_at(0, __VA_ARGS__));                                   \
    ) (                                                                                            \
      atomic_fetch_or(obj, v);                                                                     \
    )                                                                                              \
  })

#define u_atomic_xor(obj, v, ...)                                                                  \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_fetch_xor_explicit(obj, v, u_va_at(0, __VA_ARGS__));                                  \
    ) (                                                                                            \
      atomic_fetch_xor(obj, v);                                                                    \
    )                                                                                              \
  })

#define u_atomic_flag_true(obj, ...)                                                               \
  ({                                                                                               \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_flag_test_and_set_explicit(obj, u_va_at(0, __VA_ARGS__));                             \
    ) (                                                                                            \
      atomic_flag_test_and_set(obj);                                                               \
    )                                                                                              \
  })

#define u_atomic_flag_false(obj, ...)                                                              \
  do {                                                                                             \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      atomic_flag_clear_explicit(obj, u_va_at(0, __VA_ARGS__));                                    \
    ) (                                                                                            \
      atomic_flag_clear(obj);                                                                      \
    )                                                                                              \
  } while (0)
/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IATOMIC_H__ */
