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

#ifndef U_IDICT_H__
#define U_IDICT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

/***************************************************************************************************
 * Meta Type
 **************************************************************************************************/
typedef struct {
} u_dict_meta_t;

/***************************************************************************************************
 * iType
 **************************************************************************************************/

/**
 * ::Class Dict<K, V>
 */
#define u_dict_t(K, V)                                                                             \
  typeof(const struct [[gnu::packed]] {                                                            \
    any_t ref;                                                                                     \
    int len;                                                                                       \
                                                                                                   \
    struct {                                                                                       \
      u_dict_meta_t meta;                                                                          \
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
 * ::Dict<K, V>::new(self) -> Self
 * ::Dict<K, V>::new(self, hash_fn) -> Self
 */
#define u_dict_new(self, ...)                                                                      \
  ({                                                                                               \
    extern pub any_t __u_dict_new(i32_t, i32_t, u_hash_fn);                                        \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                   \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    (self) = __u_dict_new(sizeof(M.new_t.key), sizeof(M.new_t.val), u_va_0th(NULL, __VA_ARGS__));  \
                                                                                                   \
    (self)->ref;                                                                                   \
  })

/**
 * ::Dict<K, V>::clear(self) -> !
 * ::Dict<K, V>::clear(self, proc: <block>) -> !
 */
#define u_dict_clear(self, ...)                                                                    \
  do {                                                                                             \
    extern pub void __u_dict_clear(any_t);                                                         \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    u_va_has_if(__VA_ARGS__)(u_dict_each (self, it) { __VA_ARGS__; });                             \
                                                                                                   \
    __u_dict_clear((self)->ref);                                                                   \
  } while (0)

/**
 * ::Dict<K, V>::cleanup(self) -> !
 * ::Dict<K, V>::cleanup(self, proc: <block>) -> !
 */
#define u_dict_cleanup(self, ...)                                                                  \
  do {                                                                                             \
    extern pub void __u_dict_cleanup(any_t);                                                       \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    u_va_has_if(__VA_ARGS__)(u_dict_each (self, it) { __VA_ARGS__; });                             \
                                                                                                   \
    __u_dict_cleanup((self)->ref);                                                                 \
                                                                                                   \
    (self) = NULL;                                                                                 \
  } while (0)

/**
 * ::Dict<K, V>::at(self, key: K) -> V
 * ::Dict<K, V>::at(self, key: K, val: V) -> V
 *
 * Key 必须存在, 未命中为空指针解引用
 */
#define u_dict_at(self, _key, ...)                                                                 \
  ({                                                                                               \
    extern pub any_t __u_dict_at(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __tuple__         = {_key, __VA_ARGS__};                                        \
    typeof(M.at_mut_t) __tuple_mut__ = __u_dict_at((self)->ref, (any_t) & __tuple__.key);          \
    assert(__tuple_mut__);                                                                          \
                                                                                                   \
    __tuple_mut__->val u_va_has_if(__VA_ARGS__)(= __tuple__.val);                                  \
  })

/**
 * ::Dict<K, V>::at_ref(self, key: K) -> const V*
 *
 * 未命中返回 NULL
 */
#define u_dict_at_ref(self, _key)                                                                  \
  ({                                                                                               \
    extern pub any_t __u_dict_at(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __tuple__         = {_key};                                                     \
    typeof(M.at_ref_t) __tuple_ref__ = __u_dict_at((self)->ref, (any_t) & __tuple__.key);          \
                                                                                                   \
    __tuple_ref__ ? &__tuple_ref__->val : NULL;                                                    \
  })

/**
 * ::Dict<K, V>::at_mut(self, key: K) -> V*
 *
 * 未命中返回 NULL
 */
#define u_dict_at_mut(self, _key)                                                                  \
  ({                                                                                               \
    extern pub any_t __u_dict_at(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __tuple__         = {_key};                                                     \
    typeof(M.at_mut_t) __tuple_mut__ = __u_dict_at((self)->ref, (any_t) & __tuple__.key);          \
                                                                                                   \
    __tuple_mut__ ? &__tuple_mut__->val : NULL;                                                    \
  })

/**
 * ::Dict<K, V>::try_at(self, key: K) -> Option<it = V>
 * ::Dict<K, V>::try_at(self, key: K, <var-name>) -> Option<it = V>
 */
#define u_dict_try_at(self, _key, ...)                                                             \
  {                                                                                                \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (int __once__ = 1; __once__; __once__ = 0)                                                   \
    for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_[0].val_t)){}; __once__ && ({           \
         extern pub any_t __u_dict_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.at_t) __tuple__         = {_key};                                                \
         typeof(M.at_ref_t) __tuple_ref__ = __u_dict_at((self)->ref, (any_t) & __tuple__.key);     \
                                                                                                   \
         if (__tuple_ref__)                                                                        \
           u_va_0th(it, __VA_ARGS__) = __tuple_ref__->val;                                         \
                                                                                                   \
         __tuple_ref__;                                                                            \
       });                                                                                         \
       __once__ = 0)

/**
 * ::Dict<K, V>::try_at_ref(self, key: K) -> Option<it = const V*>
 * ::Dict<K, V>::try_at_ref(self, key: K, <var-name>) -> Option<it = const V*>
 */
#define u_dict_try_at_ref(self, _key, ...)                                                         \
  {                                                                                                \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (int __once__ = 1; __once__; __once__ = 0)                                                   \
    for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_[0].val_ref_t)){}; __once__ && ({       \
         extern pub any_t __u_dict_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.at_t) __tuple__         = {_key};                                                \
         typeof(M.at_ref_t) __tuple_ref__ = __u_dict_at((self)->ref, (any_t) & __tuple__.key);     \
                                                                                                   \
         if (__tuple_ref__)                                                                        \
           u_va_0th(it, __VA_ARGS__) = &__tuple_ref__->val;                                        \
                                                                                                   \
         __tuple_ref__;                                                                            \
       });                                                                                         \
       __once__ = 0)

/**
 * ::Dict<K, V>::try_at_mut(self, key: K) -> Option<it = V*>
 * ::Dict<K, V>::try_at_mut(self, key: K, <var-name>) -> Option<it = V*>
 */
#define u_dict_try_at_mut(self, _key, ...)                                                         \
  {                                                                                                \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (int __once__ = 1; __once__; __once__ = 0)                                                   \
    for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_[0].val_mut_t)){}; __once__ && ({       \
         extern pub any_t __u_dict_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.at_t) __tuple__         = {_key};                                                \
         typeof(M.at_mut_t) __tuple_mut__ = __u_dict_at((self)->ref, (any_t) & __tuple__.key);     \
                                                                                                   \
         if (__tuple_mut__)                                                                        \
           u_va_0th(it, __VA_ARGS__) = &__tuple_mut__->val;                                        \
                                                                                                   \
         __tuple_mut__;                                                                            \
       });                                                                                         \
       __once__ = 0)

/**
 * ::Dict<K, V>::remove(self, key: K) -> V
 *
 * 返回被移除的值, 未命中返回零值
 */
#define u_dict_remove(self, _key)                                                                  \
  ({                                                                                               \
    extern pub any_t __u_dict_at(any_t, any_t);                                                    \
    extern pub void __u_dict_del(any_t, any_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.remove_t) __tuple__     = {_key};                                                     \
    typeof(M.at_mut_t) __tuple_mut__ = __u_dict_at((self)->ref, (any_t) & __tuple__.key);          \
                                                                                                   \
    auto __val__ = (typeof((self)->_[0].val_t)){};                                                 \
    if (__tuple_mut__) {                                                                           \
      __val__ = __tuple_mut__->val;                                                                \
      __u_dict_del((self)->ref, (any_t) & __tuple__.key);                                          \
    }                                                                                              \
                                                                                                   \
    __val__;                                                                                       \
  })

/**
 * ::Dict<K, V>::insert(self, key: K, val: V) -> !
 *
 * 重复 Key 静默忽略
 */
#define u_dict_insert(self, _key, _val)                                                            \
  do {                                                                                             \
    extern pub any_t __u_dict_add(any_t, any_t);                                                   \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.insert_t) __tuple__ = {_key, _val};                                                   \
                                                                                                   \
    typeof(M.insert_mut_t) __tuple_mut__ = __u_dict_add((self)->ref, (any_t) & __tuple__.key);     \
                                                                                                   \
    if (__tuple_mut__) {                                                                           \
      __tuple_mut__->key = __tuple__.key;                                                          \
      __tuple_mut__->val = __tuple__.val;                                                          \
    }                                                                                              \
  } while (0)

/**
 * ::Dict<K, V>::each(self, it: <var-name>) -> Iter<it = (K, V)>
 *
 * 迭代游标存于 Dict 内部, 同一 Dict 不支持嵌套迭代
 */
#define u_dict_each(self, it)                                                                      \
  {                                                                                                \
    extern pub any_t __u_dict_each(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_dict_each((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_t)){}; ({                                               \
         extern pub any_t __u_dict_each(any_t, bool);                                              \
                                                                                                   \
         typeof((self)->_[0]) M = {};                                                              \
                                                                                                   \
         typeof(M.each_mut_t) __tuple_mut__ = __u_dict_each((self)->ref, !!0);                     \
                                                                                                   \
         if (__tuple_mut__) {                                                                      \
           it.key = __tuple_mut__->key;                                                            \
           it.val = __tuple_mut__->val;                                                            \
         }                                                                                         \
                                                                                                   \
         __tuple_mut__;                                                                            \
       });)

/**
 * ::Dict<K, V>::each_if(self, it: <var-name>, cond: <expr>) -> Iter<it = (K, V)>
 *
 * 迭代游标存于 Dict 内部, 同一 Dict 不支持嵌套迭代
 */
#define u_dict_each_if(self, it, cond)                                                             \
  u_dict_each (self, it)                                                                           \
    if (cond)

/**
 * ::Dict<K, V>::each_ref(self, it: <var-name>) -> Iter<it = (const K, const V)*>
 *
 * 迭代游标存于 Dict 内部, 同一 Dict 不支持嵌套迭代
 */
#define u_dict_each_ref(self, it)                                                                  \
  {                                                                                                \
    extern pub any_t __u_dict_each(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_dict_each((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_ref_t)){}; ({                                           \
         extern pub any_t __u_dict_each(any_t, bool);                                              \
                                                                                                   \
         it = __u_dict_each((self)->ref, !!0);                                                     \
       });)

/**
 * ::Dict<K, V>::each_if_ref(self, it: <var-name>, cond: <expr>) -> Iter<it = (const K, const V)*>
 *
 * 迭代游标存于 Dict 内部, 同一 Dict 不支持嵌套迭代
 */
#define u_dict_each_if_ref(self, it, cond)                                                         \
  u_dict_each_ref (self, it)                                                                       \
    if (cond)

/**
 * ::Dict<K, V>::each_mut(self, it: <var-name>) -> Iter<it = (const K, V)*>
 *
 * 迭代游标存于 Dict 内部, 同一 Dict 不支持嵌套迭代
 */
#define u_dict_each_mut(self, it)                                                                  \
  {                                                                                                \
    extern pub any_t __u_dict_each(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_dict_each((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].each_mut_t)){}; ({                                           \
         extern pub any_t __u_dict_each(any_t, bool);                                              \
                                                                                                   \
         it = __u_dict_each((self)->ref, !!0);                                                     \
       });)

/**
 * ::Dict<K, V>::each_if_mut(self, it: <var-name>, cond: <expr>) -> Iter<it = (const K, V)*>
 *
 * 迭代游标存于 Dict 内部, 同一 Dict 不支持嵌套迭代
 */
#define u_dict_each_if_mut(self, it, cond)                                                         \
  u_dict_each_mut (self, it)                                                                       \
    if (cond)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_IDICT_H__ */
