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

/* clang-format off */

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern any_t $map_new      (i32_t, i32_t, u_hash_fn);
extern void  $map_clear    (any_t);
extern void  $map_cleanup  (any_t);
extern any_t $map_at       (any_t, any_t, any_t);
extern void  $map_pop      (any_t, any_t, any_t);
extern void  $map_put      (any_t, any_t, any_t);
extern bool  $map_each     (any_t, any_t, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_map_t(K, V)                                                                            \
    typeof(const struct [[gnu::packed]] {                                                          \
      void* ref;                                                                                   \
      int len;                                                                                     \
                                                                                                   \
      struct { K key; V val; } _[0]; /* Don't use this field. */                                   \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_map_new(K, V, ...)                                                                     \
    ({                                                                                             \
      u_map_t(K, V) self = $map_new(sizeof(K), sizeof(V), u_va_0th(nullptr, __VA_ARGS__));         \
                                                                                                   \
      self->ref;                                                                                   \
    })

#  define u_map_is_exist(self, k)                                                                  \
    ({                                                                                             \
      typeof_unqual(self->_[0]) __it__ = {k};                                                      \
                                                                                                   \
      nullptr != $map_at(self->ref, &__it__.key, nullptr);                                         \
    })

#  define u_map_clear(self)                                                                        \
    do {                                                                                           \
      $map_clear(self->ref);                                                                       \
    } while (0)

#  define u_map_cleanup(self)                                                                      \
    do {                                                                                           \
      $map_cleanup(self->ref);                                                                     \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

#  define u_map_at(self, k, ...)                                                                   \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      ({                                                                                           \
        typeof_unqual(self->_[0]) __it__ = {k, u_va_at(0, __VA_ARGS__)};                           \
                                                                                                   \
        $map_at(self->ref, &__it__.key, &__it__.val);                                              \
      })                                                                                           \
    ) (                                                                                            \
      ({                                                                                           \
        typeof_unqual(self->_[0].key) __key__  = k;                                                \
        typeof_unqual(self->_[0].val)* __val__ = nullptr;                                          \
                                                                                                   \
        __val__ = $map_at(self->ref, &__key__, nullptr);                                           \
                                                                                                   \
        *__val__;                                                                                  \
      })                                                                                           \
    )

#  define u_map_try(self, k)                                                                       \
    for (typeof_unqual(self->_[0].val)* it =                                                       \
             $map_at(self->ref, &(typeof_unqual(self->_[0].key)){k}, nullptr);                     \
         it;                                                                                       \
         it = nullptr)

#  define u_map_pop(self, k)                                                                       \
    ({                                                                                             \
      typeof_unqual(self->_[0]) __it__ = {k};                                                      \
                                                                                                   \
      $map_pop(self->ref, &__it__.key, &__it__.val);                                               \
                                                                                                   \
      __it__.val;                                                                                  \
    })

#  define u_map_put(self, k, v)                                                                    \
    do {                                                                                           \
      typeof_unqual(self->_[0]) __it__ = {k, v};                                                   \
                                                                                                   \
      $map_put(self->ref, &__it__.key, &__it__.val);                                               \
    } while (0)

#  define u_map_each(self, it)                                                                     \
    $map_each(self->ref, nullptr, nullptr);                                                        \
    for (typeof_unqual(self->_[0]) it = {}; $map_each(self->ref, &it.key, &it.val);)

#  define u_map_each_if(self, it, cond)                                                            \
    $map_each(self->ref, nullptr, nullptr);                                                        \
    for (typeof_unqual(self->_[0]) it = {}; $map_each(self->ref, &it.key, &it.val);)               \
      if (cond)

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IMAP_H__ */
