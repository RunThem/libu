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
extern any_t    avl_new       (size_t, size_t, u_cmp_fn);
extern size_t   avl_len       (any_t);
extern bool     avl_exist     (any_t, any_t);
extern void     avl_clear     (any_t);
extern void     avl_cleanup   (any_t);
extern any_t    avl_at        (any_t, any_t);
extern void     avl_min       (any_t, any_t, any_t);
extern void     avl_max       (any_t, any_t, any_t);
extern void     avl_pop       (any_t, any_t, any_t);
extern void     avl_put       (any_t, any_t, any_t);
extern u_cmp_fn avl_fn        (any_t);
extern bool     avl_for_init  (any_t, bool);
extern void     avl_for_end   (any_t);
extern bool     avl_for       (any_t, any_t, any_t);
/* clang-format on */

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_tree_t(K, V) typeof(__u_tree_ref_t(*)(K*, V*))

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#  define u_tree_init(self, ...)                                                                   \
    do {                                                                                           \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      self = avl_new(sizeof(u_types(self, 0)),                                                     \
                     sizeof(u_types(self, 1)),                                                     \
                     u_va_0th(nullptr, __VA_ARGS__));                                              \
    } while (0)

#  define u_tree_new(K, V, ...)                                                                    \
    ({                                                                                             \
      u_tree_t(K, V) self = nullptr;                                                               \
                                                                                                   \
      u_tree_init(self, __VA_ARGS__);                                                              \
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
      avl_exist(self, &__a);                                                                       \
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
        if (__b != nullptr) {                                                                      \
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
        if (__b == nullptr) {                                                                      \
          __b = &__it;                                                                             \
        }                                                                                          \
                                                                                                   \
        *__b;                                                                                      \
      })                                                                                           \
    )
/* clang-format on */

#  define u_tree_try(self, key)                                                                    \
    for (u_types(self, 1)* it = ({                                                                 \
           u_types(self, 0) __key = key;                                                           \
           avl_at(self, &__key);                                                                   \
         });                                                                                       \
         it != nullptr;                                                                            \
         it = nullptr)

#  define u_tree_min(self)                                                                         \
    ({                                                                                             \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      struct {                                                                                     \
        u_types(self, 0) key;                                                                      \
        u_types(self, 1) val;                                                                      \
      } __ = {};                                                                                   \
                                                                                                   \
      avl_min(self, &__.key, &__.val);                                                             \
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
      avl_max(self, &__.key, &__.val);                                                             \
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

#  define u_tree_fn(self, k1, k2)                                                                  \
    ({                                                                                             \
      u_check(self, 2, __u_tree_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0) __a = k1;                                                                   \
      u_types(self, 1) __b = k2;                                                                   \
                                                                                                   \
      avl_fn(self)(&__a, &__b);                                                                    \
    })

#  define u_tree_for(self, key, val)                                                               \
    for (u_types(self, 0) key = {}; avl_for_init(self, 1); avl_for_end(self))                      \
      for (u_types(self, 1) val = {}; avl_for(self, &key, &val);)

#  define u_tree_rfor(self, key, val)                                                              \
    for (u_types(self, 0) key = {}; avl_for_init(self, 0); avl_for_end(self))                      \
      for (u_types(self, 1) val = {}; avl_for(self, &key, &val);)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IAVL_H__ */
