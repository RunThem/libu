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

/* clang-format off */

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern any_t $vec_new      (i32_t);
extern bool  $vec_is_exist (any_t, i32_t);
extern void  $vec_clear    (any_t);
extern void  $vec_cleanup  (any_t);
extern int   $vec_resize   (any_t, i32_t);
extern any_t $vec_at       (any_t, i32_t, any_t);
extern void  $vec_pop      (any_t, i32_t, any_t);
extern void  $vec_put      (any_t, i32_t, any_t);
extern bool  $vec_each     (any_t, any_t);

#if 0
extern void  vec_sort     (any_t, u_cmp_fn, u_order_e);
extern bool  vec_is_sort  (any_t, u_cmp_fn, u_order_e);
extern i64_t vec_pole     (any_t, u_cmp_fn, u_order_e);
extern bool  vec_each     (any_t, bool*, i64_t*, any_t*, u_order_e);
extern bool  vec_map_by   (any_t, bool*, i64_t*, any_t);
extern bool  vec_filter_by(any_t, bool*, i64_t*, any_t, bool*);
#endif

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_vec_t(T)                                                                               \
    typeof(const struct [[gnu::packed]] {                                                          \
      void* ref;                                                                                   \
      int len;                                                                                     \
      int cap;                                                                                     \
                                                                                                   \
      struct { int idx; T val; } _[0]; /* Don't use this field. */                                 \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_vec_new(T)                                                                             \
    ({                                                                                             \
      u_vec_t(T) self = $vec_new(sizeof(T));                                                       \
                                                                                                   \
      self->ref;                                                                                   \
    })

#  define u_vec_is_exist(self, idx)                                                                \
    ({                                                                                             \
      nullptr != $vec_at(self->ref, idx, nullptr);                                                 \
    })

#  define u_vec_resize(self, cap)                                                                  \
    ({                                                                                             \
      vec_resize(self->ref, cap);                                                                  \
    })

#  define u_vec_clear(self)                                                                        \
    do {                                                                                           \
      $vec_clear(self->ref);                                                                       \
    } while (0)

#  define u_vec_cleanup(self)                                                                      \
    do {                                                                                           \
      $vec_cleanup(self->ref);                                                                     \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

#  define u_vec_at(self, i, ...)                                                                   \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      ({                                                                                           \
        typeof_unqual(self->_[0]) __it__ = {i, u_va_at(0, __VA_ARGS__)};                           \
                                                                                                   \
        $vec_at(self->ref, __it__.idx, &__it__.val);                                               \
      })                                                                                           \
    ) (                                                                                            \
      ({                                                                                           \
        typeof_unqual(self->_[0].val)* __val__ = $vec_at(self->ref, i, nullptr);                   \
                                                                                                   \
        *__val__;                                                                                  \
      })                                                                                           \
    )

#  define u_vec_try(self, i)                                                                       \
    for (typeof_unqual(self->_[0].val)* it = $vec_at(self->ref, i, nullptr); it; it = nullptr)

#  define u_vec_pop(self, ...)                                                                     \
    ({                                                                                             \
      typeof_unqual(self->_[0]) __it__ = {u_va_0th(-1, __VA_ARGS__)};                              \
                                                                                                   \
      $vec_pop(self->ref, __it__.idx, &__it__.val);                                                \
                                                                                                   \
      __it__.val;                                                                                  \
    })

#  define u_vec_put(self, tmp, ...)                                                                \
    do {                                                                                           \
      u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                   \
        typeof_unqual(self->_[0]) __it__ = {tmp, u_va_at(0, __VA_ARGS__)};                         \
      )(                                                                                           \
        typeof_unqual(self->_[0]) __it__ = {-1, tmp};                                              \
      )                                                                                            \
                                                                                                   \
      $vec_put(self->ref, __it__.idx, &__it__.val);                                                \
    } while (0)

#  define u_vec_each(self, it)                                                                     \
    $vec_each(self->ref, nullptr);                                                                 \
    for (typeof_unqual(self->_[0].val) it = {}; $vec_each(self->ref, &it);)

#  define u_vec_each_if(self, it, cond)                                                            \
    $vec_each(self->ref, nullptr);                                                                 \
    for (typeof_unqual(self->_[0].val) it = {}; $vec_each(self->ref, &it);)                        \
      if (cond)

#  define u_vec_find_if(self, cond)                                                                \
    ({                                                                                             \
      typeof_unqual(self->_[0].val) __ = {};                                                       \
                                                                                                   \
      u_vec_each_if(self, it, cond) {                                                              \
        __ = it;                                                                                   \
        break;                                                                                     \
      }                                                                                            \
                                                                                                   \
      __;                                                                                          \
    })

/* clang-format on */

#  if 0
#    define u_vec_minidx(self, cmp_fn)                                                             \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        vec_pole(self, cmp_fn, U_ORDER_ASCEND);                                                    \
      })

#    define u_vec_maxidx(self, cmp_fn)                                                             \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        vec_pole(self, cmp_fn, U_ORDER_DESCEND);                                                   \
      })

#    define u_vec_min(self, cmp_fn)                                                                \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        i64_t __idx           = {};                                                                \
        u_types(self, 0)* __b = {};                                                                \
                                                                                                   \
        __idx = vec_pole(self, cmp_fn, U_ORDER_ASCEND);                                            \
        __b   = vec_at(self, __idx);                                                               \
                                                                                                   \
        *__b;                                                                                      \
      })

#    define u_vec_max(self, cmp_fn)                                                                \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        i64_t __idx           = {};                                                                \
        u_types(self, 0)* __b = {};                                                                \
                                                                                                   \
        __idx = vec_pole(self, cmp_fn, U_ORDER_DESCEND);                                           \
        __b   = vec_at(self, __idx);                                                               \
                                                                                                   \
        *__b;                                                                                      \
      })

#    define u_vec_sort(self, cmp_fn, ...)                                                          \
      do {                                                                                         \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        vec_sort(self, cmp_fn, u_va_0th(U_ORDER_ASCEND, __VA_ARGS__));                             \
      } while (0)

#    define u_vec_is_sort(self, cmp_fn, ...)                                                       \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        vec_is_sort(self, cmp_fn, u_va_0th(U_ORDER_ASCEND, __VA_ARGS__));                          \
      })

#    define u_vec_map_by(self, fn)                                                                 \
      ({                                                                                           \
        typeof(self) __self = vec_new(sizeof(u_types(self, 0)));                                   \
                                                                                                   \
        for (i64_t i = 0; i < vec_len(self); i++) {                                                \
          u_types(self, 0)* it = vec_at(self, i);                                                  \
          u_types(self, 0) nit = fn(i, *it);                                                       \
          vec_put(__self, -1, &nit);                                                               \
        }                                                                                          \
                                                                                                   \
        __self;                                                                                    \
      })

#  endif

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IVEC_H__ */
