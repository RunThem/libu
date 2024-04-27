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
}* __u_vec_t;

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
#define u_vec_t(...) typeof(__u_vec_t(*)(ssize_t, __VA_ARGS__))

#define __u_vec_def(T)                                                                             \
  u_vec_t(T) : (T) {                                                                               \
  }

#define u_vec_type(u)       typeof(_Generic(typeof(u), va_map(__u_vec_def, va_unpack(u_vec_def))))
#define u_vec_type_val(u)   _Generic(typeof(u), va_map(__u_vec_def, va_unpack(u_vec_def)))
#define u_vec_type_check(u) static_assert(typeeq((__u_vec_t){}, u(0, u_vec_type_val(u))))

/***************************************************************************************************
 * iApi vec
 **************************************************************************************************/
#define u_vec_init(u)                                                                              \
  do {                                                                                             \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    u = vec_new(sizeof(u_vec_type(u)));                                                            \
  } while (0)

#define u_vec_new(...)                                                                             \
  ({                                                                                               \
    u_vec_t(__VA_ARGS__) u = nullptr;                                                              \
                                                                                                   \
    u_vec_init(u);                                                                                 \
                                                                                                   \
    u;                                                                                             \
  })

#define u_vec_len(u)                                                                               \
  ({                                                                                               \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    vec_len(u);                                                                                    \
  })

#define u_vec_cap(u)                                                                               \
  ({                                                                                               \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    vec_cap(u);                                                                                    \
  })

#define u_vec_is_empty(u)                                                                          \
  ({                                                                                               \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    0 == vec_len(u);                                                                               \
  })

#define u_vec_is_exist(u, idx)                                                                     \
  ({                                                                                               \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    ssize_t _a = idx;                                                                              \
                                                                                                   \
    vec_exist(u, _a);                                                                              \
  })

#define u_vec_clear(u)                                                                             \
  do {                                                                                             \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    vec_clear(u);                                                                                  \
  } while (0)

#define u_vec_cleanup(u)                                                                           \
  do {                                                                                             \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    vec_cleanup(u);                                                                                \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

/* clang-format off */
#define u_vec_at(u, ...)                                                                           \
  va_elseif(va_size_is(2, __VA_ARGS__)) (                                                          \
    ({                                                                                             \
      u_vec_type_check(u);                                                                         \
                                                                                                   \
      bool _ret      = false;                                                                      \
      ssize_t _a     = va_at(0, __VA_ARGS__);                                                      \
      u_vec_type(u)* _b = {};                                                                      \
                                                                                                   \
      _b = vec_at(u, _a);                                                                          \
                                                                                                   \
      if (_b != nullptr) {                                                                         \
        *_b = va_at(1, __VA_ARGS__);                                                               \
        _ret = true;                                                                               \
      }                                                                                            \
                                                                                                   \
      _ret;                                                                                        \
    })                                                                                             \
  ) (                                                                                              \
    ({                                                                                             \
      u_vec_type_check(u);                                                                         \
                                                                                                   \
      auto _a        = va_at(0, __VA_ARGS__);                                                      \
      u_vec_type(u) _it = {};                                                                      \
      u_vec_type(u)* _b = {};                                                                      \
                                                                                                   \
      _b = vec_at(u, _a);                                                                          \
                                                                                                   \
      if (_b == nullptr) {                                                                         \
        _b = &_it;                                                                                 \
      }                                                                                            \
                                                                                                   \
      *_b;                                                                                         \
    })                                                                                             \
  )
/* clang-format on */

#define u_vec_try(u, i) for (u_vec_type(u)* it = vec_at(u, i); it != nullptr; it = nullptr)

/* clang-format off */
#define u_vec_pop(u, ...)                                                                          \
  ({                                                                                               \
    static_assert((va_size(__VA_ARGS__) == 1) + (va_size(__VA_ARGS__) == 0),                       \
                  "The number of '...' is 1 or 0.");                                               \
                                                                                                   \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    va_elseif(va_size_is(1, __VA_ARGS__)) (                                                        \
      ssize_t _a    = va_at(0, __VA_ARGS__);                                                       \
      u_vec_type(u) _b = {};                                                                       \
    ) (                                                                                            \
      ssize_t _a    = -1;                                                                          \
      u_vec_type(u) _b = {};                                                                       \
    )                                                                                              \
                                                                                                   \
    vec_pop(u, _a, &_b);                                                                           \
                                                                                                   \
    _b;                                                                                            \
  })

#define u_vec_put(u, ...)                                                                          \
  do {                                                                                             \
    static_assert((va_size(__VA_ARGS__) == 1) + (va_size(__VA_ARGS__) == 2),                       \
                  "The number of '...' is 1 or 2.");                                               \
                                                                                                   \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    va_elseif(va_size_is(2, __VA_ARGS__)) (                                                        \
      auto _a       = va_at(0, __VA_ARGS__);                                                       \
      u_vec_type(u) _b = va_at(1, __VA_ARGS__);                                                    \
    )(                                                                                             \
      auto _a       = -1;                                                                          \
      u_vec_type(u) _b = va_at(0, __VA_ARGS__);                                                    \
    )                                                                                              \
                                                                                                   \
    vec_put(u, _a, &_b);                                                                           \
  } while (0)
/* clang-format on */

#define u_vec_sort(u, ...)                                                                         \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    u_vec_type_check(u);                                                                           \
                                                                                                   \
    vec_sort(u, va_at(0, __VA_ARGS__));                                                            \
  } while (0)

#define u_vec_for(u, i, it)                                                                        \
  for (ssize_t i = 0; vec_for_init(u, 1); vec_for_end(u))                                          \
    for (u_vec_type(u) it = {}; vec_for(u, &i, &it);)

#define u_vec_rfor(u, i, it)                                                                       \
  for (ssize_t i = 0; vec_for_init(u, 0); vec_for_end(u))                                          \
    for (u_vec_type(u) it = {}; vec_for(u, &i, &it);)

#endif /* !U_IVEC_H__ */
