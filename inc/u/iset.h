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

#ifndef U_ISET_H__
#  define U_ISET_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
} __u_set_t;

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
#  define u_set_t(...) typeof(__u_set_t(*)(__VA_ARGS__))

#  define _u_set_defs(T)                                                                           \
    u_set_t(T) : (T) {                                                                             \
    }

#  define u_set_type(u)     typeof(_Generic(u, u_set_defs))
#  define u_set_type_val(u) _Generic(u, u_set_defs)

#  if defined(NDEBUG)
#    define u_set_type_check(u)
#  else
#    define u_set_type_check(u) static_assert(typeeq((__u_set_t){}, u(u_set_type_val(u))))
#  endif

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#  define u_set_init(u, fn)                                                                        \
    do {                                                                                           \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      u = avl_new(sizeof(u_set_type(u)), 0, fn);                                                   \
    } while (0)

#  define u_set_new(...)                                                                           \
    ({                                                                                             \
      u_set_t(va_at(0, __VA_ARGS__)) u = nullptr;                                                  \
                                                                                                   \
      u_set_init(u, va_at(1, __VA_ARGS__));                                                        \
                                                                                                   \
      u;                                                                                           \
    })

#  define u_set_len(u)                                                                             \
    ({                                                                                             \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      avl_len(u);                                                                                  \
    })

#  define u_set_is_empty(u)                                                                        \
    ({                                                                                             \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      0 == avl_len(u);                                                                             \
    })

#  define u_set_is_exist(u, _k)                                                                    \
    ({                                                                                             \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      u_set_type(u) _a = _k;                                                                       \
                                                                                                   \
      avl_exist(u, &_a);                                                                           \
    })

#  define u_set_clear(u)                                                                           \
    do {                                                                                           \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      avl_clear(u);                                                                                \
    } while (0)

#  define u_set_cleanup(u)                                                                         \
    do {                                                                                           \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      avl_cleanup(u);                                                                              \
                                                                                                   \
      u = nullptr;                                                                                 \
    } while (0)

/* clang-format off */
#  define u_set_at(u, _it, ...)                                                                    \
    va_elseif(va_cnt_is(1, __VA_ARGS__)) (                                                         \
      ({                                                                                           \
        u_set_type_check(u);                                                                       \
                                                                                                   \
        bool _ret         = false;                                                                 \
        u_set_type(u) it  = _it;                                                                   \
        u_set_type(u) _a  = va_at(0, __VA_ARGS__);                                                 \
        u_set_type(u)* _b = {};                                                                    \
                                                                                                   \
        _b = avl_at(u, &it);                                                                       \
                                                                                                   \
        if (_b != nullptr && 0 == avl_fn(u)(&_it, &_a)) {                                          \
          *(_b - 1) = va_at(0, __VA_ARGS__);                                                       \
          _ret = true;                                                                             \
        }                                                                                          \
                                                                                                   \
        _ret;                                                                                      \
      })                                                                                           \
    ) (                                                                                            \
      ({                                                                                           \
        u_set_type_check(u);                                                                       \
                                                                                                   \
        u_set_type(u) it  = _it;                                                                   \
        u_set_type(u) _a = {};                                                                     \
        u_set_type(u)* _b = {};                                                                    \
                                                                                                   \
        _b = avl_at(u, &it);                                                                       \
                                                                                                   \
        if (_b == nullptr) {                                                                       \
          _b = &_it;                                                                               \
        } else {                                                                                   \
          _b -= 1;                                                                                 \
        }                                                                                          \
                                                                                                   \
        *_b;                                                                                       \
      })                                                                                           \
    )
/* clang-format on */

#  define u_set_try(u, _it)                                                                        \
    for (u_set_type(u)* it = avl_at(u, ({                                                          \
                                      u_set_type(u) __it = _it;                                    \
                                      &__it;                                                       \
                                    }));                                                           \
         it != nullptr && (it -= 1);                                                               \
         it = nullptr)

#  define u_set_pop(u, _it)                                                                        \
    ({                                                                                             \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      u_set_type(u) it = _it;                                                                      \
                                                                                                   \
      avl_pop(u, &it, &it);                                                                        \
                                                                                                   \
      it;                                                                                          \
    })

#  define u_set_put(u, _it)                                                                        \
    do {                                                                                           \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      u_set_type(u) it = _it;                                                                      \
                                                                                                   \
      avl_put(u, &it, &it);                                                                        \
    } while (0)

#  define u_set_fn(u, _k1, _k2)                                                                    \
    ({                                                                                             \
      u_set_type_check(u);                                                                         \
                                                                                                   \
      u_set_type(u) _a = _k1;                                                                      \
      u_set_type(u) _b = _k2;                                                                      \
                                                                                                   \
      avl_fn(u)(&_a, &_b);                                                                         \
    })

#  define u_set_for(u, _it)                                                                        \
    for (u_set_type(u) _it = {}; avl_for_init(u, 1); avl_for_end(u))                               \
      for (; avl_for(u, &_it, &_it);)

#  define u_set_rfor(u, _it)                                                                       \
    for (u_set_type(u) _it = {}; avl_for_init(u, 0); avl_for_end(u))                               \
      for (; avl_for(u, &_it, &_it);)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ISET_H__ */
