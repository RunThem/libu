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

#ifndef U_IAVL_H__
#  define U_IAVL_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* __u_tree_ref_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern any_t avl_new      (i64_t, i64_t, u_cmp_fn);
extern i64_t avl_len      (any_t);
extern bool  avl_is_exist (any_t, any_t);
extern void  avl_clear    (any_t);
extern void  avl_cleanup  (any_t);
extern any_t avl_at       (any_t, any_t);
extern void  avl_pole     (any_t, any_t, any_t, u_order_e);
extern void  avl_pop      (any_t, any_t, any_t);
extern void  avl_put      (any_t, any_t, any_t);
extern bool  avl_for      (any_t, any_t, any_t, any_t*, u_order_e, any_t);
/* clang-format on */

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_tree_t(K, V) typeof(__u_tree_ref_t(*)(K*, V*))

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#  define u_tree_init(self, cmp_fn)                                                                \
    do {                                                                                           \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      self = avl_new(sizeof(u_types(self, 0)), sizeof(u_types(self, 1)), cmp_fn);                  \
    } while (0)

#  define u_tree_new(K, V, cmp_fn)                                                                 \
    ({                                                                                             \
      u_tree_t(K, V) self = nullptr;                                                               \
                                                                                                   \
      self = avl_new(sizeof(K), sizeof(V), cmp_fn);                                                \
                                                                                                   \
      self;                                                                                        \
    })

#  define u_tree_len(self)                                                                         \
    ({                                                                                             \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      avl_len(self);                                                                               \
    })

#  define u_tree_is_empty(self)                                                                    \
    ({                                                                                             \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      0 == avl_len(self);                                                                          \
    })

#  define u_tree_is_exist(self, key)                                                               \
    ({                                                                                             \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0) __a = key;                                                                  \
                                                                                                   \
      avl_is_exist(self, &__a);                                                                    \
    })

#  define u_tree_clear(self)                                                                       \
    do {                                                                                           \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      avl_clear(self);                                                                             \
    } while (0)

#  define u_tree_cleanup(self)                                                                     \
    do {                                                                                           \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      avl_cleanup(self);                                                                           \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

/* clang-format off */
#  define u_tree_at(self, key, ...)                                                                \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      ({                                                                                           \
        u_check(self, 2, __u_tree_ref_t);                                                          \
                                                                                                   \
        bool __ret            = false;                                                             \
        u_types(self, 0) __a  = key;                                                               \
        u_types(self, 1)* __b = {};                                                                \
                                                                                                   \
        __b = avl_at(self, &__a);                                                                  \
                                                                                                   \
        if (__b) {                                                                                 \
          *__b = u_va_at(0, __VA_ARGS__);                                                          \
          __ret = true;                                                                            \
        }                                                                                          \
                                                                                                   \
        __ret;                                                                                     \
      })                                                                                           \
    ) (                                                                                            \
      ({                                                                                           \
        u_check(self, 2, __u_tree_ref_t);                                                          \
                                                                                                   \
        u_types(self, 0) __a  = key;                                                               \
        u_types(self, 1) __it = {};                                                                \
        u_types(self, 1)* __b = {};                                                                \
                                                                                                   \
        __b = avl_at(self, &__a);                                                                  \
                                                                                                   \
        if (!__b) {                                                                                \
          __b = &__it;                                                                             \
        }                                                                                          \
                                                                                                   \
        *__b;                                                                                      \
      })                                                                                           \
    )
/* clang-format on */

#  define u_tree_try(self, key)                                                                    \
    for (u_types(self, 1) __key = key, *it = avl_at(self, &__key); it; it = nullptr)

#  define u_tree_min(self)                                                                         \
    ({                                                                                             \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      struct {                                                                                     \
        u_types(self, 0) key;                                                                      \
        u_types(self, 1) val;                                                                      \
      } __ = {};                                                                                   \
                                                                                                   \
      avl_pole(self, &__.key, &__.val, U_ORDER_ASCEND);                                            \
                                                                                                   \
      __;                                                                                          \
    })

#  define u_tree_max(self)                                                                         \
    ({                                                                                             \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      struct {                                                                                     \
        u_types(self, 0) key;                                                                      \
        u_types(self, 1) val;                                                                      \
      } __ = {};                                                                                   \
                                                                                                   \
      avl_pole(self, &__.key, &__.val, U_ORDER_DESCEND);                                           \
                                                                                                   \
      __;                                                                                          \
    })

#  define u_tree_pop(self, key)                                                                    \
    ({                                                                                             \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0) __a = key;                                                                  \
      u_types(self, 1) __b = {};                                                                   \
                                                                                                   \
      avl_pop(self, &__a, &__b);                                                                   \
                                                                                                   \
      __b;                                                                                         \
    })

#  define u_tree_put(self, key, val)                                                               \
    do {                                                                                           \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0) __a = key;                                                                  \
      u_types(self, 1) __b = val;                                                                  \
                                                                                                   \
      avl_put(self, &__a, &__b);                                                                   \
    } while (0)

#  define u_tree_for(self, key, val, ...)                                                          \
    for (u_types(self, 0) key = {}, *_ = &key, *__iter = nullptr; _; _ = nullptr)                  \
      for (u_types(self, 1) val = {};                                                              \
           avl_for(self, &key, &val, (any_t*)&__iter, u_va_0th(U_ORDER_ASCEND, __VA_ARGS__), _);   \
           _ = nullptr)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IAVL_H__ */
