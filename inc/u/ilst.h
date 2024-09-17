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
}* __u_list_ref_t;

typedef struct u_node_t u_node_t, *u_node_ref_t;
struct u_node_t {
  u_node_ref_t prev;
  u_node_ref_t next;
  any_t ptr;
};

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern any_t  lst_new      (i64_t);
extern bool   lst_clear    (any_t, any_t);
extern void   lst_cleanup  (any_t);
extern i64_t  lst_len      (any_t);
extern bool   lst_is_exist (any_t, any_t);
extern any_t  lst_head     (any_t);
extern any_t  lst_tail     (any_t);
extern any_t  lst_prev     (any_t, any_t);
extern any_t  lst_next     (any_t, any_t);
extern void   lst_pop      (any_t, any_t);
extern void   lst_put      (any_t, any_t, any_t);
extern bool   lst_for      (any_t, bool*, any_t*, u_order_e);
/* clang-format on */

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_list_t(T) typeof(__u_list_ref_t(*)(T*))

/***************************************************************************************************
 * iApi lst
 **************************************************************************************************/
#  define u_list_init(self, T, filed)                                                              \
    do {                                                                                           \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      self = lst_new(offsetof(T, filed));                                                          \
    } while (0)

#  define u_list_new(T, filed)                                                                     \
    ({                                                                                             \
      u_list_t(T) self = nullptr;                                                                  \
                                                                                                   \
      self = lst_new(offsetof(T, filed));                                                          \
                                                                                                   \
      self;                                                                                        \
    })

#  define u_list_len(self)                                                                         \
    ({                                                                                             \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      lst_len(self);                                                                               \
    })

#  define u_list_is_empty(self)                                                                    \
    ({                                                                                             \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      0 == lst_len(self);                                                                          \
    })

#  define u_list_is_exist(self, ptr)                                                               \
    ({                                                                                             \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0)* __a = ptr;                                                                 \
                                                                                                   \
      lst_is_exist(self, __a);                                                                     \
    })

#  define u_list_clear(self, it)                                                                   \
    for (u_types(self, 0)* it = lst_head(self); lst_clear(self, it); it = lst_head(self))

#  define u_list_cleanup(self)                                                                     \
    do {                                                                                           \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      lst_cleanup(self);                                                                           \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

#  define u_list_head(self)                                                                        \
    ({                                                                                             \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0)* __a = {};                                                                  \
                                                                                                   \
      __a = lst_head(self);                                                                        \
                                                                                                   \
      __a;                                                                                         \
    })

#  define u_list_tail(self)                                                                        \
    ({                                                                                             \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0)* __a = {};                                                                  \
                                                                                                   \
      __a = lst_tail(self);                                                                        \
                                                                                                   \
      __a;                                                                                         \
    })

#  define u_list_prev(self, ptr)                                                                   \
    ({                                                                                             \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0)* __a = ptr;                                                                 \
      u_types(self, 0)* __b = {};                                                                  \
                                                                                                   \
      __b = lst_prev(self, __a);                                                                   \
                                                                                                   \
      __b;                                                                                         \
    })

#  define u_list_next(self, ptr)                                                                   \
    ({                                                                                             \
      u_check(self, 1, __u_list_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0)* __a = ptr;                                                                 \
      u_types(self, 0)* __b = {};                                                                  \
                                                                                                   \
      __b = lst_next(self, __a);                                                                   \
                                                                                                   \
      __b;                                                                                         \
    })

/* clang-format off */
#define u_list_pop(self, ...)                                                                      \
  ({                                                                                               \
    u_check(self, 1, __u_list_ref_t);                                                              \
                                                                                                   \
    u_types(self, 0)* __a = u_va_0th(lst_head(self), __VA_ARGS__);                                 \
                                                                                                   \
    lst_pop(self, __a);                                                                            \
                                                                                                   \
    __a;                                                                                           \
  })                                                                                               \

#define u_list_put(self, ptr, ...)                                                                 \
  do {                                                                                             \
    u_check(self, 1, __u_list_ref_t);                                                              \
                                                                                                   \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      u_types(self, 0)* __a = ptr;                                                                 \
      u_types(self, 0)* __b = u_va_at(0, __VA_ARGS__);                                             \
    ) (                                                                                            \
      u_types(self, 0)* __a = lst_tail(self);                                                      \
      u_types(self, 0)* __b = ptr;                                                                 \
    )                                                                                              \
                                                                                                   \
    lst_put(self, __a, __b);                                                                       \
  } while (0)
/* clang-format on */

#  define u_list_for(self, it, ...)                                                                \
    u_va_for_let (bool, __init, {})                                                                \
      for (u_types(self, 0)* it = {};                                                              \
           lst_for(self, &__init, (any_t*)&it, u_va_0th(U_ORDER_ASCEND, __VA_ARGS__));)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ILST_H__ */
