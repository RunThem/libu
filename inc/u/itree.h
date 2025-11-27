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
extern void  $tree_clear    (any_t);
extern void  $tree_cleanup  (any_t);
extern any_t $tree_at       (any_t, any_t);
extern void  $tree_del      (any_t, any_t);
extern any_t $tree_add      (any_t, any_t);
extern any_t $tree_each     (any_t, bool);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_tree_t(K, V)                                                                           \
    typeof(const struct [[gnu::packed]] {                                                          \
      any_t ref;                                                                                   \
      int len;                                                                                     \
                                                                                                   \
      struct {                                                                                     \
        $tree_t meta;                                                                              \
        K key;                                                                                     \
        V val;                                                                                     \
        struct {       K key;       V val; }   pair;                                               \
        struct { const K key;       V val; } * mut;                                                \
        struct { const K key; const V val; } * ref;                                                \
      } _[0]; /* Don't use this field. */                                                          \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_tree_new(self, cmp_fn)                                                                 \
    ({                                                                                             \
      typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                           \
                                                                                                   \
      self = $tree_new(sizeof(self->_[0].key), sizeof(self->_[0].val), cmp_fn);                    \
                                                                                                   \
      self->ref;                                                                                   \
    })


#  define u_tree_is_valid(self, _key)                                                              \
    ({                                                                                             \
      typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].pair) __pair__ = {_key};                                            \
                                                                                                   \
      (bool) (nullptr != $tree_at(self->ref, &__pair__.key));                                      \
    })


#  define u_tree_clear(self, ...)                                                                  \
    do {                                                                                           \
      typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                           \
                                                                                                   \
      u_va_has_if(__VA_ARGS__) ( u_tree_each(self, it) { __VA_ARGS__ } )                           \
                                                                                                   \
      $tree_clear(self->ref);                                                                      \
    } while (0)


#  define u_tree_cleanup(self, ...)                                                                \
    do {                                                                                           \
      typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                           \
                                                                                                   \
      u_va_has_if(__VA_ARGS__) ( u_tree_each(self, it) { __VA_ARGS__ } )                           \
                                                                                                   \
      $tree_cleanup(self->ref);                                                                    \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)


#   define u_tree_at(self, _key, ...)                                                              \
    ({                                                                                             \
      *u_tree_at_mut(self, _key) u_va_has_if(__VA_ARGS__) ( = u_va_at(0, __VA_ARGS__) );           \
    })


#  define u_tree_at_ref(self, _key)                                                                \
    ({                                                                                             \
      typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].key) __key__ = _key;                                                \
      typeof_unqual(self->_[0].ref) __ref__ = $tree_at(self->ref, (any_t)&__key__);                \
                                                                                                   \
      assert(__ref__);                                                                             \
                                                                                                   \
      &__ref__->val;                                                                               \
    })


#  define u_tree_at_mut(self, _key)                                                                \
    ({                                                                                             \
      typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].key) __key__ = _key;                                                \
      typeof_unqual(self->_[0].mut) __mut__ = $tree_at(self->ref, (any_t)&__key__);                \
                                                                                                   \
      assert(__mut__);                                                                             \
                                                                                                   \
      &__mut__->val;                                                                               \
    })


#  define u_tree_try_at(self, _key, ...)                                                           \
    u_va_let(typeof_unqual(self->_[0].key), key, _key);                                            \
    u_va_let(typeof_unqual(self->_[0].ref), ref, $tree_at(self->ref, (any_t)&u_va_var(key)));      \
    if (u_va_var(ref))                                                                             \
      for (auto u_va_0th(it, __VA_ARGS__) = u_va_var(ref)->val; u_va_var(ref); u_va_var(ref) = nullptr)


#  define u_tree_try_at_ref(self, _key, ...)                                                       \
    u_va_let(typeof_unqual(self->_[0].key), key, _key);                                            \
    u_va_let(typeof_unqual(self->_[0].ref), ref, $tree_at(self->ref, (any_t)&u_va_var(key)));      \
    if (u_va_var(ref))                                                                             \
      for (auto u_va_0th(it, __VA_ARGS__) = &u_va_var(ref)->val; u_va_var(ref); u_va_var(ref) = nullptr)


#  define u_tree_try_at_mut(self, _key, ...)                                                       \
    u_va_let(typeof_unqual(self->_[0].key), key, _key);                                            \
    u_va_let(typeof_unqual(self->_[0].mut), mut, $tree_at(self->ref, (any_t)&u_va_var(key)));      \
    if (u_va_var(mut))                                                                             \
      for (auto u_va_0th(it, __VA_ARGS__) = &u_va_var(mut)->val; u_va_var(mut); u_va_var(mut) = nullptr)


#  define u_tree_remove(self, _key)                                                                \
    ({                                                                                             \
      typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].pair) __pair__ = {_key};                                            \
      typeof_unqual(self->_[0].ref) __ref__ = $tree_at(self->ref, &__pair__.key);                  \
      assert(__ref__);                                                                             \
                                                                                                   \
      __pair__.val = __ref__->val;                                                                 \
      $tree_del(self->ref, (any_t)__ref__);                                                        \
                                                                                                   \
      __pair__.val;                                                                                \
    })


#  define u_tree_insert(self, _key, _val)                                                          \
    do {                                                                                           \
      typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].pair) __pair__ = {_key, _val};                                      \
      typeof_unqual(__pair__)* __mut__ = $tree_add(self->ref, &__pair__.key);                      \
      assert(__mut__);                                                                             \
                                                                                                   \
      __mut__->key = __pair__.key;                                                                 \
      __mut__->val = __pair__.val;                                                                 \
    } while (0)


#  define u_tree_each(self, it)                                                                    \
    typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                             \
                                                                                                   \
    (void)$tree_each(self->ref, !0);                                                               \
    for (auto it = (typeof(self->_[0].pair)) {}; ({ typeof(it)* __ref__ = $tree_each(self->ref, !!0); if (__ref__) it = *__ref__; __ref__; }); )


#  define u_tree_each_if(self, it, cond) u_tree_each(self, it) if (cond)


#  define u_tree_each_ref(self, it)                                                                \
    typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                             \
                                                                                                   \
    (void)$tree_each(self->ref, !0);                                                               \
    for (auto it = (typeof(self->_[0].ref)) {}; (it = $tree_each(self->ref, !!0)); )


#  define u_tree_each_if_ref(self, it, cond) u_tree_each_ref(self, it) if (cond)


#  define u_tree_each_mut(self, it)                                                                \
    typecheck($tree_t, self->_[0].meta, "meta type not's Tree<K, V>");                             \
                                                                                                   \
    (void)$tree_each(self->ref, !0);                                                               \
    for (auto it = (typeof(self->_[0].mut)) {}; (it = $tree_each(self->ref, !!0)); )


#  define u_tree_each_if_mut(self, it, cond) u_tree_each_mut(self, it) if (cond)

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ITREE_H__ */
