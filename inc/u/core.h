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

#ifndef U_CORE_H__
#  define U_CORE_H__

#  include <u/u.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_vec_t;

typedef struct {
}* u_map_t;

typedef struct {
}* u_avl_t;

typedef struct {
}* u_lst_t;

/***************************************************************************************************
 * Let
 **************************************************************************************************/
extern u_vec_t u_ivec;
extern u_map_t u_imap;
extern u_avl_t u_iavl;
extern u_lst_t u_ilst;

/***************************************************************************************************
 * Api
 ***************s***********************************************************************************/
extern any_t vec_new(size_t);
extern any_t map_new(size_t, size_t);
extern any_t avl_new(size_t, size_t, u_cmp_fn);
extern any_t lst_new();

extern size_t vec_len(any_t);
extern size_t map_len(any_t);
extern size_t avl_len(any_t);
extern size_t lst_len(any_t);

extern size_t vec_cap(any_t);

extern bool vec_exist(any_t, size_t);
extern bool map_exist(any_t, any_t);
extern bool avl_exist(any_t, any_t);
extern bool lst_exist(any_t, any_t);

extern void vec_clear(any_t);
extern void map_clear(any_t);
extern void avl_clear(any_t);

extern void vec_cleanup(any_t);
extern void map_cleanup(any_t);
extern void avl_cleanup(any_t);
extern void lst_cleanup(any_t);

extern any_t vec_at(any_t, ssize_t);
extern any_t map_at(any_t, any_t);
extern any_t avl_at(any_t, any_t);

extern any_t lst_first(any_t);
extern any_t lst_last(any_t);
extern any_t lst_next(any_t, any_t);
extern any_t lst_prev(any_t, any_t);

extern void vec_pop(any_t, ssize_t, any_t);
extern void map_pop(any_t, any_t, any_t);
extern void avl_pop(any_t, any_t, any_t);
extern void lst_pop(any_t, any_t);

extern void vec_put(any_t, ssize_t, any_t);
extern void map_put(any_t, any_t, any_t);
extern void avl_put(any_t, any_t, any_t);
extern void lst_put(any_t, any_t, any_t);

extern void vec_sort(any_t, u_cmp_fn);

extern bool vec_for_init(any_t, bool);
extern bool map_for_init(any_t, bool);
extern bool avl_for_init(any_t, bool);
extern bool lst_for_init(any_t, bool);

extern void vec_for_end(any_t);
extern void map_for_end(any_t);
extern void avl_for_end(any_t);
extern void lst_for_end(any_t);

extern bool vec_for(any_t, ssize_t*, any_t);
extern bool map_for(any_t, any_t, any_t);
extern bool avl_for(any_t, any_t, any_t);
extern any_t lst_for(any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define uvec(T)    typeof(T(*(*)(u_vec_t, ssize_t, T*))[sizeof(ssize_t)][sizeof(T)])
#  define umap(K, V) typeof(V(*(*)(u_map_t, K*, V*))[sizeof(K)][sizeof(V)])
#  define uavl(K, V) typeof(V(*(*)(u_avl_t, K*, V*))[sizeof(K)][sizeof(V)])
#  define ulst(T)    typeof(T(*(*)(u_lst_t)))

/***************************************************************************************************
 * iApi vec
 **************************************************************************************************/
#  define uv_init(u)                                                                               \
    do {                                                                                           \
      typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                  \
                                                                                                   \
      u = vec_new(sizeof(typeof(**u(nullptr, 0, nullptr))) /                                       \
                  sizeof(typeof(***u(nullptr, 0, nullptr))));                                      \
    } while (0)

#  define uv_new(T)                                                                                \
    ({                                                                                             \
      uvec(T) u = nullptr;                                                                         \
                                                                                                   \
      uv_init(u);                                                                                  \
                                                                                                   \
      u;                                                                                           \
    })

#  define uv_len(u)                                                                                \
    ({                                                                                             \
      typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                  \
                                                                                                   \
      vec_len(u);                                                                                  \
    })

#  define uv_cap(u)                                                                                \
    ({                                                                                             \
      typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                  \
                                                                                                   \
      vec_cap(u);                                                                                  \
    })

#  define uv_empty(u)                                                                              \
    ({                                                                                             \
      typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                  \
                                                                                                   \
      0 == vec_len(u);                                                                             \
    })

#  define uv_exist(u, ...)                                                                         \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      auto _a                              = va_at(0, __VA_ARGS__);                                \
      typeof(***u(u_ivec, _a, nullptr)) _b = {};                                                   \
                                                                                                   \
      vec_exist(u, _a);                                                                            \
    })

#  define uv_clear(u)                                                                              \
    do {                                                                                           \
      typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                  \
                                                                                                   \
      vec_clear(u);                                                                                \
    } while (0)

#  define uv_cleanup(u)                                                                            \
    do {                                                                                           \
      typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                  \
                                                                                                   \
      vec_cleanup(u);                                                                              \
                                                                                                   \
      u = nullptr;                                                                                 \
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

#  define uv_try(u, ...)                                                                           \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                       \
                                                                                                   \
      bool _ret                             = false;                                               \
      auto _a                               = va_at(0, __VA_ARGS__);                               \
      typeof(***u(u_ivec, _a, nullptr))* it = {};                                                  \
                                                                                                   \
      it = vec_at(u, _a);                                                                          \
                                                                                                   \
      if (it != nullptr) {                                                                         \
        _ret = true;                                                                               \
                                                                                                   \
        va_at(1, __VA_ARGS__)                                                                      \
      }                                                                                            \
                                                                                                   \
      _ret;                                                                                        \
    })

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

#  define uv_sort(u, ...)                                                                          \
    do {                                                                                           \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      typeof(u(u_ivec, 0, nullptr)) _m = nullptr;                                                  \
                                                                                                   \
      vec_sort(u, va_at(0, __VA_ARGS__));                                                          \
    } while (0)

#  define uv_for_all(u, i, it)                                                                     \
    for (ssize_t i = 0; vec_for_init(u, 1); vec_for_end(u))                                        \
      for (typeof(***u(u_ivec, i, nullptr)) it = {}; vec_for(u, &i, &it);)

#  define uv_rfor_all(u, i, it)                                                                    \
    for (ssize_t i = 0; vec_for_init(u, 0); vec_for_end(u))                                        \
      for (typeof(***u(u_ivec, i, nullptr)) it = {}; vec_for(u, &i, &it);)

/***************************************************************************************************
 * iApi map
 **************************************************************************************************/
#  define um_init(u)                                                                               \
    do {                                                                                           \
      typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      u = map_new(sizeof(typeof(*u(nullptr, nullptr, nullptr))) /                                  \
                      sizeof(typeof(**u(nullptr, nullptr, nullptr))),                              \
                  sizeof(typeof(**u(nullptr, nullptr, nullptr))) /                                 \
                      sizeof(typeof(***u(nullptr, nullptr, nullptr))));                            \
    } while (0)

#  define um_new(K, V)                                                                             \
    ({                                                                                             \
      umap(K, V) u = nullptr;                                                                      \
                                                                                                   \
      um_init(u);                                                                                  \
                                                                                                   \
      u;                                                                                           \
    })

#  define um_len(u)                                                                                \
    ({                                                                                             \
      typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      map_len(u);                                                                                  \
    })

#  define um_empty(u)                                                                              \
    ({                                                                                             \
      typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      0 == map_len(u);                                                                             \
    })

#  define um_exist(u, ...)                                                                         \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      auto _a                               = va_at(0, __VA_ARGS__);                               \
      typeof(***u(u_imap, &_a, nullptr)) _b = {};                                                  \
                                                                                                   \
      map_exist(u, &_a);                                                                           \
    })

#  define um_clear(u)                                                                              \
    do {                                                                                           \
      typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      map_clear(u);                                                                                \
    } while (0)

#  define um_cleanup(u)                                                                            \
    do {                                                                                           \
      typeof(u(u_imap, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      map_cleanup(u);                                                                              \
                                                                                                   \
      u = nullptr;                                                                                 \
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

#  define um_try(u, ...)                                                                           \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                       \
                                                                                                   \
      bool _ret                              = false;                                              \
      auto _a                                = va_at(0, __VA_ARGS__);                              \
      typeof(***u(u_imap, &_a, nullptr))* it = {};                                                 \
                                                                                                   \
      it = map_at(u, &_a);                                                                         \
                                                                                                   \
      if (it != nullptr) {                                                                         \
        _ret = true;                                                                               \
                                                                                                   \
        va_at(1, __VA_ARGS__)                                                                      \
      }                                                                                            \
                                                                                                   \
      _ret;                                                                                        \
    })

#  define um_pop(u, ...)                                                                           \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                       \
                                                                                                   \
      auto _a                               = va_at(0, __VA_ARGS__);                               \
      typeof(***u(u_imap, &_a, nullptr)) _b = {};                                                  \
                                                                                                   \
      map_pop(u, &_a, &_b);                                                                        \
                                                                                                   \
      _b;                                                                                          \
    })

#  define um_put(u, ...)                                                                           \
    do {                                                                                           \
      static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                       \
                                                                                                   \
      auto _a                               = va_at(0, __VA_ARGS__);                               \
      typeof(***u(u_imap, &_a, nullptr)) _b = va_at(1, __VA_ARGS__);                               \
                                                                                                   \
      map_put(u, &_a, &_b);                                                                        \
    } while (0)

#  define um_for_all(u, k, v, K)                                                                   \
    for (K k = {}; map_for_init(u, 1); map_for_end(u))                                             \
      for (typeof(***u(u_imap, &k, nullptr)) v = {}; map_for(u, &k, &v);)

#  define um_rfor_all(u, k, v, K)                                                                  \
    for (K k = {}; map_for_init(u, 0); map_for_end(u))                                             \
      for (typeof(***u(u_imap, &k, nullptr)) v = {}; map_for(u, &k, &v);)

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#  define ut_init(u, ...)                                                                          \
    do {                                                                                           \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
      typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      u = avl_new(sizeof(typeof(*u(nullptr, nullptr, nullptr))) /                                  \
                      sizeof(typeof(**u(nullptr, nullptr, nullptr))),                              \
                  sizeof(typeof(**u(nullptr, nullptr, nullptr))) /                                 \
                      sizeof(typeof(***u(nullptr, nullptr, nullptr))),                             \
                  va_at(0, __VA_ARGS__));                                                          \
    } while (0)

#  define ut_new(K, V, ...)                                                                        \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      uavl(K, V) u = nullptr;                                                                      \
                                                                                                   \
      ut_init(u, va_at(0, __VA_ARGS__));                                                           \
                                                                                                   \
      u;                                                                                           \
    })

#  define ut_len(u)                                                                                \
    ({                                                                                             \
      typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      avl_len(u);                                                                                  \
    })

#  define ut_empty(u)                                                                              \
    ({                                                                                             \
      typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      0 == avl_len(u);                                                                             \
    })

#  define ut_exist(u, ...)                                                                         \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      auto _a                               = va_at(0, __VA_ARGS__);                               \
      typeof(***u(u_iavl, &_a, nullptr)) _b = {};                                                  \
                                                                                                   \
      avl_exist(u, &_a);                                                                           \
    })

#  define ut_clear(u)                                                                              \
    do {                                                                                           \
      typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      avl_clear(u);                                                                                \
    } while (0)

#  define ut_cleanup(u)                                                                            \
    do {                                                                                           \
      typeof(u(u_iavl, nullptr, nullptr)) _m = nullptr;                                            \
                                                                                                   \
      avl_cleanup(u);                                                                              \
                                                                                                   \
      u = nullptr;                                                                                 \
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

#  define ut_try(u, ...)                                                                           \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                       \
                                                                                                   \
      bool _ret                              = false;                                              \
      auto _a                                = va_at(0, __VA_ARGS__);                              \
      typeof(***u(u_iavl, &_a, nullptr))* it = {};                                                 \
                                                                                                   \
      it = avl_at(u, &_a);                                                                         \
                                                                                                   \
      if (it != nullptr) {                                                                         \
        _ret = true;                                                                               \
                                                                                                   \
        va_at(1, __VA_ARGS__)                                                                      \
      }                                                                                            \
                                                                                                   \
      _ret;                                                                                        \
    })

#  define ut_pop(u, ...)                                                                           \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                       \
                                                                                                   \
      auto _a                               = va_at(0, __VA_ARGS__);                               \
      typeof(***u(u_iavl, &_a, nullptr)) _b = {};                                                  \
                                                                                                   \
      avl_pop(u, &_a, &_b);                                                                        \
                                                                                                   \
      _b;                                                                                          \
    })

#  define ut_put(u, ...)                                                                           \
    do {                                                                                           \
      static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                       \
                                                                                                   \
      auto _a                               = va_at(0, __VA_ARGS__);                               \
      typeof(***u(u_iavl, &_a, nullptr)) _b = va_at(1, __VA_ARGS__);                               \
                                                                                                   \
      avl_put(u, &_a, &_b);                                                                        \
    } while (0)

#  define ut_for_all(u, k, v, K)                                                                   \
    for (K k = {}; avl_for_init(u, 1); avl_for_end(u))                                             \
      for (typeof(***u(u_iavl, &k, nullptr)) v = {}; avl_for(u, &k, &v);)

#  define ut_rfor_all(u, k, v, K)                                                                  \
    for (K k = {}; avl_for_init(u, 0); avl_for_end(u))                                             \
      for (typeof(***u(u_iavl, &k, nullptr)) v = {}; avl_for(u, &k, &v);)

/***************************************************************************************************
 * iApi lst
 **************************************************************************************************/
#  define ul_init(u)                                                                               \
    do {                                                                                           \
      typeof(u(u_ilst)) _m = nullptr;                                                              \
                                                                                                   \
      u = lst_new();                                                                               \
    } while (0)

#  define ul_new(T)                                                                                \
    ({                                                                                             \
      ulst(T) u = nullptr;                                                                         \
                                                                                                   \
      ul_init(u);                                                                                  \
                                                                                                   \
      u;                                                                                           \
    })

#  define ul_len(u)                                                                                \
    ({                                                                                             \
      typeof(u(u_ilst)) _m = nullptr;                                                              \
                                                                                                   \
      lst_len(u);                                                                                  \
    })

#  define ul_empty(u)                                                                              \
    ({                                                                                             \
      typeof(u(u_ilst)) _m = nullptr;                                                              \
                                                                                                   \
      0 == lst_len(u);                                                                             \
    })

#  define ul_exist(u, ...)                                                                         \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                                \
                                                                                                   \
      lst_exist(u, _a);                                                                            \
    })

#  define ul_clear(u)                                                                              \
    do {                                                                                           \
      typeof(u(u_ilst)) _m = nullptr;                                                              \
                                                                                                   \
      lst_clear(u);                                                                                \
    } while (0)

#  define ul_cleanup(u)                                                                            \
    do {                                                                                           \
      typeof(u(u_ilst)) _m = nullptr;                                                              \
                                                                                                   \
      lst_cleanup(u);                                                                              \
                                                                                                   \
      u = nullptr;                                                                                 \
    } while (0)

#  define ul_first(u)                                                                              \
    ({                                                                                             \
      typeof(u(u_ilst)) _a = {};                                                                   \
                                                                                                   \
      _a = lst_first(u);                                                                           \
                                                                                                   \
      _a;                                                                                          \
    })

#  define ul_last(u)                                                                               \
    ({                                                                                             \
      typeof(u(u_ilst)) _a = {};                                                                   \
                                                                                                   \
      _a = lst_last(u);                                                                            \
                                                                                                   \
      _a;                                                                                          \
    })

#  define ul_prev(u, ...)                                                                          \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                       \
                                                                                                   \
      typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                                \
      typeof(u(u_ilst)) _b = {};                                                                   \
                                                                                                   \
      _b = lst_prev(u, _a);                                                                        \
                                                                                                   \
      _b;                                                                                          \
    })

#  define ul_next(u, ...)                                                                          \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                       \
                                                                                                   \
      typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                                \
      typeof(u(u_ilst)) _b = {};                                                                   \
                                                                                                   \
      _b = lst_next(u, _a);                                                                        \
                                                                                                   \
      _b;                                                                                          \
    })

#  define ul_pop(u, ...)                                                                           \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                       \
                                                                                                   \
      typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                                \
                                                                                                   \
      lst_pop(u, _a);                                                                              \
                                                                                                   \
      _a;                                                                                          \
    })

/* clang-format off */
#  define ul_put(u, ...)                                                                           \
    do {                                                                                           \
      static_assert((va_size(__VA_ARGS__) == 1) + (va_size(__VA_ARGS__) == 2),                     \
                    "The number of '...' is 1 or 2.");                                             \
                                                                                                   \
      va_elseif(va_size_is(2, __VA_ARGS__)) (                                                      \
        typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                              \
        typeof(u(u_ilst)) _b = va_at(1, __VA_ARGS__);                                              \
      ) (                                                                                          \
        typeof(u(u_ilst)) _a = lst_last(u);                                                        \
        typeof(u(u_ilst)) _b = va_at(0, __VA_ARGS__);                                              \
      )                                                                                            \
                                                                                                   \
      lst_put(u, _a, _b);                                                                          \
    } while (0)
/* clang-format on */

#  define ul_for_all(u, it)                                                                        \
    for (; lst_for_init(u, 1); lst_for_end(u))                                                     \
      for (typeof(u(u_ilst)) it = {}; (it = lst_for(u));)

#  define ul_rfor_all(u, it)                                                                       \
    for (; lst_for_init(u, 0); lst_for_end(u))                                                     \
      for (typeof(u(u_ilst)) it = {}; (it = lst_for(u));)

#endif /* !U_CORE_H__ */
