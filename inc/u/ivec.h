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

#ifndef U_IVEC_H__
#define U_IVEC_H__

#include "utils/type.h"
#include "utils/va.h"

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_vec_t;

/***************************************************************************************************
 * Let
 **************************************************************************************************/
extern u_vec_t u_ivec;

/***************************************************************************************************
 * Api
 ***************s***********************************************************************************/
extern any_t vec_new(size_t);

extern size_t vec_len(any_t);

extern size_t vec_cap(any_t);

extern bool vec_exist(any_t, size_t);

extern void vec_clear(any_t);

extern void vec_cleanup(any_t);

extern any_t vec_at(any_t, ssize_t);

extern void vec_pop(any_t, ssize_t, any_t);

extern void vec_put(any_t, ssize_t, any_t);

extern void vec_sort(any_t, u_cmp_fn);

extern bool vec_for_init(any_t, bool);

extern void vec_for_end(any_t);

extern bool vec_for(any_t, ssize_t*, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define uvec(T) typeof(T(*(*)(u_vec_t, ssize_t, T*))[sizeof(ssize_t)][sizeof(T)])

/***************************************************************************************************
 * iApi vec
 **************************************************************************************************/
#define uv_init(u)                                                                                 \
  do {                                                                                             \
    typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                    \
                                                                                                   \
    u = vec_new(sizeof(typeof(**u(nullptr, 0, nullptr))) /                                         \
                sizeof(typeof(***u(nullptr, 0, nullptr))));                                        \
  } while (0)

#define uv_new(T)                                                                                  \
  ({                                                                                               \
    uvec(T) u = nullptr;                                                                           \
                                                                                                   \
    uv_init(u);                                                                                    \
                                                                                                   \
    u;                                                                                             \
  })

#define uv_len(u)                                                                                  \
  ({                                                                                               \
    typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                    \
                                                                                                   \
    vec_len(u);                                                                                    \
  })

#define uv_cap(u)                                                                                  \
  ({                                                                                               \
    typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                    \
                                                                                                   \
    vec_cap(u);                                                                                    \
  })

#define uv_empty(u)                                                                                \
  ({                                                                                               \
    typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                    \
                                                                                                   \
    0 == vec_len(u);                                                                               \
  })

#define uv_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                              = va_at(0, __VA_ARGS__);                                  \
    typeof(***u(u_ivec, _a, nullptr)) _b = {};                                                     \
                                                                                                   \
    vec_exist(u, _a);                                                                              \
  })

#define uv_clear(u)                                                                                \
  do {                                                                                             \
    typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                    \
                                                                                                   \
    vec_clear(u);                                                                                  \
  } while (0)

#define uv_cleanup(u)                                                                              \
  do {                                                                                             \
    typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                    \
                                                                                                   \
    vec_cleanup(u);                                                                                \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

/* clang-format off */
#  define uv_at(u, ...)                                                                            \
    va_elseif(va_size_is(2, __VA_ARGS__)) (                                                        \
      ({                                                                                           \
        static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                     \
                                                                                                   \
        bool _ret                      = false;                                                    \
        auto _a                        = va_at(0, __VA_ARGS__);                                    \
        typeof(***u(u_ivec, _a, nullptr))* _b = {};                                                \
                                                                                                   \
        _b = vec_at(u, _a);                                                                        \
                                                                                                   \
        if (_b != nullptr) {                                                                       \
          *_b = va_at(1, __VA_ARGS__);                                                             \
          _ret = true;                                                                             \
        }                                                                                          \
                                                                                                   \
        _ret;                                                                                      \
      })                                                                                           \
    ) (                                                                                            \
      ({                                                                                           \
        static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                     \
                                                                                                   \
        auto _a                        = va_at(0, __VA_ARGS__);                                    \
        typeof(***u(u_ivec, _a, nullptr)) _it = {};                                                \
        typeof(***u(u_ivec, _a, nullptr))* _b = {};                                                \
                                                                                                   \
        _b = vec_at(u, _a);                                                                        \
                                                                                                   \
        if (_b == nullptr) {                                                                       \
          _b = &_it;                                                                               \
        }                                                                                          \
                                                                                                   \
        *_b;                                                                                       \
      })                                                                                           \
    )
/* clang-format on */

#define uv_try(u, i)                                                                               \
  for (typeof(***u(u_ivec, i, nullptr))* it = vec_at(u, i); it != nullptr; it = nullptr)

/* clang-format off */
#  define uv_pop(u, ...)                                                                           \
    ({                                                                                             \
      static_assert((va_size(__VA_ARGS__) == 1) + (va_size(__VA_ARGS__) == 0),                     \
                    "The number of '...' is 1 or 0.");                                             \
                                                                                                   \
      va_elseif(va_size_is(1, __VA_ARGS__)) (                                                      \
        auto _a                              = va_at(0, __VA_ARGS__);                              \
        typeof(***u(u_ivec, _a, nullptr)) _b = {};                                                 \
      ) (                                                                                          \
        auto _a                              = -1;                                                 \
        typeof(***u(u_ivec, _a, nullptr)) _b = {};                                                 \
      )                                                                                            \
                                                                                                   \
      vec_pop(u, _a, &_b);                                                                         \
                                                                                                   \
      _b;                                                                                          \
    })

#  define uv_put(u, ...)                                                                           \
    do {                                                                                           \
      static_assert((va_size(__VA_ARGS__) == 1) + (va_size(__VA_ARGS__) == 2),                     \
                    "The number of '...' is 1 or 2.");                                             \
                                                                                                   \
      va_elseif(va_size_is(2, __VA_ARGS__)) (                                                      \
        auto _a                              = va_at(0, __VA_ARGS__);                              \
        typeof(***u(u_ivec, _a, nullptr)) _b = va_at(1, __VA_ARGS__);                              \
      ) (                                                                                          \
        auto _a                              = -1;                                                 \
        typeof(***u(u_ivec, _a, nullptr)) _b = va_at(0, __VA_ARGS__);                              \
      )                                                                                            \
                                                                                                   \
      vec_put(u, _a, &_b);                                                                         \
    } while (0)
/* clang-format on */

#define uv_sort(u, ...)                                                                            \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                    \
                                                                                                   \
    vec_sort(u, va_at(0, __VA_ARGS__));                                                            \
  } while (0)

#define uv_for_all(u, i, it)                                                                       \
  for (ssize_t i = 0; vec_for_init(u, 1); vec_for_end(u))                                          \
    for (typeof(***u(u_ivec, i, nullptr)) it = {}; vec_for(u, &i, &it);)

#define uv_rfor_all(u, i, it)                                                                      \
  for (ssize_t i = 0; vec_for_init(u, 0); vec_for_end(u))                                          \
    for (typeof(***u(u_ivec, i, nullptr)) it = {}; vec_for(u, &i, &it);)

#endif /* !U_IVEC_H__ */
