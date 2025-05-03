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

#ifndef U_ITREE_H__
#  define U_ITREE_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/* clang-format off */

/***************************************************************************************************
 * Api
 **************************************************************************************************/
typedef struct {}* $tree_t;

extern any_t $tree_new      (i32_t, i32_t, u_cmp_fn);
extern bool  $tree_is_exist (any_t, any_t);
extern void  $tree_clear    (any_t);
extern void  $tree_cleanup  (any_t);
extern any_t $tree_at       (any_t, any_t, any_t);
extern void  $tree_pop      (any_t, any_t, any_t);
extern void  $tree_put      (any_t, any_t, any_t);
extern bool  $tree_each     (any_t, any_t, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_tree_t(K, V)                                                                           \
    typeof(const struct [[gnu::packed]] {                                                          \
      $tree_t ref;                                                                                 \
      int len;                                                                                     \
                                                                                                   \
      struct { K key; V val; } _[0]; /* Don't use this field. */                                   \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_tree_new(K, V, cmp_fn)                                                                 \
    ({                                                                                             \
      u_tree_t(K, V) self = $tree_new(sizeof(K), sizeof(V), cmp_fn);                               \
                                                                                                   \
      (any_t)self->ref;                                                                            \
    })

#  define u_tree_is_exist(self, k)                                                                 \
    ({                                                                                             \
      typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                                 \
                                                                                                   \
      typeof_unqual(self->_[0]) __it__ = {k};                                                      \
                                                                                                   \
      nullptr != $tree_at(self->ref, &__it__.key, nullptr);                                        \
    })

#  define u_tree_clear(self)                                                                       \
    do {                                                                                           \
      typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                                 \
                                                                                                   \
      $tree_clear(self->ref);                                                                      \
    } while (0)

#  define u_tree_cleanup(self)                                                                     \
    do {                                                                                           \
      typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                                 \
                                                                                                   \
      $tree_cleanup(self->ref);                                                                    \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

#  define u_tree_at(self, k, ...)                                                                  \
    u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                     \
      ({                                                                                           \
        typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                               \
                                                                                                   \
        typeof_unqual(self->_[0]) __it__ = {k, u_va_at(0, __VA_ARGS__)};                           \
                                                                                                   \
        $tree_at(self->ref, &__it__.key, &__it__.val);                                             \
      })                                                                                           \
    ) (                                                                                            \
      ({                                                                                           \
        typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                               \
                                                                                                   \
        typeof_unqual(self->_[0].key) __key__  = k;                                                \
        typeof_unqual(self->_[0].val)* __val__ = nullptr;                                          \
                                                                                                   \
        __val__ = $tree_at(self->ref, &__key__, nullptr);                                          \
        assert(__val__);                                                                           \
                                                                                                   \
        *__val__;                                                                                  \
      })                                                                                           \
    )

#  define u_tree_try(self, k)                                                                      \
    typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                                   \
                                                                                                   \
    for (typeof_unqual(self->_[0].val)* it =                                                       \
             $tree_at(self->ref, &(typeof_unqual(self->_[0].key)){k}, nullptr);                    \
         it;                                                                                       \
         it = nullptr)

#  define u_tree_pop(self, k)                                                                      \
    ({                                                                                             \
      typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                                 \
                                                                                                   \
      typeof_unqual(self->_[0]) __it__ = {k};                                                      \
                                                                                                   \
      $tree_pop(self->ref, &__it__.key, &__it__.val);                                              \
                                                                                                   \
      __it__.val;                                                                                  \
    })

#  define u_tree_put(self, k, v)                                                                   \
    do {                                                                                           \
      typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                                 \
                                                                                                   \
      typeof_unqual(self->_[0]) __it__ = {k, v};                                                   \
                                                                                                   \
      $tree_put(self->ref, &__it__.key, &__it__.val);                                              \
    } while (0)

#  define u_tree_each(self, it)                                                                    \
    typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                                   \
                                                                                                   \
    $tree_each(self->ref, nullptr, nullptr);                                                       \
    for (typeof_unqual(self->_[0]) it = {}; $tree_each(self->ref, &it.key, &it.val);)

#  define u_tree_each_if(self, it, cond)                                                           \
    typecheck($tree_t, self->ref, "mete type not's Tree<K, V>");                                   \
                                                                                                   \
    $tree_each(self->ref, nullptr, nullptr);                                                       \
    for (typeof_unqual(self->_[0]) it = {}; $tree_each(self->ref, &it.key, &it.val);)              \
      if (cond)

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ITREE_H__ */
