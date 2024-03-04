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

#include <u/u.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_vec_t;

typedef struct {
}* u_tbl_t;

typedef struct {
}* u_avl_t;

typedef struct {
}* u_lst_t;

/***************************************************************************************************
 * Api
 ***************s***********************************************************************************/
extern u_vec_t vec_new(size_t);
extern u_tbl_t tbl_new(size_t, size_t);
extern u_avl_t avl_new(size_t, size_t, u_cmp_fn);
extern u_lst_t lst_new();

extern size_t vec_len(u_vec_t);
extern size_t tbl_len(u_tbl_t);
extern size_t avl_len(u_avl_t);
extern size_t lst_len(u_lst_t);

extern size_t vec_cap(u_vec_t);

extern bool vec_exist(u_vec_t, size_t);
extern bool tbl_exist(u_tbl_t, any_t);
extern bool avl_exist(u_avl_t, any_t);
extern bool lst_exist(u_lst_t, any_t);

extern void vec_clear(u_vec_t);
extern void tbl_clear(u_tbl_t);
extern void avl_clear(u_avl_t);

extern void vec_cleanup(u_vec_t);
extern void tbl_cleanup(u_tbl_t);
extern void avl_cleanup(u_avl_t);
extern void lst_cleanup(u_lst_t);

extern any_t vec_at(u_vec_t, ssize_t);
extern any_t tbl_at(u_tbl_t, any_t);
extern any_t avl_at(u_avl_t, any_t);

extern any_t lst_first(u_lst_t);
extern any_t lst_last(u_lst_t);
extern any_t lst_next(u_lst_t, any_t);
extern any_t lst_prev(u_lst_t, any_t);

extern void vec_pop(u_vec_t, ssize_t, any_t);
extern void tbl_pop(u_tbl_t, any_t, any_t);
extern void avl_pop(u_avl_t, any_t, any_t);
extern void lst_pop(u_lst_t, any_t);

extern void vec_put(u_vec_t, ssize_t, any_t);
extern void tbl_put(u_tbl_t, any_t, any_t);
extern void avl_put(u_avl_t, any_t, any_t);
extern void lst_put(u_lst_t, any_t, any_t);

extern void vec_sort(u_vec_t, u_cmp_fn);

extern bool vec_each_init(u_vec_t, bool);
extern bool tbl_each_init(u_tbl_t, bool);
extern bool avl_each_init(u_avl_t, bool);
extern bool lst_each_init(u_lst_t, bool);

extern bool vec_each(u_vec_t, ssize_t*, any_t);
extern bool tbl_each(u_tbl_t, any_t, any_t);
extern bool avl_each(u_avl_t, any_t, any_t);
extern any_t lst_each(u_lst_t _self);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define uvec(T)    typeof(T(*(*)(u_vec_t, ssize_t, T*))[sizeof(ssize_t)][sizeof(T)])
#define utbl(K, V) typeof(V(*(*)(u_tbl_t, K*, V*))[sizeof(K)][sizeof(V)])
#define uavl(K, V) typeof(V(*(*)(u_avl_t, K*, V*))[sizeof(K)][sizeof(V)])
#define ulst(T)    typeof(T(*(*)(u_lst_t)))

/***************************************************************************************************
 * iApi vec
 **************************************************************************************************/
#define uv_init(u)                                                                                 \
  do {                                                                                             \
    typeof(u((u_vec_t){nullptr}, 0, nullptr)) _m = nullptr;                                        \
                                                                                                   \
    u = any(vec_new(sizeof(typeof(**u(nullptr, 0, nullptr))) /                                     \
                    sizeof(typeof(***u(nullptr, 0, nullptr)))));                                   \
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
    typeof(u((u_vec_t){nullptr}, 0, nullptr)) _m = nullptr;                                        \
                                                                                                   \
    vec_len((u_vec_t)u);                                                                           \
  })

#define uv_cap(u)                                                                                  \
  ({                                                                                               \
    typeof(u((u_vec_t){nullptr}, 0, nullptr)) _m = nullptr;                                        \
                                                                                                   \
    vec_cap((u_vec_t)u);                                                                           \
  })

#define uv_empty(u)                                                                                \
  ({                                                                                               \
    typeof(u((u_vec_t){nullptr}, 0, nullptr)) _m = nullptr;                                        \
                                                                                                   \
    0 == vec_len((u_vec_t)u);                                                                      \
  })

#define uv_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                                          = va_at(0, __VA_ARGS__);                      \
    typeof(***u((u_vec_t){nullptr}, _a, nullptr)) _b = {};                                         \
                                                                                                   \
    vec_exist((u_vec_t)u, _a);                                                                     \
  })

#define uv_clear(u)                                                                                \
  do {                                                                                             \
    typeof(u((u_vec_t){nullptr}, 0, nullptr)) _m = nullptr;                                        \
                                                                                                   \
    vec_clear((u_vec_t)u);                                                                         \
  } while (0)

#define uv_cleanup(u)                                                                              \
  do {                                                                                             \
    typeof(u((u_vec_t){nullptr}, 0, nullptr)) _m = nullptr;                                        \
                                                                                                   \
    vec_cleanup((u_vec_t)u);                                                                       \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

/* clang-format off */
#define uv_at(u, ...)                                                                              \
  va_elseif(va_size_is(1, __VA_ARGS__)) (                                                          \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                       \
                                                                                                   \
      bool _ret                      = false;                                                      \
      auto _a                        = va_at(0, __VA_ARGS__);                                      \
      typeof(***u((u_vec_t){nullptr}, _a, nullptr))* _b = {};                                      \
                                                                                                   \
      _b = vec_at((u_vec_t)u, _a);                                                             \
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
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      auto _a                        = va_at(0, __VA_ARGS__);                                      \
      typeof(***u((u_vec_t){nullptr}, _a, nullptr)) _it = {};                                      \
      typeof(***u((u_vec_t){nullptr}, _a, nullptr))* _b = {};                                      \
                                                                                                   \
      _b = vec_at((u_vec_t)u, _a);                                                             \
                                                                                                   \
      if (_b == nullptr) {                                                                         \
        _b = &_it;                                                                                 \
      }                                                                                            \
                                                                                                   \
      *_b;                                                                                         \
    })                                                                                             \
  )
/* clang-format on */

#define uv_try(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    bool _ret                                         = false;                                     \
    auto _a                                           = va_at(0, __VA_ARGS__);                     \
    typeof(***u((u_vec_t){nullptr}, _a, nullptr))* it = {};                                        \
                                                                                                   \
    it = vec_at((u_vec_t)u, _a);                                                                   \
                                                                                                   \
    if (it != nullptr) {                                                                           \
      _ret = true;                                                                                 \
                                                                                                   \
      va_at(1, __VA_ARGS__)                                                                        \
    }                                                                                              \
                                                                                                   \
    _ret;                                                                                          \
  })

#define uv_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                                          = va_at(0, __VA_ARGS__);                      \
    typeof(***u((u_vec_t){nullptr}, _a, nullptr)) _b = {};                                         \
                                                                                                   \
    vec_pop((u_vec_t)u, _a, &_b);                                                                  \
                                                                                                   \
    _b;                                                                                            \
  })

#define uv_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                                          = va_at(0, __VA_ARGS__);                      \
    typeof(***u((u_vec_t){nullptr}, _a, nullptr)) _b = va_at(1, __VA_ARGS__);                      \
                                                                                                   \
    vec_put((u_vec_t)u, _a, &_b);                                                                  \
  } while (0)

#define uv_sort(u, ...)                                                                            \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    typeof(u((u_vec_t){nullptr}, 0, nullptr)) _m = nullptr;                                        \
                                                                                                   \
    vec_sort((u_vec_t)u, va_at(0, __VA_ARGS__));                                                   \
  } while (0)

#define uv_for_all(u, i, it)                                                                       \
  for (ssize_t i = 0; vec_each_init((u_vec_t)u, 1);)                                               \
    for (typeof(***u((u_vec_t){nullptr}, i, nullptr)) it = {}; vec_each((u_vec_t)u, &i, &it);)

#define uv_rfor_all(u, i, it)                                                                      \
  for (ssize_t i = 0; vec_each_init((u_vec_t)u, 0);)                                               \
    for (typeof(***u((u_vec_t){nullptr}, i, nullptr)) it = {}; vec_each((u_vec_t)u, &i, &it);)

/***************************************************************************************************
 * iApi tbl
 **************************************************************************************************/
#define ut_init(u)                                                                                 \
  do {                                                                                             \
    typeof(u((u_tbl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    u = any(tbl_new(sizeof(typeof(*u(nullptr, nullptr, nullptr))) /                                \
                        sizeof(typeof(**u(nullptr, nullptr, nullptr))),                            \
                    sizeof(typeof(**u(nullptr, nullptr, nullptr))) /                               \
                        sizeof(typeof(***u(nullptr, nullptr, nullptr)))));                         \
  } while (0)

#define ut_new(K, V)                                                                               \
  ({                                                                                               \
    utbl(K, V) u = nullptr;                                                                        \
                                                                                                   \
    ut_init(u);                                                                                    \
                                                                                                   \
    u;                                                                                             \
  })

#define ut_len(u)                                                                                  \
  ({                                                                                               \
    typeof(u((u_tbl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    tbl_len((u_tbl_t)u);                                                                           \
  })

#define ut_empty(u)                                                                                \
  ({                                                                                               \
    typeof(u((u_tbl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    0 == tbl_len((u_tbl_t)u);                                                                      \
  })

#define ut_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                                           = va_at(0, __VA_ARGS__);                     \
    typeof(***u((u_tbl_t){nullptr}, &_a, nullptr)) _b = {};                                        \
                                                                                                   \
    tbl_exist((u_tbl_t)u, &_a);                                                                    \
  })

#define ut_clear(u)                                                                                \
  do {                                                                                             \
    typeof(u((u_tbl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    tbl_clear((u_tbl_t)u);                                                                         \
  } while (0)

#define ut_cleanup(u)                                                                              \
  do {                                                                                             \
    typeof(u((u_tbl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    tbl_cleanup((u_tbl_t)u);                                                                       \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

/* clang-format off */
#define ut_at(u, ...)                                                                              \
  va_elseif(va_size_is(1, __VA_ARGS__)) (                                                          \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                       \
                                                                                                   \
      bool _ret                      = false;                                                      \
      auto _a                        = va_at(0, __VA_ARGS__);                                      \
      typeof(***u((u_tbl_t){nullptr}, &_a, nullptr))* _b = {};                                     \
                                                                                                   \
      _b = tbl_at((u_tbl_t)u, &_a);                                                            \
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
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      auto _a                        = va_at(0, __VA_ARGS__);                                      \
      typeof(***u((u_tbl_t){nullptr}, &_a, nullptr)) _it = {};                                     \
      typeof(***u((u_tbl_t){nullptr}, &_a, nullptr))* _b = {};                                     \
                                                                                                   \
      _b = tbl_at((u_tbl_t)u, &_a);                                                            \
                                                                                                   \
      if (_b == nullptr) {                                                                         \
        _b = &_it;                                                                                 \
      }                                                                                            \
                                                                                                   \
      *_b;                                                                                         \
    })                                                                                             \
  )
/* clang-format on */

#define ut_try(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    bool _ret                                          = false;                                    \
    auto _a                                            = va_at(0, __VA_ARGS__);                    \
    typeof(***u((u_tbl_t){nullptr}, &_a, nullptr))* it = {};                                       \
                                                                                                   \
    it = tbl_at((u_tbl_t)u, &_a);                                                                  \
                                                                                                   \
    if (it != nullptr) {                                                                           \
      _ret = true;                                                                                 \
                                                                                                   \
      va_at(1, __VA_ARGS__)                                                                        \
    }                                                                                              \
                                                                                                   \
    _ret;                                                                                          \
  })

#define ut_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                                           = va_at(0, __VA_ARGS__);                     \
    typeof(***u((u_tbl_t){nullptr}, &_a, nullptr)) _b = {};                                        \
                                                                                                   \
    tbl_pop((u_tbl_t)u, &_a, &_b);                                                                 \
                                                                                                   \
    _b;                                                                                            \
  })

#define ut_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                                           = va_at(0, __VA_ARGS__);                     \
    typeof(***u((u_tbl_t){nullptr}, &_a, nullptr)) _b = va_at(1, __VA_ARGS__);                     \
                                                                                                   \
    tbl_put((u_tbl_t)u, &_a, &_b);                                                                 \
  } while (0)

#define ut_for_all(u, k, v, K)                                                                     \
  for (K k = {}; tbl_each_init((u_tbl_t)u, 1);)                                                    \
    for (typeof(***u((u_tbl_t){nullptr}, k, nullptr)) v = {}; tbl_each((u_tbl_t)u, &k, &v);)

#define ut_rfor_all(u, k, v, K)                                                                    \
  for (K k = {}; tbl_each_init((u_tbl_t)u, 0);)                                                    \
    for (typeof(***u((u_tbl_t){nullptr}, k, nullptr)) v = {}; tbl_each((u_tbl_t)u, &k, &v);)

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#define ua_init(u, ...)                                                                            \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
    typeof(u((u_avl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    u = any(avl_new(sizeof(typeof(*u(nullptr, nullptr, nullptr))) /                                \
                        sizeof(typeof(**u(nullptr, nullptr, nullptr))),                            \
                    sizeof(typeof(**u(nullptr, nullptr, nullptr))) /                               \
                        sizeof(typeof(***u(nullptr, nullptr, nullptr))),                           \
                    va_at(0, __VA_ARGS__)));                                                       \
  } while (0)

#define ua_new(K, V, ...)                                                                          \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    uavl(K, V) u = nullptr;                                                                        \
                                                                                                   \
    ua_init(u, va_at(0, __VA_ARGS__));                                                             \
                                                                                                   \
    u;                                                                                             \
  })

#define ua_len(u)                                                                                  \
  ({                                                                                               \
    typeof(u((u_avl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    avl_len((u_avl_t)u);                                                                           \
  })

#define ua_empty(u)                                                                                \
  ({                                                                                               \
    typeof(u((u_avl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    0 == avl_len((u_avl_t)u);                                                                      \
  })

#define ua_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                                           = va_at(0, __VA_ARGS__);                     \
    typeof(***u((u_avl_t){nullptr}, &_a, nullptr)) _b = {};                                        \
                                                                                                   \
    avl_exist((u_avl_t)u, &_a);                                                                    \
  })

#define ua_clear(u)                                                                                \
  do {                                                                                             \
    typeof(u((u_avl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    avl_clear((u_avl_t)u);                                                                         \
  } while (0)

#define ua_cleanup(u)                                                                              \
  do {                                                                                             \
    typeof(u((u_avl_t){nullptr}, nullptr, nullptr)) _m = nullptr;                                  \
                                                                                                   \
    avl_cleanup((u_avl_t)u);                                                                       \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

/* clang-format off */
#define ua_at(u, ...)                                                                              \
  va_elseif(va_size_is(1, __VA_ARGS__)) (                                                          \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                       \
                                                                                                   \
      bool _ret                      = false;                                                      \
      auto _a                        = va_at(0, __VA_ARGS__);                                      \
      typeof(***u((u_avl_t){nullptr}, &_a, nullptr))* _b = {};                                     \
                                                                                                   \
      _b = avl_at((u_avl_t)u, &_a);                                                            \
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
      static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                       \
                                                                                                   \
      auto _a                        = va_at(0, __VA_ARGS__);                                      \
      typeof(***u((u_avl_t){nullptr}, &_a, nullptr)) _it = {};                                     \
      typeof(***u((u_avl_t){nullptr}, &_a, nullptr))* _b = {};                                     \
                                                                                                   \
      _b = avl_at((u_avl_t)u, &_a);                                                            \
                                                                                                   \
      if (_b == nullptr) {                                                                         \
        _b = &_it;                                                                                 \
      }                                                                                            \
                                                                                                   \
      *_b;                                                                                         \
    })                                                                                             \
  )
/* clang-format on */

#define ua_try(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    bool _ret                                          = false;                                    \
    auto _a                                            = va_at(0, __VA_ARGS__);                    \
    typeof(***u((u_avl_t){nullptr}, &_a, nullptr))* it = {};                                       \
                                                                                                   \
    it = avl_at((u_avl_t)u, &_a);                                                                  \
                                                                                                   \
    if (it != nullptr) {                                                                           \
      _ret = true;                                                                                 \
                                                                                                   \
      va_at(1, __VA_ARGS__)                                                                        \
    }                                                                                              \
                                                                                                   \
    _ret;                                                                                          \
  })

#define ua_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                                           = va_at(0, __VA_ARGS__);                     \
    typeof(***u((u_avl_t){nullptr}, &_a, nullptr)) _b = {};                                        \
                                                                                                   \
    avl_pop((u_avl_t)u, &_a, &_b);                                                                 \
                                                                                                   \
    _b;                                                                                            \
  })

#define ua_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                                           = va_at(0, __VA_ARGS__);                     \
    typeof(***u((u_avl_t){nullptr}, &_a, nullptr)) _b = va_at(1, __VA_ARGS__);                     \
                                                                                                   \
    avl_put((u_avl_t)u, &_a, &_b);                                                                 \
  } while (0)

#define ua_for_all(u, k, v, K)                                                                     \
  for (K k = {}; avl_each_init((u_avl_t)u, 1);)                                                    \
    for (typeof(***u((u_avl_t){nullptr}, k, nullptr)) v = {}; avl_each((u_avl_t)u, &k, &v);)

#define ua_rfor_all(u, k, v, K)                                                                    \
  for (K k = {}; avl_each_init((u_avl_t)u, 0);)                                                    \
    for (typeof(***u((u_avl_t){nullptr}, k, nullptr)) v = {}; avl_each((u_avl_t)u, &k, &v);)

/***************************************************************************************************
 * iApi lst
 **************************************************************************************************/
#define ul_init(u)                                                                                 \
  do {                                                                                             \
    typeof(u((u_lst_t){nullptr})) _m = nullptr;                                                    \
                                                                                                   \
    u = any(lst_new());                                                                            \
  } while (0)

#define ul_new(T)                                                                                  \
  ({                                                                                               \
    ulst(T) u = nullptr;                                                                           \
                                                                                                   \
    ul_init(u);                                                                                    \
                                                                                                   \
    u;                                                                                             \
  })

#define ul_len(u)                                                                                  \
  ({                                                                                               \
    typeof(u((u_lst_t){nullptr})) _m = nullptr;                                                    \
                                                                                                   \
    lst_len((u_lst_t)u);                                                                           \
  })

#define ul_empty(u)                                                                                \
  ({                                                                                               \
    typeof(u((u_lst_t){nullptr})) _m = nullptr;                                                    \
                                                                                                   \
    0 == lst_len((u_lst_t)u);                                                                      \
  })

#define ul_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    typeof(u((u_lst_t){nullptr})) _a = va_at(0, __VA_ARGS__);                                      \
                                                                                                   \
    lst_exist((u_lst_t)u, _a);                                                                     \
  })

#define ul_clear(u)                                                                                \
  do {                                                                                             \
    typeof(u((u_lst_t){nullptr})) _m = nullptr;                                                    \
                                                                                                   \
    lst_clear((u_lst_t)u);                                                                         \
  } while (0)

#define ul_cleanup(u)                                                                              \
  do {                                                                                             \
    typeof(u((u_lst_t){nullptr})) _m = nullptr;                                                    \
                                                                                                   \
    lst_cleanup((u_lst_t)u);                                                                       \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

#define ul_first(u)                                                                                \
  ({                                                                                               \
    typeof(u((u_lst_t){nullptr})) _a = {};                                                         \
                                                                                                   \
    _a = lst_first((u_lst_t)u);                                                                    \
                                                                                                   \
    _a;                                                                                            \
  })

#define ul_last(u)                                                                                 \
  ({                                                                                               \
    typeof(u((u_lst_t){nullptr})) _a = {};                                                         \
                                                                                                   \
    _a = lst_last((u_lst_t)u);                                                                     \
                                                                                                   \
    _a;                                                                                            \
  })

#define ul_prev(u, ...)                                                                            \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    typeof(u((u_lst_t){nullptr})) _a = va_at(0, __VA_ARGS__);                                      \
    typeof(u((u_lst_t){nullptr})) _b = {};                                                         \
                                                                                                   \
    _b = lst_prev((u_lst_t)u, _a);                                                                 \
                                                                                                   \
    _b;                                                                                            \
  })

#define ul_next(u, ...)                                                                            \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    typeof(u((u_lst_t){nullptr})) _a = va_at(0, __VA_ARGS__);                                      \
    typeof(u((u_lst_t){nullptr})) _b = {};                                                         \
                                                                                                   \
    _b = lst_next((u_lst_t)u, _a);                                                                 \
                                                                                                   \
    _b;                                                                                            \
  })

#define ul_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    typeof(u((u_lst_t){nullptr})) _a = va_at(0, __VA_ARGS__);                                      \
                                                                                                   \
    lst_pop((u_lst_t)u, _a);                                                                       \
                                                                                                   \
    _a;                                                                                            \
  })

#define ul_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    typeof(u((u_lst_t){nullptr})) _a = va_at(0, __VA_ARGS__);                                      \
    typeof(u((u_lst_t){nullptr})) _b = va_at(1, __VA_ARGS__);                                      \
                                                                                                   \
    lst_put((u_lst_t)u, _a, _b);                                                                   \
  } while (0)

#define ul_for_all(u, it)                                                                          \
  for (; lst_each_init((u_lst_t)u, 1);)                                                            \
    for (typeof(u((u_lst_t){nullptr})) it = {}; (it = lst_each((u_lst_t)u));)

#define ul_rfor_all(u, it)                                                                         \
  for (; lst_each_init((u_lst_t)u, 0);)                                                            \
    for (typeof(u((u_lst_t){nullptr})) it = {}; (it = lst_each((u_lst_t)u));)
