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

#ifndef U_IMAP_H__
#define U_IMAP_H__

#include "utils/type.h"
#include "utils/va.h"

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_map_t;

/***************************************************************************************************
 * Let
 **************************************************************************************************/
extern u_map_t u_imap;

/***************************************************************************************************
 * Api
 ***************s***********************************************************************************/
extern any_t map_new(size_t, size_t);

extern size_t map_len(any_t);

extern bool map_exist(any_t, any_t);

extern void map_clear(any_t);

extern void map_cleanup(any_t);

extern any_t map_at(any_t, any_t);

extern void map_pop(any_t, any_t, any_t);

extern void map_put(any_t, any_t, any_t);

extern void vec_sort(any_t, u_cmp_fn);

extern bool map_for_init(any_t, bool);

extern void map_for_end(any_t);

extern bool map_for(any_t, any_t, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define umap(K, V) typeof(V(*(*)(u_map_t, K*, V*))[sizeof(K)][sizeof(V)])

/***************************************************************************************************
 * iApi map
 **************************************************************************************************/
#define um_init(u)                                                                                 \
  do {                                                                                             \
    typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    u = map_new(sizeof(typeof(*u(nullptr, nullptr, nullptr))) /                                    \
                    sizeof(typeof(**u(nullptr, nullptr, nullptr))),                                \
                sizeof(typeof(**u(nullptr, nullptr, nullptr))) /                                   \
                    sizeof(typeof(***u(nullptr, nullptr, nullptr))));                              \
  } while (0)

#define um_new(K, V)                                                                               \
  ({                                                                                               \
    umap(K, V) u = nullptr;                                                                        \
                                                                                                   \
    um_init(u);                                                                                    \
                                                                                                   \
    u;                                                                                             \
  })

#define um_len(u)                                                                                  \
  ({                                                                                               \
    typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    map_len(u);                                                                                    \
  })

#define um_empty(u)                                                                                \
  ({                                                                                               \
    typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    0 == map_len(u);                                                                               \
  })

#define um_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                               = va_at(0, __VA_ARGS__);                                 \
    typeof(***u(u_imap, &_a, nullptr)) _b = {};                                                    \
                                                                                                   \
    map_exist(u, &_a);                                                                             \
  })

#define um_clear(u)                                                                                \
  do {                                                                                             \
    typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    map_clear(u);                                                                                  \
  } while (0)

#define um_cleanup(u)                                                                              \
  do {                                                                                             \
    typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                              \
                                                                                                   \
    map_cleanup(u);                                                                                \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

/* clang-format off */
#  define um_at(u, ...)                                                                            \
    va_elseif(va_size_is(2, __VA_ARGS__)) (                                                        \
      ({                                                                                           \
        static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                     \
                                                                                                   \
        bool _ret                      = false;                                                    \
        auto _a                        = va_at(0, __VA_ARGS__);                                    \
        typeof(***u(u_imap, &_a, nullptr))* _b = {};                                               \
                                                                                                   \
        _b = map_at(u, &_a);                                                                       \
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
        typeof(***u(u_imap, &_a, nullptr)) _it = {};                                               \
        typeof(***u(u_imap, &_a, nullptr))* _b = {};                                               \
                                                                                                   \
        _b = map_at(u, &_a);                                                                       \
                                                                                                   \
        if (_b == nullptr) {                                                                       \
          _b = &_it;                                                                               \
        }                                                                                          \
                                                                                                   \
        *_b;                                                                                       \
      })                                                                                           \
    )
/* clang-format on */

#define um_try(u, k)                                                                               \
  for (typeof(***u(u_imap, &(typeof(k)){k}, nullptr))* it = map_at(u, &(typeof(k)){k});            \
       it != nullptr;                                                                              \
       it = nullptr)

#define um_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                               = va_at(0, __VA_ARGS__);                                 \
    typeof(***u(u_imap, &_a, nullptr)) _b = {};                                                    \
                                                                                                   \
    map_pop(u, &_a, &_b);                                                                          \
                                                                                                   \
    _b;                                                                                            \
  })

#define um_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                               = va_at(0, __VA_ARGS__);                                 \
    typeof(***u(u_imap, &_a, nullptr)) _b = va_at(1, __VA_ARGS__);                                 \
                                                                                                   \
    map_put(u, &_a, &_b);                                                                          \
  } while (0)

#define um_for_all(u, k, v, K)                                                                     \
  for (K k = {}; map_for_init(u, 1); map_for_end(u))                                               \
    for (typeof(***u(u_imap, &k, nullptr)) v = {}; map_for(u, &k, &v);)

#define um_rfor_all(u, k, v, K)                                                                    \
  for (K k = {}; map_for_init(u, 0); map_for_end(u))                                               \
    for (typeof(***u(u_imap, &k, nullptr)) v = {}; map_for(u, &k, &v);)

#endif /* !U_IMAP_H__ */
