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
#define u_dict_t(K, V)                                                                             \
  typeof(const struct [[gnu::packed]] {                                                            \
    any_t ref;                                                                                     \
    int len;                                                                                       \
                                                                                                   \
    struct {                                                                                       \
      u_dict_meta_t meta;                                                                          \
                                                                                                   \
      K key;                                                                                       \
      K* kmut;                                                                                     \
      const K* kref;                                                                               \
                                                                                                   \
      V val;                                                                                       \
      V* vmut;                                                                                     \
      const V* vref;                                                                               \
                                                                                                   \
      struct {                                                                                     \
        K key;                                                                                     \
        V val;                                                                                     \
      } pair;                                                                                      \
                                                                                                   \
      struct {                                                                                     \
        const K key;                                                                               \
        const V val;                                                                               \
      }* ref;                                                                                      \
                                                                                                   \
      struct {                                                                                     \
        const K key;                                                                               \
        V val;                                                                                     \
      }* mut;                                                                                      \
    } _[0]; /* Don't use this field. */                                                            \
  }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#define u_dict_new(self, ...)                                                                      \
  ({                                                                                               \
    extern pub any_t __u_dict_new(i32_t, i32_t, u_hash_fn);                                        \
                                                                                                   \
    {                                                                                              \
      typecheck(u_dict_meta_t, (self)->_->meta, "meta type not's Dict<K, V>");                     \
    }                                                                                              \
                                                                                                   \
    (self) = __u_dict_new(sizeof((self)->_[0].key),                                                \
                          sizeof((self)->_[0].val),                                                \
                          u_va_0th(NULL, __VA_ARGS__));                                            \
                                                                                                   \
    (self)->ref;                                                                                   \
  })

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
    u_va_has_if(__VA_ARGS__)(u_dict_each (self, it){__VA_ARGS__});                                 \
                                                                                                   \
    __u_dict_clear((self)->ref);                                                                   \
  } while (0)

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
    u_va_has_if(__VA_ARGS__)(u_dict_each (self, it){__VA_ARGS__});                                 \
                                                                                                   \
    __u_dict_cleanup((self)->ref);                                                                 \
                                                                                                   \
    (self) = NULL;                                                                                 \
  } while (0)

#define u_dict_at(self, _key, ...)                                                                 \
  ({                                                                                               \
    ;                                                                                              \
    *u_dict_at_mut(self, _key) u_va_has_if(__VA_ARGS__)(= u_va_at(0, __VA_ARGS__));                \
  })

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
    typeof((self)->_->key) __key__   = _key;                                                       \
    typeof((self)->_->ref) __ref__   = __u_dict_at((self)->ref, (any_t) & __key__);                \
    typeof((self)->_->vref) __vref__ = __ref__ ? &__ref__->val : NULL;                             \
                                                                                                   \
    __vref__;                                                                                      \
  })

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
    typeof((self)->_->key) __key__   = _key;                                                       \
    typeof((self)->_->mut) __mut__   = __u_dict_at((self)->ref, (any_t) & __key__);                \
    typeof((self)->_->vmut) __vmut__ = __mut__ ? &__mut__->val : NULL;                             \
                                                                                                   \
    __vmut__;                                                                                      \
  })

#define u_dict_try_at(self, _key, ...)                                                             \
  {                                                                                                \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_->val)){}; ({                             \
         extern pub any_t __u_dict_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_->key) __key__ = _key;                                                    \
         typeof((self)->_->ref) __ref__ = __u_dict_at((self)->ref, (any_t) & __key__);             \
                                                                                                   \
         if (__ref__)                                                                              \
           u_va_0th(it, __VA_ARGS__) = __ref__->val;                                               \
                                                                                                   \
         __ref__;                                                                                  \
       });                                                                                         \
       ({ break; }))

#define u_dict_try_at_ref(self, ...)                                                               \
  {                                                                                                \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_->vref)){}; ({                            \
         extern pub any_t __u_dict_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_->key) __key__ = _key;                                                    \
         typeof((self)->_->ref) __ref__ = __u_dict_at((self)->ref, (any_t) & __key__);             \
                                                                                                   \
         if (__ref__)                                                                              \
           u_va_0th(it, __VA_ARGS__) = &__ref__->val;                                              \
                                                                                                   \
         u_va_0th(it, __VA_ARGS__);                                                                \
       });                                                                                         \
       ({ break; }))

#define u_dict_try_at_mut(self, ...)                                                               \
  {                                                                                                \
    typecheck(u_dict_meta_t, (self)->_[0].meta, "meta type not's Dict<K, V>");                     \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto u_va_0th(it, __VA_ARGS__) = (typeof((self)->_->vmut)){}; ({                            \
         extern pub any_t __u_dict_at(any_t, any_t);                                               \
                                                                                                   \
         typeof((self)->_->key) __key__ = _key;                                                    \
         typeof((self)->_->mut) __mut__ = __u_dict_at((self)->ref, (any_t) & __key__);             \
                                                                                                   \
         if (__mut__)                                                                              \
           u_va_0th(it, __VA_ARGS__) = &__mut__->val;                                              \
                                                                                                   \
         u_va_0th(it, __VA_ARGS__);                                                                \
       });                                                                                         \
       ({ break; }))

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
    typeof((self)->_->key) __key__   = _key;                                                       \
    typeof((self)->_[0].mut) __mut__ = __u_dict_at((self)->ref, (any_t) & __key__);                \
                                                                                                   \
    auto __val__ = (typeof((self)->_->val)){};                                                     \
    if (__mut__) {                                                                                 \
      __val__ = __mut__->val;                                                                      \
      __u_dict_del((self)->ref, (any_t) & __key__);                                                \
    }                                                                                              \
                                                                                                   \
    __val__;                                                                                       \
  })

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
    typeof((self)->_->key) __key__ = _key;                                                         \
    typeof((self)->_->val) __val__ = _val;                                                         \
    typeof((self)->_->mut) __mut__ = __u_dict_add((self)->ref, (any_t) & __key__);                 \
                                                                                                   \
    if (__mut__) {                                                                                 \
      *(typeof((self)->_->kmut))(&__mut__->key) = __key__;                                         \
      __mut__->val                              = __val__;                                         \
    }                                                                                              \
  } while (0)

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
  for (auto it = (typeof((self)->_->pair)){}; ({                                                   \
         extern pub any_t __u_dict_each(any_t, bool);                                              \
                                                                                                   \
         typeof((self)->_->ref) __ref__ = __u_dict_each((self)->ref, !!0);                         \
                                                                                                   \
         if (__ref__) {                                                                            \
           it.key = __ref__->key;                                                                  \
           it.val = __ref__->val;                                                                  \
         }                                                                                         \
                                                                                                   \
         __ref__;                                                                                  \
       });)

#define u_dict_each_if(self, it, cond)                                                             \
  u_dict_each (self, it)                                                                           \
    if (cond)

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
  for (auto it = (typeof((self)->_->ref)){}; ({                                                    \
         extern pub any_t __u_dict_each(any_t, bool);                                              \
                                                                                                   \
         it = __u_dict_each((self)->ref, !!0);                                                     \
       });)

#define u_dict_each_if_ref(self, it, cond)                                                         \
  u_dict_each_ref (self, it)                                                                       \
    if (cond)

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
  for (auto it = (typeof((self)->_->mut)){}; ({                                                    \
         extern pub any_t __u_dict_each(any_t, bool);                                              \
                                                                                                   \
         it = __u_dict_each((self)->ref, !!0);                                                     \
       });)

#define u_dict_each_if_mut(self, it, cond)                                                         \
  u_dict_each_mut (self, it)                                                                       \
    if (cond)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_IDICT_H__ */
