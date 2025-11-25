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

#ifndef U_IDICT_H__
#  define U_IDICT_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/* clang-format off */

/***************************************************************************************************
 * Api
 **************************************************************************************************/
typedef struct {}* $dict_t;

extern any_t $dict_new      (i32_t, i32_t, u_hash_fn);
extern void  $dict_clear    (any_t);
extern void  $dict_cleanup  (any_t);
extern any_t $dict_at       (any_t, any_t);
extern void  $dict_del      (any_t, any_t);
extern any_t $dict_add      (any_t, any_t);
extern any_t $dict_each     (any_t, bool);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_dict_t(K, V)                                                                           \
    typeof(const struct [[gnu::packed]] {                                                          \
      any_t ref;                                                                                   \
      int len;                                                                                     \
                                                                                                   \
      struct {                                                                                     \
        $dict_t meta;                                                                              \
        K key;                                                                                     \
        V val;                                                                                     \
              struct { K key; V val; }   pair;                                                     \
        const struct { K key; V val; } * ref;                                                      \
              struct { K key; V val; } * mut;                                                      \
      } _[0]; /* Don't use this field. */                                                          \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_dict_new(self, ...)                                                                    \
    ({                                                                                             \
      typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                           \
                                                                                                   \
      self = $dict_new(sizeof(self->_[0].key), sizeof(self->_[0].val), u_va_0th(0, __VA_ARGS__));  \
                                                                                                   \
      self->ref;                                                                                   \
    })


#  define u_dict_is_valid(self, _key)                                                              \
    ({                                                                                             \
      typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].pair) __pair__ = {_key};                                            \
                                                                                                   \
      (bool) (nullptr != $dict_at(self->ref, &__pair__.key));                                      \
    })


#  define u_dict_clear(self, ...)                                                                  \
    do {                                                                                           \
      typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                           \
                                                                                                   \
      u_va_has_if(__VA_ARGS__) ( u_dict_each(self, it) { __VA_ARGS__ } )                           \
                                                                                                   \
      $dict_clear(self->ref);                                                                      \
    } while (0)


#  define u_dict_cleanup(self, ...)                                                                \
    do {                                                                                           \
      typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                           \
                                                                                                   \
      u_va_has_if(__VA_ARGS__) ( u_dict_each(self, it) { __VA_ARGS__ } )                           \
                                                                                                   \
      $dict_cleanup(self->ref);                                                                    \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)


#  define u_dict_at(self, _key, ...)                                                               \
    ({                                                                                             \
      *u_dict_at_mut(self, _key) u_va_has_if(__VA_ARGS__) ( = u_va_at(0, __VA_ARGS__) );           \
    })


#  define u_dict_at_ref(self, _key)                                                                \
    ({                                                                                             \
      typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].key) __key__ = _key;                                                \
      typeof_unqual(self->_[0].mut) __ref__ = $dict_at(self->ref, (any_t)&__key__);                \
                                                                                                   \
      assert(__ref__);                                                                             \
                                                                                                   \
      &__ref__->val;                                                                               \
    })


#  define u_dict_at_mut(self, _key)                                                                \
    ({                                                                                             \
      typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].key) __key__ = _key;                                                \
      typeof_unqual(self->_[0].mut) __mut__ = $dict_at(self->ref, (any_t)&__key__);                \
                                                                                                   \
      assert(__mut__);                                                                             \
                                                                                                   \
      &__mut__->val;                                                                               \
    })


#  define u_dict_try_at(self, _key, ...)                                                           \
    u_va_let(typeof_unqual(self->_[0].key), key, _key);                                            \
    u_va_let(typeof_unqual(self->_[0].ref), ref, $dict_at(self->ref, (any_t)&u_va_var(key)));      \
    if (u_va_var(ref))                                                                             \
      for (auto u_va_0th(it, __VA_ARGS__) = u_va_var(ref)->val; u_va_var(ref); u_va_var(ref) = nullptr)


#  define u_dict_try_at_ref(self, ...)                                                             \
    u_va_let(typeof_unqual(self->_[0].key), key, _key);                                            \
    u_va_let(typeof_unqual(self->_[0].ref), ref, $dict_at(self->ref, (any_t)&u_va_var(key)));      \
    if (u_va_var(ref))                                                                             \
      for (auto u_va_0th(it, __VA_ARGS__) = &u_va_var(ref)->val; u_va_var(ref); u_va_var(ref) = nullptr)


#  define u_dict_try_at_mut(self, ...)                                                             \
    u_va_let(typeof_unqual(self->_[0].key), key, _key);                                            \
    u_va_let(typeof_unqual(self->_[0].mut), mut, $dict_at(self->ref, (any_t)&u_va_var(key)));      \
    if (u_va_var(mut))                                                                             \
      for (auto u_va_0th(it, __VA_ARGS__) = &u_va_var(mut)->val; u_va_var(mut); u_va_var(mut) = nullptr)


#  define u_dict_remove(self, _key)                                                                \
    ({                                                                                             \
      typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].pair) __pair__ = {_key};                                            \
      typeof_unqual(self->_[0].ref) __ref__ = $dict_at(self->ref, &__pair__.key);                  \
      assert(__ref__);                                                                             \
                                                                                                   \
      __pair__.val = __ref__->val;                                                                 \
      $dict_del(self->ref, (any_t)__ref__);                                                        \
                                                                                                   \
      __pair__.val;                                                                                \
    })


#  define u_dict_insert(self, _key, _val)                                                          \
    do {                                                                                           \
      typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                           \
                                                                                                   \
      typeof_unqual(self->_[0].pair) __pair__ = {_key, _val};                                      \
      typeof_unqual(self->_[0].mut) __mut__ = $dict_add(self->ref, &__pair__.key);                 \
      assert(__mut__);                                                                             \
                                                                                                   \
      __mut__->key = __pair__.key;                                                                 \
      __mut__->val = __pair__.val;                                                                 \
    } while (0)


#  define u_dict_each(self, it)                                                                    \
    typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                             \
                                                                                                   \
    (void)$dict_each(self->ref, !0);                                                               \
    for (auto it = (typeof(self->_[0].pair)) {}; ({ typeof(it)* __ref__ = $dict_each(self->ref, !!0); if (__ref__) it = *__ref__; __ref__; }); )


#  define u_dict_each_if(self, it, cond) u_dict_each(self, it) if (cond)


#  define u_dict_each_ref(self, it)                                                                \
    typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                             \
                                                                                                   \
    (void)$dict_each(self->ref, !0);                                                               \
    for (auto it = (typeof(self->_[0].ref)) {}; (it = $dict_each(self->ref, !!0)); )


#  define u_dict_each_if_ref(self, it, cond) u_dict_each_ref(self, it) if (cond)


#  define u_dict_each_mut(self, it)                                                                \
    typecheck($dict_t, self->_[0].meta, "meta type not's Dict<K, V>");                             \
                                                                                                   \
    (void)$dict_each(self->ref, !0);                                                               \
    for (typeof(struct { typeof(self->_[0].key) key; typeof_unqual(self->_[0].val) val; }*) it = {}; (it = $dict_each(self->ref, !!0)); )


#  define u_dict_each_if_mut(self, it, cond) u_dict_each_mut(self, it) if (cond)

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IDICT_H__ */
