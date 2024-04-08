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
#define ulst(T) typeof(u_lst_t(*)(T*))

#define __ulst_def(T)                                                                              \
  ulst(T) : (T*) {                                                                                 \
  }

#define ul_type(u)       typeof(_Generic(typeof(u), va_map(__ulst_def, va_unpack(ulst_def))))
#define ul_type_check(u) static_assert(typeeq((u_lst_t){}, u(nullptr)))

/***************************************************************************************************
 * iApi lst
 **************************************************************************************************/
#define ul_init(u)                                                                                 \
  do {                                                                                             \
    ul_type_check(u);                                                                              \
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
    ul_type_check(u);                                                                              \
                                                                                                   \
    lst_len(u);                                                                                    \
  })

#define ul_is_empty(u)                                                                             \
  ({                                                                                               \
    ul_type_check(u);                                                                              \
                                                                                                   \
    0 == lst_len(u);                                                                               \
  })

#define ul_is_exist(u, ptr)                                                                        \
  ({                                                                                               \
    ul_type_check(u);                                                                              \
                                                                                                   \
    ul_type(u) _a = ptr;                                                                           \
                                                                                                   \
    lst_exist(u, _a);                                                                              \
  })

#define ul_clear(u)                                                                                \
  do {                                                                                             \
    ul_type_check(u);                                                                              \
                                                                                                   \
    lst_clear(u);                                                                                  \
  } while (0)

#define ul_cleanup(u)                                                                              \
  do {                                                                                             \
    ul_type_check(u);                                                                              \
                                                                                                   \
    lst_cleanup(u);                                                                                \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

#define ul_first(u)                                                                                \
  ({                                                                                               \
    ul_type_check(u);                                                                              \
                                                                                                   \
    ul_type(u) _a = {};                                                                            \
                                                                                                   \
    _a = lst_first(u);                                                                             \
                                                                                                   \
    _a;                                                                                            \
  })

#define ul_last(u)                                                                                 \
  ({                                                                                               \
    ul_type_check(u);                                                                              \
                                                                                                   \
    ul_type(u) _a = {};                                                                            \
                                                                                                   \
    _a = lst_last(u);                                                                              \
                                                                                                   \
    _a;                                                                                            \
  })

#define ul_prev(u, ptr)                                                                            \
  ({                                                                                               \
    ul_type_check(u);                                                                              \
                                                                                                   \
    ul_type(u) _a = ptr;                                                                           \
    ul_type(u) _b = {};                                                                            \
                                                                                                   \
    _b = lst_prev(u, _a);                                                                          \
                                                                                                   \
    _b;                                                                                            \
  })

#define ul_next(u, ptr)                                                                            \
  ({                                                                                               \
    ul_type_check(u);                                                                              \
                                                                                                   \
    ul_type(u) _a = ptr;                                                                           \
    ul_type(u) _b = {};                                                                            \
                                                                                                   \
    _b = lst_next(u, _a);                                                                          \
                                                                                                   \
    _b;                                                                                            \
  })

#define ul_pop(u, ...)                                                                             \
  ({                                                                                               \
    va_elseif(va_size_is(1, __VA_ARGS__))(ul_type(u) _a = va_at(0, __VA_ARGS__);)(                 \
        ul_type(u) _a = lst_last(u);)                                                              \
                                                                                                   \
        lst_pop(u, _a);                                                                            \
                                                                                                   \
    _a;                                                                                            \
  })

/* clang-format off */
#define ul_put(u, ptr, ...)                                                                        \
  do {                                                                                             \
    ul_type_check(u);                                                                              \
                                                                                                   \
    va_elseif(va_size_is(1, __VA_ARGS__)) (                                                        \
      ul_type(u) _a = ptr;                                                                         \
      ul_type(u) _b = va_at(0, __VA_ARGS__);                                                       \
    ) (                                                                                            \
      ul_type(u) _a = lst_last(u);                                                                 \
      ul_type(u) _b = ptr;                                                                         \
    )                                                                                              \
                                                                                                   \
    lst_put(u, _a, _b);                                                                            \
  } while (0)
/* clang-format on */

#define ul_for_all(u, it)                                                                          \
  for (; lst_for_init(u, 1); lst_for_end(u))                                                       \
    for (ul_type(u) it = {}; (it = lst_for(u));)

#define ul_rfor_all(u, it)                                                                         \
  for (; lst_for_init(u, 0); lst_for_end(u))                                                       \
    for (ul_type(u) it = {}; (it = lst_for(u));)

#endif /* !U_ILST_H__ */
