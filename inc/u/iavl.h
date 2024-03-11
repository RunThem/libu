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

#ifndef U_IAVL_H__
#define U_IAVL_H__

#include "utils/type.h"
#include "utils/va.h"

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_avl_t;

/***************************************************************************************************
 * Let
 **************************************************************************************************/
extern u_avl_t u_iavl;

/***************************************************************************************************
 * Api
 ***************s***********************************************************************************/
extern any_t avl_new(size_t, size_t, u_cmp_fn);

extern size_t avl_len(any_t);

extern bool avl_exist(any_t, any_t);

extern void avl_clear(any_t);

extern void avl_cleanup(any_t);

extern any_t avl_at(any_t, any_t);

extern void avl_pop(any_t, any_t, any_t);

extern void avl_put(any_t, any_t, any_t);

extern bool avl_for_init(any_t, bool);

extern void avl_for_end(any_t);

extern bool avl_for(any_t, any_t, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define uavl(K, V) typeof(V(*(*)(u_avl_t, K*, V*))[sizeof(K)][sizeof(V)])

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#define ut_init(u, ...)                                                                            \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
    typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    u = avl_new(sizeof(typeof(*u(nullptr, nullptr, nullptr))) /                                    \
                    sizeof(typeof(**u(nullptr, nullptr, nullptr))),                                \
                sizeof(typeof(**u(nullptr, nullptr, nullptr))) /                                   \
                    sizeof(typeof(***u(nullptr, nullptr, nullptr))),                               \
                va_at(0, __VA_ARGS__));                                                            \
  } while (0)

#define ut_new(K, V, ...)                                                                          \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    uavl(K, V) u = nullptr;                                                                        \
                                                                                                   \
    ut_init(u, va_at(0, __VA_ARGS__));                                                             \
                                                                                                   \
    u;                                                                                             \
  })

#define ut_len(u)                                                                                  \
  ({                                                                                               \
    typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    avl_len(u);                                                                                    \
  })

#define ut_empty(u)                                                                                \
  ({                                                                                               \
    typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    0 == avl_len(u);                                                                               \
  })

#define ut_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                               = va_at(0, __VA_ARGS__);                                 \
    typeof(***u(u_iavl, &_a, nullptr)) _b = {};                                                    \
                                                                                                   \
    avl_exist(u, &_a);                                                                             \
  })

#define ut_clear(u)                                                                                \
  do {                                                                                             \
    typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    avl_clear(u);                                                                                  \
  } while (0)

#define ut_cleanup(u)                                                                              \
  do {                                                                                             \
    typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    avl_cleanup(u);                                                                                \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

/* clang-format off */
#  define ut_at(u, ...)                                                                            \
    va_elseif(va_size_is(2, __VA_ARGS__)) (                                                        \
      ({                                                                                           \
        static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                     \
                                                                                                   \
        bool _ret                      = false;                                                    \
        auto _a                        = va_at(0, __VA_ARGS__);                                    \
        typeof(***u(u_iavl, &_a, nullptr))* _b = {};                                               \
                                                                                                   \
        _b = avl_at(u, &_a);                                                                       \
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
        typeof(***u(u_iavl, &_a, nullptr)) _it = {};                                               \
        typeof(***u(u_iavl, &_a, nullptr))* _b = {};                                               \
                                                                                                   \
        _b = avl_at(u, &_a);                                                                       \
                                                                                                   \
        if (_b == nullptr) {                                                                       \
          _b = &_it;                                                                               \
        }                                                                                          \
                                                                                                   \
        *_b;                                                                                       \
      })                                                                                           \
    )
/* clang-format on */

#define ut_try(u, k)                                                                               \
  for (typeof(***u(u_iavl, &(typeof(k)){k}, nullptr))* it = avl_at(u, &(typeof(k)){k});            \
       it != nullptr;                                                                              \
       it = nullptr)

#define ut_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                               = va_at(0, __VA_ARGS__);                                 \
    typeof(***u(u_iavl, &_a, nullptr)) _b = {};                                                    \
                                                                                                   \
    avl_pop(u, &_a, &_b);                                                                          \
                                                                                                   \
    _b;                                                                                            \
  })

#define ut_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                               = va_at(0, __VA_ARGS__);                                 \
    typeof(***u(u_iavl, &_a, nullptr)) _b = va_at(1, __VA_ARGS__);                                 \
                                                                                                   \
    avl_put(u, &_a, &_b);                                                                          \
  } while (0)

#define ut_for_all(u, k, v, K)                                                                     \
  for (K k = {}; avl_for_init(u, 1); avl_for_end(u))                                               \
    for (typeof(***u(u_iavl, &k, nullptr)) v = {}; avl_for(u, &k, &v);)

#define ut_rfor_all(u, k, v, K)                                                                    \
  for (K k = {}; avl_for_init(u, 0); avl_for_end(u))                                               \
    for (typeof(***u(u_iavl, &k, nullptr)) v = {}; avl_for(u, &k, &v);)

#endif /* !U_IAVL_H__ */
