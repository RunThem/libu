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

#ifndef U_ILST_H__
#define U_ILST_H__

#include "utils/type.h"
#include "utils/va.h"

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_lst_t;

/***************************************************************************************************
 * Let
 **************************************************************************************************/
extern u_lst_t u_ilst;

/***************************************************************************************************
 * Api
 ***************s***********************************************************************************/
extern any_t lst_new();

extern size_t lst_len(any_t);

extern bool lst_exist(any_t, any_t);

extern void lst_cleanup(any_t);

extern any_t lst_first(any_t);

extern any_t lst_last(any_t);

extern any_t lst_next(any_t, any_t);

extern any_t lst_prev(any_t, any_t);

extern void lst_pop(any_t, any_t);

extern void lst_put(any_t, any_t, any_t);

extern bool lst_for_init(any_t, bool);

extern void lst_for_end(any_t);

extern any_t lst_for(any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define ulst(T) typeof(T(*(*)(u_lst_t)))

/***************************************************************************************************
 * iApi lst
 **************************************************************************************************/
#define ul_init(u)                                                                                 \
  do {                                                                                             \
    typeof(u(u_ilst)) _m = nullptr;                                                                \
                                                                                                   \
    u = lst_new();                                                                                 \
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
    typeof(u(u_ilst)) _m = nullptr;                                                                \
                                                                                                   \
    lst_len(u);                                                                                    \
  })

#define ul_empty(u)                                                                                \
  ({                                                                                               \
    typeof(u(u_ilst)) _m = nullptr;                                                                \
                                                                                                   \
    0 == lst_len(u);                                                                               \
  })

#define ul_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                                  \
                                                                                                   \
    lst_exist(u, _a);                                                                              \
  })

#define ul_clear(u)                                                                                \
  do {                                                                                             \
    typeof(u(u_ilst)) _m = nullptr;                                                                \
                                                                                                   \
    lst_clear(u);                                                                                  \
  } while (0)

#define ul_cleanup(u)                                                                              \
  do {                                                                                             \
    typeof(u(u_ilst)) _m = nullptr;                                                                \
                                                                                                   \
    lst_cleanup(u);                                                                                \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

#define ul_first(u)                                                                                \
  ({                                                                                               \
    typeof(u(u_ilst)) _a = {};                                                                     \
                                                                                                   \
    _a = lst_first(u);                                                                             \
                                                                                                   \
    _a;                                                                                            \
  })

#define ul_last(u)                                                                                 \
  ({                                                                                               \
    typeof(u(u_ilst)) _a = {};                                                                     \
                                                                                                   \
    _a = lst_last(u);                                                                              \
                                                                                                   \
    _a;                                                                                            \
  })

#define ul_prev(u, ...)                                                                            \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                                  \
    typeof(u(u_ilst)) _b = {};                                                                     \
                                                                                                   \
    _b = lst_prev(u, _a);                                                                          \
                                                                                                   \
    _b;                                                                                            \
  })

#define ul_next(u, ...)                                                                            \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                                  \
    typeof(u(u_ilst)) _b = {};                                                                     \
                                                                                                   \
    _b = lst_next(u, _a);                                                                          \
                                                                                                   \
    _b;                                                                                            \
  })

#define ul_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    typeof(u(u_ilst)) _a = va_at(0, __VA_ARGS__);                                                  \
                                                                                                   \
    lst_pop(u, _a);                                                                                \
                                                                                                   \
    _a;                                                                                            \
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

#define ul_for_all(u, it)                                                                          \
  for (; lst_for_init(u, 1); lst_for_end(u))                                                       \
    for (typeof(u(u_ilst)) it = {}; (it = lst_for(u));)

#define ul_rfor_all(u, it)                                                                         \
  for (; lst_for_init(u, 0); lst_for_end(u))                                                       \
    for (typeof(u(u_ilst)) it = {}; (it = lst_for(u));)

#endif /* !U_ILST_H__ */
