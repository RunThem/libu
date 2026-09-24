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

#ifndef U_ITREE_H__
#define U_ITREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

/***************************************************************************************************
 * Meta Type
 **************************************************************************************************/
typedef struct {
} u_tree_meta_t;

/***************************************************************************************************
 * iType
 **************************************************************************************************/

/**
 * ::Class Tree<K, V>
 */
#define u_tree_t(K, V)                                                                             \
  typeof(const struct [[gnu::packed]] {                                                            \
    any_t ref;                                                                                     \
    int len;                                                                                       \
                                                                                                   \
    struct {                                                                                       \
      u_tree_meta_t meta;                                                                          \
                                                                                                   \
      K key_t;                                                                                     \
      K* key_mut_t;                                                                                \
      const K* key_ref_t;                                                                          \
                                                                                                   \
      V val_t;                                                                                     \
      V* val_mut_t;                                                                                \
      const V* val_ref_t;                                                                          \
                                                                                                   \
      struct {                                                                                     \
        K key;                                                                                     \
        V val;                                                                                     \
      } new_t;                                                                                     \
                                                                                                   \
      struct {                                                                                     \
        K key;                                                                                     \
        V val;                                                                                     \
      } at_t;                                                                                      \
                                                                                                   \
      struct {                                                                                     \
        const K key;                                                                               \
        V val;                                                                                     \
      }* at_mut_t;                                                                                 \
                                                                                                   \
      struct {                                                                                     \
        const K key;                                                                               \
        const V val;                                                                               \
      }* at_ref_t;                                                                                 \
                                                                                                   \
      struct {                                                                                     \
        K key;                                                                                     \
        V val;                                                                                     \
      } remove_t;                                                                                  \
                                                                                                   \
      struct {                                                                                     \
        K key;                                                                                     \
        V val;                                                                                     \
      } insert_t;                                                                                  \
                                                                                                   \
      struct {                                                                                     \
        K key;                                                                                     \
        V val;                                                                                     \
      }* insert_mut_t;                                                                             \
                                                                                                   \
      struct {                                                                                     \
        K key;                                                                                     \
        V val;                                                                                     \
      } each_t;                                                                                    \
                                                                                                   \
      struct {                                                                                     \
        const K key;                                                                               \
        V val;                                                                                     \
      }* each_mut_t;                                                                               \
                                                                                                   \
      struct {                                                                                     \
        const K key;                                                                               \
        const V val;                                                                               \
      }* each_ref_t;                                                                               \
    } _[0]; /* Don't use this field. */                                                            \
  }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/

/**
 * ::Tree<K, V>::new(self, cmp_fn: fn (K*, K*) -> int) -> Self
 */
#define u_tree_new(self, cmp_fn)                                                                   \
  ({                                                                                               \
    extern pub any_t __u_tree_new(i32_t, u_cmp_fn);                                                \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto CmpFn = (cmp_fn);                                                                       \
      assert(CmpFn != NULL);                                                                       \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    (self) = __u_tree_new(sizeof(M.new_t), (cmp_fn));                                              \
                                                                                                   \
    (self)->ref;                                                                                   \
  })

/**
 * ::Tree<K, V>::clear(self) -> !
 * ::Tree<K, V>::clear(self, proc: <block>) -> !
 */
#define u_tree_clear(self, ...)                                                                    \
  do {                                                                                             \
    extern pub void __u_tree_clear(any_t);                                                         \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    u_va_has_if(__VA_ARGS__)(u_tree_each (self, it) { __VA_ARGS__; });                             \
                                                                                                   \
    __u_tree_clear((self)->ref);                                                                   \
  } while (0)

/**
 * ::Tree<K, V>::cleanup(self) -> !
 * ::Tree<K, V>::cleanup(self, proc: <block>) -> !
 */
#define u_tree_cleanup(self, ...)                                                                  \
  do {                                                                                             \
    extern pub void __u_tree_cleanup(any_t);                                                       \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    u_va_has_if(__VA_ARGS__)(u_tree_each (self, it) { __VA_ARGS__; });                             \
                                                                                                   \
    __u_tree_cleanup((self)->ref);                                                                 \
                                                                                                   \
    (self) = NULL;                                                                                 \
  } while (0)

/**
 * ::Tree<K, V>::at(self, key: K) -> V
 * ::Tree<K, V>::at(self, key: K, val: V) -> V
 *
 * Key 必须存在, 未命中触发断言
 */
#define u_tree_at(self, _key, ...)                                                                 \
  ({                                                                                               \
    extern pub any_t __u_tree_at(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __tuple__         = {_key, __VA_ARGS__};                                        \
    typeof(M.at_mut_t) __tuple_mut__ = __u_tree_at((self)->ref, (any_t) & __tuple__.key);          \
    assert(__tuple_mut__);                                                                         \
                                                                                                   \
    __tuple_mut__->val u_va_has_if(__VA_ARGS__)(= __tuple__.val);                                  \
  })

/**
 * ::Tree<K, V>::at_ref(self, key: K) -> const V*
 *
 * Key 必须存在, 未命中触发断言
 */
#define u_tree_at_ref(self, _key)                                                                  \
  ({                                                                                               \
    extern pub any_t __u_tree_at(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __tuple__         = {_key};                                                     \
    typeof(M.at_ref_t) __tuple_ref__ = __u_tree_at((self)->ref, (any_t) & __tuple__.key);          \
    assert(__tuple_ref__);                                                                         \
                                                                                                   \
    &__tuple_ref__->val;                                                                           \
  })

/**
 * ::Tree<K, V>::at_mut(self, key: K) -> V*
 *
 * Key 必须存在, 未命中触发断言
 */
#define u_tree_at_mut(self, _key)                                                                  \
  ({                                                                                               \
    extern pub any_t __u_tree_at(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __tuple__         = {_key};                                                     \
    typeof(M.at_mut_t) __tuple_mut__ = __u_tree_at((self)->ref, (any_t) & __tuple__.key);          \
    assert(__tuple_mut__);                                                                         \
                                                                                                   \
    &__tuple_mut__->val;                                                                           \
  })

/**
 * ::Tree<K, V>::contains(self, key: K) -> bool
 */
#define u_tree_contains(self, _key)                                                                \
  ({                                                                                               \
    extern pub any_t __u_tree_at(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __tuple__ = {_key};                                                             \
                                                                                                   \
    __u_tree_at((self)->ref, (any_t) & __tuple__.key) != NULL;                                     \
  })

/**
 * ::Tree<K, V>::min(self) -> (const K, const V)*
 *
 * 空树触发断言
 */
#define u_tree_min(self)                                                                           \
  ({                                                                                               \
    extern pub any_t __u_tree_min(any_t);                                                          \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_ref_t) __ref__ = __u_tree_min((self)->ref);                                        \
    assert(__ref__ != NULL);                                                                       \
                                                                                                   \
    __ref__;                                                                                       \
  })

/**
 * ::Tree<K, V>::max(self) -> (const K, const V)*
 *
 * 空树触发断言
 */
#define u_tree_max(self)                                                                           \
  ({                                                                                               \
    extern pub any_t __u_tree_max(any_t);                                                          \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_ref_t) __ref__ = __u_tree_max((self)->ref);                                        \
    assert(__ref__ != NULL);                                                                       \
                                                                                                   \
    __ref__;                                                                                       \
  })

/**
 * ::Tree<K, V>::try_at(self, key: K) -> Option<it = V>
 * ::Tree<K, V>::try_at(self, key: K, <var-name>) -> Option<it = V>
 */
#define u_tree_try_at(self, _key, ...)                                                             \
  {                                                                                                \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (int __once__ = 1; __once__; __once__ = 0)                                                   \
    for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_[0].val_t)){}; __once__ && ({           \
         extern pub any_t __u_tree_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.at_t) __tuple__         = {_key};                                                \
         typeof(M.at_ref_t) __tuple_ref__ = __u_tree_at((self)->ref, (any_t) & __tuple__.key);     \
                                                                                                   \
         if (__tuple_ref__)                                                                        \
           u_va_0th(it, __VA_ARGS__) = __tuple_ref__->val;                                         \
                                                                                                   \
         __tuple_ref__;                                                                            \
       });                                                                                         \
       __once__ = 0)

/**
 * ::Tree<K, V>::try_at_ref(self, key: K) -> Option<it = const V*>
 * ::Tree<K, V>::try_at_ref(self, key: K, <var-name>) -> Option<it = const V*>
 */
#define u_tree_try_at_ref(self, _key, ...)                                                         \
  {                                                                                                \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (int __once__ = 1; __once__; __once__ = 0)                                                   \
    for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_[0].val_ref_t)){}; __once__ && ({       \
         extern pub any_t __u_tree_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.at_t) __tuple__         = {_key};                                                \
         typeof(M.at_ref_t) __tuple_ref__ = __u_tree_at((self)->ref, (any_t) & __tuple__.key);     \
                                                                                                   \
         if (__tuple_ref__)                                                                        \
           u_va_0th(it, __VA_ARGS__) = &__tuple_ref__->val;                                        \
                                                                                                   \
         __tuple_ref__;                                                                            \
       });                                                                                         \
       __once__ = 0)

/**
 * ::Tree<K, V>::try_at_mut(self, key: K) -> Option<it = V*>
 * ::Tree<K, V>::try_at_mut(self, key: K, <var-name>) -> Option<it = V*>
 */
#define u_tree_try_at_mut(self, _key, ...)                                                         \
  {                                                                                                \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (int __once__ = 1; __once__; __once__ = 0)                                                   \
    for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_[0].val_mut_t)){}; __once__ && ({       \
         extern pub any_t __u_tree_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.at_t) __tuple__         = {_key};                                                \
         typeof(M.at_mut_t) __tuple_mut__ = __u_tree_at((self)->ref, (any_t) & __tuple__.key);     \
                                                                                                   \
         if (__tuple_mut__)                                                                        \
           u_va_0th(it, __VA_ARGS__) = &__tuple_mut__->val;                                        \
                                                                                                   \
         __tuple_mut__;                                                                            \
       });                                                                                         \
       __once__ = 0)

/**
 * ::Tree<K, V>::remove(self, key: K) -> !
 */
#define u_tree_remove(self, _key)                                                                  \
  do {                                                                                             \
    extern pub void __u_tree_del(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.remove_t) __tuple__ = {_key};                                                         \
                                                                                                   \
    __u_tree_del((self)->ref, (any_t) & __tuple__.key);                                            \
  } while (0)

/**
 * ::Tree<K, V>::insert(self, key: K, val: V) -> !
 *
 * 重复 Key 触发断言
 */
#define u_tree_insert(self, _key, _val)                                                            \
  do {                                                                                             \
    extern pub any_t __u_tree_add(any_t, any_t);                                                   \
                                                                                                   \
    {                                                                                              \
      typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.insert_t) __tuple__ = {_key, _val};                                                   \
                                                                                                   \
    typeof(M.insert_mut_t) __tuple_mut__ = __u_tree_add((self)->ref, (any_t) & __tuple__.key);     \
    assert(__tuple_mut__);                                                                         \
                                                                                                   \
    __tuple_mut__->key = __tuple__.key;                                                            \
    __tuple_mut__->val = __tuple__.val;                                                            \
  } while (0)

/**
 * ::Tree<K, V>::each(self, it: <var-name>) -> Iter<it = (K, V)>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_each(self, it)                                                                      \
  {                                                                                                \
    extern pub any_t __u_tree_each(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_tree_each((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_t)){}; ({                                               \
         extern pub any_t __u_tree_each(any_t, bool);                                              \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.each_mut_t) __tuple_mut__ = __u_tree_each((self)->ref, !!0);                     \
                                                                                                   \
         if (__tuple_mut__) {                                                                      \
           it.key = __tuple_mut__->key;                                                            \
           it.val = __tuple_mut__->val;                                                            \
         }                                                                                         \
                                                                                                   \
         __tuple_mut__;                                                                            \
       });)

/**
 * ::Tree<K, V>::each_if(self, it: <var-name>, cond: <expr>) -> Iter<it = (K, V)>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_each_if(self, it, cond)                                                             \
  u_tree_each (self, it)                                                                           \
    if (cond)

/**
 * ::Tree<K, V>::each_ref(self, it: <var-name>) -> Iter<it = (const K, const V)*>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_each_ref(self, it)                                                                  \
  {                                                                                                \
    extern pub any_t __u_tree_each(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_tree_each((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_ref_t)){}; ({                                           \
         extern pub any_t __u_tree_each(any_t, bool);                                              \
                                                                                                   \
         it = __u_tree_each((self)->ref, !!0);                                                     \
       });)

/**
 * ::Tree<K, V>::each_if_ref(self, it: <var-name>, cond: <expr>) -> Iter<it = (const K, const V)*>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_each_if_ref(self, it, cond)                                                         \
  u_tree_each_ref (self, it)                                                                       \
    if (cond)

/**
 * ::Tree<K, V>::each_mut(self, it: <var-name>) -> Iter<it = (const K, V)*>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_each_mut(self, it)                                                                  \
  {                                                                                                \
    extern pub any_t __u_tree_each(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_tree_each((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_mut_t)){}; ({                                           \
         extern pub any_t __u_tree_each(any_t, bool);                                              \
                                                                                                   \
         it = __u_tree_each((self)->ref, !!0);                                                     \
       });)

/**
 * ::Tree<K, V>::each_if_mut(self, it: <var-name>, cond: <expr>) -> Iter<it = (const K, V)*>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_each_if_mut(self, it, cond)                                                         \
  u_tree_each_mut (self, it)                                                                       \
    if (cond)

/**
 * ::Tree<K, V>::reach(self, it: <var-name>) -> Iter<it = (K, V)>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_reach(self, it)                                                                     \
  {                                                                                                \
    extern pub any_t __u_tree_reach(any_t, bool);                                                  \
                                                                                                   \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_tree_reach((self)->ref, !0);                                                         \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_t)){}; ({                                               \
         extern pub any_t __u_tree_reach(any_t, bool);                                             \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.each_mut_t) __tuple_mut__ = __u_tree_reach((self)->ref, !!0);                    \
                                                                                                   \
         if (__tuple_mut__) {                                                                      \
           it.key = __tuple_mut__->key;                                                            \
           it.val = __tuple_mut__->val;                                                            \
         }                                                                                         \
                                                                                                   \
         __tuple_mut__;                                                                            \
       });)

/**
 * ::Tree<K, V>::reach_if(self, it: <var-name>, cond: <expr>) -> Iter<it = (K, V)>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_reach_if(self, it, cond) u_tree_reach(self, it) if (cond)

/**
 * ::Tree<K, V>::reach_ref(self, it: <var-name>) -> Iter<it = (const K, const V)*>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_reach_ref(self, it)                                                                 \
  {                                                                                                \
    extern pub any_t __u_tree_reach(any_t, bool);                                                  \
                                                                                                   \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_tree_reach((self)->ref, !0);                                                         \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_ref_t)){}; ({                                           \
         extern pub any_t __u_tree_reach(any_t, bool);                                             \
                                                                                                   \
         it = __u_tree_reach((self)->ref, !!0);                                                    \
       });)

/**
 * ::Tree<K, V>::reach_if_ref(self, it: <var-name>, cond: <expr>) -> Iter<it = (const K, const V)*>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_reach_if_ref(self, it, cond) u_tree_reach_ref(self, it) if (cond)

/**
 * ::Tree<K, V>::reach_mut(self, it: <var-name>) -> Iter<it = (const K, V)*>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_reach_mut(self, it)                                                                 \
  {                                                                                                \
    extern pub any_t __u_tree_reach(any_t, bool);                                                  \
                                                                                                   \
    typecheck(u_tree_meta_t, (self)->_[0].meta, "meta type not's Tree<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_tree_reach((self)->ref, !0);                                                         \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_mut_t)){}; ({                                           \
         extern pub any_t __u_tree_reach(any_t, bool);                                             \
                                                                                                   \
         it = __u_tree_reach((self)->ref, !!0);                                                    \
       });)

/**
 * ::Tree<K, V>::reach_if_mut(self, it: <var-name>, cond: <expr>) -> Iter<it = (const K, V)*>
 *
 * 迭代游标存于 Tree 内部, 同一 Tree 不支持嵌套迭代
 */
#define u_tree_reach_if_mut(self, it, cond) u_tree_reach_mut(self, it) if (cond)

/**
 * ::Tree<K, V>::map_by(self, proc: <block>) -> Self
 *
 * 在 proc 中修改 it 实现转换, 块返回值忽略; 若修改 key 需保证唯一, 重复触发断言
 */
#define u_tree_map_by(self, proc)                                                                  \
  ({                                                                                               \
    extern pub u_cmp_fn __u_tree_cmp_fn(any_t);                                                    \
                                                                                                   \
    typeof(self) __self__ = u_tree_new(__self__, __u_tree_cmp_fn((self)->ref));                    \
                                                                                                   \
    u_tree_each (self, it) {                                                                       \
      proc;                                                                                        \
                                                                                                   \
      u_tree_insert(__self__, it.key, it.val);                                                     \
    }                                                                                              \
                                                                                                   \
    __self__;                                                                                      \
  })

/**
 * ::Tree<K, V>::filter_if(self, cond: <expr>) -> Self
 */
#define u_tree_filter_if(self, cond)                                                               \
  ({                                                                                               \
    extern pub u_cmp_fn __u_tree_cmp_fn(any_t);                                                    \
                                                                                                   \
    typeof(self) __self__ = u_tree_new(__self__, __u_tree_cmp_fn((self)->ref));                    \
                                                                                                   \
    u_tree_each_if (self, it, cond) {                                                              \
      u_tree_insert(__self__, it.key, it.val);                                                     \
    }                                                                                              \
                                                                                                   \
    __self__;                                                                                      \
  })

/**
 * ::Tree<K, V>::filter_if_ref(self, cond: <expr>) -> Self
 */
#define u_tree_filter_if_ref(self, cond)                                                           \
  ({                                                                                               \
    extern pub u_cmp_fn __u_tree_cmp_fn(any_t);                                                    \
                                                                                                   \
    typeof(self) __self__ = u_tree_new(__self__, __u_tree_cmp_fn((self)->ref));                    \
                                                                                                   \
    u_tree_each_if_ref (self, it, cond) {                                                          \
      u_tree_insert(__self__, it->key, it->val);                                                   \
    }                                                                                              \
                                                                                                   \
    __self__;                                                                                      \
  })

/**
 * ::Tree<K, V>::all_if(self, cond: <expr>) -> bool
 */
#define u_tree_all_if(self, cond)                                                                  \
  ({                                                                                               \
    bool __result__ = !0;                                                                          \
                                                                                                   \
    u_tree_each_if (self, it, !(cond)) {                                                           \
      __result__ = !!0;                                                                            \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Tree<K, V>::all_if_ref(self, cond: <expr>) -> bool
 */
#define u_tree_all_if_ref(self, cond)                                                              \
  ({                                                                                               \
    bool __result__ = !0;                                                                          \
                                                                                                   \
    u_tree_each_if_ref (self, it, !(cond)) {                                                       \
      __result__ = !!0;                                                                            \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Tree<K, V>::any_if(self, cond: <expr>) -> bool
 */
#define u_tree_any_if(self, cond)                                                                  \
  ({                                                                                               \
    bool __result__ = !!0;                                                                         \
                                                                                                   \
    u_tree_each_if (self, it, cond) {                                                              \
      __result__ = !0;                                                                             \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Tree<K, V>::any_if_ref(self, cond: <expr>) -> bool
 */
#define u_tree_any_if_ref(self, cond)                                                              \
  ({                                                                                               \
    bool __result__ = !!0;                                                                         \
                                                                                                   \
    u_tree_each_if_ref (self, it, cond) {                                                          \
      __result__ = !0;                                                                             \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_ITREE_H__ */
