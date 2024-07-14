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

#ifndef U_IVEC_H__
#  define U_IVEC_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* __u_vec_ref_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern any_t  vec_new       (size_t);
extern size_t vec_len       (any_t);
extern size_t vec_cap       (any_t);
extern bool   vec_exist     (any_t, int);
extern void   vec_clear     (any_t);
extern void   vec_cleanup   (any_t);
extern any_t  vec_at        (any_t, int);
extern void   vec_pop       (any_t, int, any_t);
extern void   vec_put       (any_t, int, any_t);
extern void   vec_sort      (any_t, u_cmp_fn);
extern bool   vec_for_init  (any_t, bool);
extern void   vec_for_end   (any_t);
extern bool   vec_for       (any_t, int*, any_t);
/* clang-format on */

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_vec_t(T) typeof(__u_vec_ref_t(*)(T*))

/***************************************************************************************************
 * iApi vec
 **************************************************************************************************/
#  define u_vec_init(self)                                                                         \
    do {                                                                                           \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      self = vec_new(sizeof(u_types(self, 0)));                                                    \
    } while (0)

#  define u_vec_new(T)                                                                             \
    ({                                                                                             \
      u_vec_t(T) self = nullptr;                                                                   \
                                                                                                   \
      u_vec_init(self);                                                                            \
                                                                                                   \
      self;                                                                                        \
    })

#  define u_vec_len(self)                                                                          \
    ({                                                                                             \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      vec_len(self);                                                                               \
    })

#  define u_vec_cap(self)                                                                          \
    ({                                                                                             \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      vec_cap(self);                                                                               \
    })

#  define u_vec_is_empty(self)                                                                     \
    ({                                                                                             \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      0 == vec_len(self);                                                                          \
    })

#  define u_vec_is_exist(self, idx)                                                                \
    ({                                                                                             \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      int _a = idx;                                                                                \
                                                                                                   \
      vec_exist(self, _a);                                                                         \
    })

#  define u_vec_clear(self)                                                                        \
    do {                                                                                           \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      vec_clear(self);                                                                             \
    } while (0)

#  define u_vec_cleanup(self)                                                                      \
    do {                                                                                           \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      vec_cleanup(self);                                                                           \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

/* clang-format off */
#  define u_vec_at(self, idx, ...)                                                                 \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        bool __ret            = false;                                                             \
        int __a               = idx;                                                               \
        u_types(self, 0)* __b = {};                                                                \
                                                                                                   \
        __b = vec_at(self, __a);                                                                   \
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
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        u_types(self, 0) __it = {};                                                                \
        u_types(self, 0)* __b = {};                                                                \
                                                                                                   \
        __b = vec_at(self, idx);                                                                   \
                                                                                                   \
        if (__b == nullptr) {                                                                      \
          __b = &__it;                                                                             \
        }                                                                                          \
                                                                                                   \
        *__b;                                                                                      \
      })                                                                                           \
    )
/* clang-format on */

#  define u_vec_try(self, i)                                                                       \
    for (u_types(self, 0)* it = vec_at(self, i); it != nullptr; it = nullptr)

/* clang-format off */
#  define u_vec_pop(self, ...)                                                                     \
    ({                                                                                             \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                   \
        int __a              = u_va_at(0, __VA_ARGS__);                                            \
        u_types(self, 0) __b = {};                                                                 \
      ) (                                                                                          \
        int __a              = -1;                                                                 \
        u_types(self, 0) __b = {};                                                                 \
      )                                                                                            \
                                                                                                   \
      vec_pop(self, __a, &__b);                                                                    \
                                                                                                   \
      __b;                                                                                         \
    })

#  define u_vec_put(self, tmp, ...)                                                                \
    do {                                                                                           \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                   \
        int __a             = tmp;                                                                 \
        u_types(self, 0) __b = u_va_at(0, __VA_ARGS__);                                            \
      )(                                                                                           \
        auto __a             = -1;                                                                 \
        u_types(self, 0) __b = tmp;                                                                \
      )                                                                                            \
                                                                                                   \
      vec_put(self, __a, &__b);                                                                    \
    } while (0)
/* clang-format on */

#  define u_vec_sort(self, cmp_fn)                                                                 \
    do {                                                                                           \
      u_check(self, 1, __u_vec_ref_t);                                                             \
                                                                                                   \
      vec_sort(self, cmp_fn);                                                                      \
    } while (0)

#  define u_vec_for(self, i, it)                                                                   \
    for (int i = 0; vec_for_init(self, 1); vec_for_end(self))                                      \
      for (u_types(self, 0) it = {}; vec_for(self, &i, &it);)

#  define u_vec_rfor(self, i, it)                                                                  \
    for (int i = 0; vec_for_init(self, 0); vec_for_end(self))                                      \
      for (u_types(self, 0) it = {}; vec_for(self, &i, &it);)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IVEC_H__ */
