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

#ifndef U_IAVL_H__
#  define U_IAVL_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* __u_tree_t;

/***************************************************************************************************
 * Api
 ***************************************************************************************************/
extern any_t avl_new(size_t, size_t, u_cmp_fn);

extern size_t avl_len(any_t);

extern bool avl_exist(any_t, any_t);

extern void avl_clear(any_t);

extern void avl_cleanup(any_t);

extern any_t avl_at(any_t, any_t);

extern void avl_pop(any_t, any_t, any_t);

extern void avl_put(any_t, any_t, any_t);

extern u_cmp_fn avl_fn(any_t);

extern bool avl_for_init(any_t, bool);

extern void avl_for_end(any_t);

extern bool avl_for(any_t, any_t, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_tree_t(...) typeof(__u_tree_t(*)(__VA_ARGS__))

#  define __u_tree_defs(K, V)                                                                      \
    u_tree_t(K, V) :                                                                               \
        (struct {                                                                                  \
          K k;                                                                                     \
          V v;                                                                                     \
        }) {                                                                                       \
    }

#  define _u_tree_defs(arg) __u_tree_defs arg

#  define u_tree_type(u, arg)     typeof(_Generic(u, u_tree_defs).arg)
#  define u_tree_type_val(u, arg) _Generic(u, u_tree_defs).arg

#  if defined(NDEBUG)
#    define u_tree_type_check(u)
#  else
#    define u_tree_type_check(u)                                                                   \
      static_assert(typeeq((__u_tree_t){}, u(u_tree_type_val(u, k), u_tree_type_val(u, v))))
#  endif

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#  define u_tree_init(u, fn)                                                                       \
    do {                                                                                           \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      u = avl_new(sizeof(u_tree_type(u, k)), sizeof(u_tree_type(u, v)), fn);                       \
    } while (0)

#  define u_tree_new(...)                                                                          \
    ({                                                                                             \
      u_tree_t(va_at(0, __VA_ARGS__), va_at(1, __VA_ARGS__)) u = nullptr;                          \
                                                                                                   \
      u_tree_init(u, va_at(2, __VA_ARGS__));                                                       \
                                                                                                   \
      u;                                                                                           \
    })

#  define u_tree_len(u)                                                                            \
    ({                                                                                             \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      avl_len(u);                                                                                  \
    })

#  define u_tree_is_empty(u)                                                                       \
    ({                                                                                             \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      0 == avl_len(u);                                                                             \
    })

#  define u_tree_is_exist(u, _k)                                                                   \
    ({                                                                                             \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      u_tree_type(u, k) _a = _k;                                                                   \
      u_tree_type(u, v) _b = {};                                                                   \
                                                                                                   \
      avl_exist(u, &_a);                                                                           \
    })

#  define u_tree_clear(u)                                                                          \
    do {                                                                                           \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      avl_clear(u);                                                                                \
    } while (0)

#  define u_tree_cleanup(u)                                                                        \
    do {                                                                                           \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      avl_cleanup(u);                                                                              \
                                                                                                   \
      u = nullptr;                                                                                 \
    } while (0)

/* clang-format off */
#  define u_tree_at(u, _k, ...)                                                                    \
    va_elseif(va_size_is(1, __VA_ARGS__)) (                                                        \
      ({                                                                                           \
        u_tree_type_check(u);                                                                      \
                                                                                                   \
        bool _ret         = false;                                                                 \
        u_tree_type(u, k) _a  = _k;                                                                \
        u_tree_type(u, v)* _b = {};                                                                \
                                                                                                   \
        _b = avl_at(u, &_a);                                                                       \
                                                                                                   \
        if (_b != nullptr) {                                                                       \
          *_b = va_at(0, __VA_ARGS__);                                                             \
          _ret = true;                                                                             \
        }                                                                                          \
                                                                                                   \
        _ret;                                                                                      \
      })                                                                                           \
    ) (                                                                                            \
      ({                                                                                           \
        u_tree_type_check(u);                                                                      \
                                                                                                   \
        u_tree_type(u, k) _a  = _k;                                                                \
        u_tree_type(u, v) _it = {};                                                                \
        u_tree_type(u, v)* _b = {};                                                                \
                                                                                                   \
        _b = avl_at(u, &_a);                                                                       \
                                                                                                   \
        if (_b == nullptr) {                                                                       \
          _b = &_it;                                                                               \
        }                                                                                          \
                                                                                                   \
        *_b;                                                                                       \
      })                                                                                           \
    )
/* clang-format on */

#  define u_tree_try(u, _k)                                                                        \
    for (u_tree_type(u, v)* it = avl_at(u, &(u_tree_type(u, k)){_k}); it != nullptr; it = nullptr)

#  define u_tree_pop(u, _k)                                                                        \
    ({                                                                                             \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      u_tree_type(u, k) _a = _k;                                                                   \
      u_tree_type(u, v) _b = {};                                                                   \
                                                                                                   \
      avl_pop(u, &_a, &_b);                                                                        \
                                                                                                   \
      _b;                                                                                          \
    })

#  define u_tree_put(u, _k, _v)                                                                    \
    do {                                                                                           \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      u_tree_type(u, k) _a = _k;                                                                   \
      u_tree_type(u, v) _b = _v;                                                                   \
                                                                                                   \
      avl_put(u, &_a, &_b);                                                                        \
    } while (0)

#  define u_tree_fn(u, _k1, _k2)                                                                   \
    ({                                                                                             \
      u_tree_type_check(u);                                                                        \
                                                                                                   \
      u_tree_type(u, k) _a = _k1;                                                                  \
      u_tree_type(u, k) _b = _k2;                                                                  \
                                                                                                   \
      avl_fn(u)(&_a, &_b);                                                                         \
    })

#  define u_tree_for(u, _k, _v)                                                                    \
    for (u_tree_type(u, k) _k = {}; avl_for_init(u, 1); avl_for_end(u))                            \
      for (u_tree_type(u, v) _v = {}; avl_for(u, &_k, &_v);)

#  define u_tree_rfor(u, _k, _v)                                                                   \
    for (u_tree_type(u, k) _k = {}; avl_for_init(u, 0); avl_for_end(u))                            \
      for (u_tree_type(u, v) _v = {}; avl_for(u, &_k, &_v);)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IAVL_H__ */
