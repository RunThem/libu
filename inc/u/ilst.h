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

#ifndef U_ILST_H__
#  define U_ILST_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* __u_list_t;

/***************************************************************************************************
 * Api
 ***************************************************************************************************/
extern any_t lst_new();

extern void lst_cleanup(any_t);

extern size_t lst_len(any_t);

extern bool lst_exist(any_t, any_t);

extern any_t lst_head(any_t);

extern any_t lst_tail(any_t);

extern any_t lst_prev(any_t, any_t);

extern any_t lst_next(any_t, any_t);

extern void lst_pop(any_t, any_t);

extern void lst_put(any_t, any_t, any_t);

extern bool lst_for_init(any_t, bool);

extern void lst_for_end(any_t);

extern any_t lst_for(any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_list_t(...) typeof(__u_list_t(*)(__VA_ARGS__*))

#  define _u_list_defs(T)                                                                          \
    u_list_t(T*) : (T*) {                                                                          \
    }

#  define u_list_type(u) typeof(_Generic(u, u_list_defs))

#  if defined(NDEBUG)
#    define u_list_type_check(u)
#  else
#    define u_list_type_check(u) static_assert(typeeq((__u_list_t){}, u(nullptr)))
#  endif

/***************************************************************************************************
 * iApi lst
 **************************************************************************************************/
#  define u_list_init(u)                                                                           \
    do {                                                                                           \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      u = lst_new();                                                                               \
    } while (0)

#  define u_list_new(...)                                                                          \
    ({                                                                                             \
      u_list_t(__VA_ARGS__) u = nullptr;                                                           \
                                                                                                   \
      u_list_init(u);                                                                              \
                                                                                                   \
      u;                                                                                           \
    })

#  define u_list_len(u)                                                                            \
    ({                                                                                             \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      lst_len(u);                                                                                  \
    })

#  define u_list_is_empty(u)                                                                       \
    ({                                                                                             \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      0 == lst_len(u);                                                                             \
    })

#  define u_list_is_exist(u, ptr)                                                                  \
    ({                                                                                             \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      u_list_type(u) _a = ptr;                                                                     \
                                                                                                   \
      lst_exist(u, _a);                                                                            \
    })

#  define u_list_cleanup(u)                                                                        \
    do {                                                                                           \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      lst_cleanup(u);                                                                              \
                                                                                                   \
      u = nullptr;                                                                                 \
    } while (0)

#  define u_list_head(u)                                                                           \
    ({                                                                                             \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      u_list_type(u) _a = {};                                                                      \
                                                                                                   \
      _a = lst_head(u);                                                                            \
                                                                                                   \
      _a;                                                                                          \
    })

#  define u_list_tail(u)                                                                           \
    ({                                                                                             \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      u_list_type(u) _a = {};                                                                      \
                                                                                                   \
      _a = lst_tail(u);                                                                            \
                                                                                                   \
      _a;                                                                                          \
    })

#  define u_list_prev(u, ptr)                                                                      \
    ({                                                                                             \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      u_list_type(u) _a = ptr;                                                                     \
      u_list_type(u) _b = {};                                                                      \
                                                                                                   \
      _b = lst_prev(u, _a);                                                                        \
                                                                                                   \
      _b;                                                                                          \
    })

#  define u_list_next(u, ptr)                                                                      \
    ({                                                                                             \
      u_list_type_check(u);                                                                        \
                                                                                                   \
      u_list_type(u) _a = ptr;                                                                     \
      u_list_type(u) _b = {};                                                                      \
                                                                                                   \
      _b = lst_next(u, _a);                                                                        \
                                                                                                   \
      _b;                                                                                          \
    })

/* clang-format off */
#define u_list_pop(u, ...)                                                                         \
  ({                                                                                               \
    va_elseif(va_cnt_is(1, __VA_ARGS__)) (                                                         \
      u_list_type(u) _a = va_at(0, __VA_ARGS__);                                                   \
    ) (                                                                                            \
      u_list_type(u) _a = lst_head(u);                                                             \
    )                                                                                              \
                                                                                                   \
    lst_pop(u, _a);                                                                                \
                                                                                                   \
    _a;                                                                                            \
  })                                                                                               \

#define u_list_put(u, ptr, ...)                                                                    \
  do {                                                                                             \
    u_list_type_check(u);                                                                          \
                                                                                                   \
    va_elseif(va_cnt_is(1, __VA_ARGS__)) (                                                         \
      u_list_type(u) _a = ptr;                                                                     \
      u_list_type(u) _b = va_at(0, __VA_ARGS__);                                                   \
    ) (                                                                                            \
      u_list_type(u) _a = lst_tail(u);                                                             \
      u_list_type(u) _b = ptr;                                                                     \
    )                                                                                              \
                                                                                                   \
    lst_put(u, _a, _b);                                                                            \
  } while (0)
/* clang-format on */

#  define u_list_for_all(u, it)                                                                    \
    for (; lst_for_init(u, 1); lst_for_end(u))                                                     \
      for (u_list_type(u) it = {}; (it = lst_for(u));)

#  define u_list_rfor_all(u, it)                                                                   \
    for (; lst_for_init(u, 0); lst_for_end(u))                                                     \
      for (u_list_type(u) it = {}; (it = lst_for(u));)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ILST_H__ */
