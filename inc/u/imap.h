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
 * */

#pragma once

#ifndef U_IMAP_H__
#  define U_IMAP_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* __u_map_ref_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern any_t  map_new       (size_t, size_t, u_hash_fn);
extern size_t map_len       (any_t);
extern bool   map_exist     (any_t, any_t);
extern void   map_clear     (any_t);
extern void   map_cleanup   (any_t);
extern any_t  map_at        (any_t, any_t);
extern void   map_pop       (any_t, any_t, any_t);
extern void   map_put       (any_t, any_t, any_t);
extern void   vec_sort      (any_t, u_cmp_fn);
extern bool   map_for_init  (any_t, bool);
extern void   map_for_end   (any_t);
extern bool   map_for       (any_t, any_t, any_t);
/* clang-format on */

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_map_t(K, V) typeof(__u_map_ref_t(*)(K*, V*))

/***************************************************************************************************
 * iApi map
 **************************************************************************************************/
#  define u_map_init(self)                                                                         \
    do {                                                                                           \
      u_check(self, 2, __u_map_ref_t);                                                             \
                                                                                                   \
      self = map_new(sizeof(u_types(self, 0)), sizeof(u_types(self, 1)), nullptr);                 \
    } while (0)

#  define u_map_new(K, V)                                                                          \
    ({                                                                                             \
      u_map_t(K, V) self = nullptr;                                                                \
                                                                                                   \
      u_map_init(self);                                                                            \
                                                                                                   \
      self;                                                                                        \
    })

#  define u_map_len(self)                                                                          \
    ({                                                                                             \
      u_check(self, 2, __u_map_ref_t);                                                             \
                                                                                                   \
      map_len(self);                                                                               \
    })

#  define u_map_is_empty(self)                                                                     \
    ({                                                                                             \
      u_check(self, 2, __u_map_ref_t);                                                             \
                                                                                                   \
      0 == map_len(self);                                                                          \
    })

#  define u_map_is_exist(self, key)                                                                \
    ({                                                                                             \
      u_check(self, 2, __u_map_ref_t);                                                             \
                                                                                                   \
      u_types(self, 0) __a = key;                                                                  \
      u_types(self, 1) __b = {};                                                                   \
                                                                                                   \
      map_exist(self, &__a);                                                                       \
    })

#  define u_map_clear(self)                                                                        \
    do {                                                                                           \
      u_check(self, 2, __u_map_ref_t);                                                             \
                                                                                                   \
      map_clear(self);                                                                             \
    } while (0)

#  define u_map_cleanup(self)                                                                      \
    do {                                                                                           \
      u_check(self, 2, __u_map_ref_t);                                                             \
                                                                                                   \
      map_cleanup(self);                                                                           \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

/* clang-format off */
#  define u_map_at(self, key, ...)                                                                 \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      ({                                                                                           \
        u_check(self, 2, __u_map_ref_t);                                                           \
                                                                                                   \
        bool __ret             = false;                                                            \
        u_types(self, 0) __a  = key;                                                               \
        u_types(self, 1)* __b = {};                                                                \
                                                                                                   \
        __b = map_at(self, &__a);                                                                  \
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
        u_check(self, 2, __u_map_ref_t);                                                           \
                                                                                                   \
        u_types(self, 0) __a  = key;                                                               \
        u_types(self, 1) __it = {};                                                                \
        u_types(self, 1)* __b = {};                                                                \
                                                                                                   \
        __b = map_at(self, &__a);                                                                  \
                                                                                                   \
        if (__b == nullptr) {                                                                      \
          __b = &__it;                                                                             \
        }                                                                                          \
                                                                                                   \
        *__b;                                                                                      \
      })                                                                                           \
    )
/* clang-format on */

#  define u_map_try(self, key)                                                                     \
    for (u_types(self, 1)* it = ({                                                                 \
           u_types(self, 0) __key = key;                                                           \
           map_at(self, &__key);                                                                   \
         });                                                                                       \
         it != nullptr;                                                                            \
         it = nullptr)

#  define u_map_pop(self, key)                                                                     \
    ({                                                                                             \
      u_check(self, 2, __u_map_ref_t);                                                             \
                                                                                                   \
      u_types(self, 0) __a = key;                                                                  \
      u_types(self, 1) __b = {};                                                                   \
                                                                                                   \
      map_pop(self, &__a, &__b);                                                                   \
                                                                                                   \
      __b;                                                                                         \
    })

#  define u_map_put(self, key, val)                                                                \
    do {                                                                                           \
      u_check(self, 2, __u_map_ref_t);                                                             \
                                                                                                   \
      u_types(self, 0) __a = key;                                                                  \
      u_types(self, 1) __b = val;                                                                  \
                                                                                                   \
      map_put(self, &__a, &__b);                                                                   \
    } while (0)

#  define u_map_for(self, key, val)                                                                \
    for (u_types(self, 0) key = {}; map_for_init(self, 1); map_for_end(self))                      \
      for (u_types(self, 1) val = {}; map_for(self, &key, &val);)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IMAP_H__ */
