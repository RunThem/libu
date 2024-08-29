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

#ifndef U_ISET_H__
#  define U_ISET_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* __u_set_ref_t;

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_set_t(T) typeof(__u_set_ref_t(*)(T*))

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#  define u_set_init(self, fn)                                                                     \
    do {                                                                                           \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      self = avl_new(sizeof(u_types(self, 0)), 0, fn);                                             \
    } while (0)

#  define u_set_new(T, fn)                                                                         \
    ({                                                                                             \
      u_set_t(T) self = nullptr;                                                                   \
                                                                                                   \
      u_set_init(self, fn);                                                                        \
                                                                                                   \
      self;                                                                                        \
    })

#  define u_set_len(self)                                                                          \
    ({                                                                                             \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      avl_len(self);                                                                               \
    })

#  define u_set_is_empty(self)                                                                     \
    ({                                                                                             \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      0 == avl_len(self);                                                                          \
    })

#  define u_set_is_exist(self, item)                                                               \
    ({                                                                                             \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      u_types(self, 0) __a = item;                                                                 \
                                                                                                   \
      avl_is_exist(self, &__a);                                                                    \
    })

#  define u_set_clear(self)                                                                        \
    do {                                                                                           \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      avl_clear(self);                                                                             \
    } while (0)

#  define u_set_cleanup(self)                                                                      \
    do {                                                                                           \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      avl_cleanup(self);                                                                           \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

/* clang-format off */
#  define u_set_at(self, item, ...)                                                                \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      ({                                                                                           \
        u_check(self, 1, __u_set_ref_t);                                                           \
                                                                                                   \
        bool __ret              = false;                                                           \
        u_types(self, 0) __item = item;                                                            \
        u_types(self, 0) __a    = u_va_at(0, __VA_ARGS__);                                         \
        u_types(self, 0)* __b   = {};                                                              \
                                                                                                   \
        __b = avl_at(self, &__item);                                                               \
                                                                                                   \
        if (__b != nullptr && 0 == avl_fn(self)(&__item, &__a)) {                                  \
          *(__b - 1) = u_va_at(0, __VA_ARGS__);                                                    \
          __ret = true;                                                                            \
        }                                                                                          \
                                                                                                   \
        __ret;                                                                                     \
      })                                                                                           \
    ) (                                                                                            \
      ({                                                                                           \
        u_check(self, 1, __u_set_ref_t);                                                           \
                                                                                                   \
        u_types(self, 0) __item  = item;                                                           \
        u_types(self, 0) __a     = {};                                                             \
        u_types(self, 0)* __b    = {};                                                             \
                                                                                                   \
        __b = avl_at(self, &__item);                                                               \
                                                                                                   \
        if (__b == nullptr) {                                                                      \
          __b = &__a;                                                                              \
        } else {                                                                                   \
          __b -= 1;                                                                                \
        }                                                                                          \
                                                                                                   \
        *__b;                                                                                      \
      })                                                                                           \
    )
/* clang-format on */

#  define u_set_try(self, item)                                                                    \
    for (u_types(self, 0)* it = ({                                                                 \
           u_types(self, 0) __item = item;                                                         \
                                                                                                   \
           avl_at(self, &__item);                                                                  \
         });                                                                                       \
         it != nullptr && (it -= 1);                                                               \
         it = nullptr)

#  define u_set_pop(self, item)                                                                    \
    ({                                                                                             \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      u_types(self, 0) __item = item;                                                              \
                                                                                                   \
      avl_pop(self, &__item, &__item);                                                             \
                                                                                                   \
      __item;                                                                                      \
    })

#  define u_set_put(self, item)                                                                    \
    do {                                                                                           \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      u_types(self, 0) __item = item;                                                              \
                                                                                                   \
      avl_put(self, &__item, &__item);                                                             \
    } while (0)

#  define u_set_fn(self, it1, it2)                                                                 \
    ({                                                                                             \
      u_check(self, 1, __u_set_ref_t);                                                             \
                                                                                                   \
      u_types(self, 0) __a = it1;                                                                  \
      u_types(self, 0) __b = it2;                                                                  \
                                                                                                   \
      avl_fn(self)(&__a, &__b);                                                                    \
    })

#  define u_set_for(self, it)                                                                      \
    for (u_types(self, 0) it = {}; avl_for_init(self, 1); avl_for_end(self))                       \
      for (; avl_for(self, &it, &it);)

#  define u_set_rfor(self, it)                                                                     \
    for (u_types(self, 0) it = {}; avl_for_init(self, 0); avl_for_end(self))                       \
      for (; avl_for(self, &it, &it);)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ISET_H__ */
