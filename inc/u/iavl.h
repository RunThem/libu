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

#ifndef U_IAVL_H__
#define U_IAVL_H__

#include "utils/type.h"
#include "utils/va.h"

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_avl_t;

/***************************************************************************************************
 * Api
 ***************s***********************************************************************************/
extern any_t avl_new(size_t, size_t, u_cmp_fn);

extern size_t avl_len(any_t);

extern bool avl_exist(any_t, any_t);

extern void avl_clear(any_t);

extern void avl_cleanup(any_t);

extern any_t avl_at(any_t, any_t);

extern void avl_pop(any_t, any_t, any_t);

extern void avl_put(any_t, any_t, any_t);

extern bool avl_for_init(any_t, bool);

extern void avl_for_end(any_t);

extern bool avl_for(any_t, any_t, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define uavl(K, V) typeof(u_avl_t(*)(K, V))

#define __uavl_def(K, V)                                                                           \
  uavl(K, V) :                                                                                     \
      (struct {                                                                                    \
        K k;                                                                                       \
        V v;                                                                                       \
      }) {                                                                                         \
  }

#define _uavl_def(arg) __uavl_def arg

#define ut_type(u, arg)     typeof(_Generic(typeof(u), va_map(_uavl_def, va_unpack(uavl_def))).arg)
#define ut_type_val(u, arg) _Generic(typeof(u), va_map(_uavl_def, va_unpack(uavl_def))).arg
#define ut_type_check(u)    static_assert(typeeq((u_avl_t){}, u(ut_type_val(u, k), ut_type_val(u, v))))

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#define ut_init(u, fn)                                                                             \
  do {                                                                                             \
    ut_type_check(u);                                                                              \
                                                                                                   \
    u = avl_new(sizeof(ut_type(u, k)), sizeof(ut_type(u, v)), fn);                                 \
  } while (0)

#define ut_new(K, V, fn)                                                                           \
  ({                                                                                               \
    uavl(K, V) u = nullptr;                                                                        \
                                                                                                   \
    ut_init(u, fn);                                                                                \
                                                                                                   \
    u;                                                                                             \
  })

#define ut_len(u)                                                                                  \
  ({                                                                                               \
    ut_type_check(u);                                                                              \
                                                                                                   \
    avl_len(u);                                                                                    \
  })

#define ut_is_empty(u)                                                                             \
  ({                                                                                               \
    ut_type_check(u);                                                                              \
                                                                                                   \
    0 == avl_len(u);                                                                               \
  })

#define ut_is_exist(u, _k)                                                                         \
  ({                                                                                               \
    ut_type_check(u);                                                                              \
                                                                                                   \
    ut_type(u, k) _a = _k;                                                                         \
    ut_type(u, v) _b = {};                                                                         \
                                                                                                   \
    avl_exist(u, &_a);                                                                             \
  })

#define ut_clear(u)                                                                                \
  do {                                                                                             \
    ut_type_check(u);                                                                              \
                                                                                                   \
    avl_clear(u);                                                                                  \
  } while (0)

#define ut_cleanup(u)                                                                              \
  do {                                                                                             \
    ut_type_check(u);                                                                              \
                                                                                                   \
    avl_cleanup(u);                                                                                \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

/* clang-format off */
#  define ut_at(u, _k, ...)                                                                        \
    va_elseif(va_size_is(1, __VA_ARGS__)) (                                                        \
      ({                                                                                           \
        ut_type_check(u);                                                                          \
                                                                                                   \
        bool _ret         = false;                                                                 \
        ut_type(u, k) _a  = _k;                                                                    \
        ut_type(u, v)* _b = {};                                                                    \
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
        ut_type_check(u);                                                                          \
                                                                                                   \
        ut_type(u, k) _a  = _k;                                                                    \
        ut_type(u, v) _it = {};                                                                    \
        ut_type(u, v)* _b = {};                                                                    \
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

#define ut_try(u, _k)                                                                              \
  for (ut_type(u, v)* it = avl_at(u, &(ut_type(u, k)){_k}); it != nullptr; it = nullptr)

#define ut_pop(u, _k)                                                                              \
  ({                                                                                               \
    ut_type_check(u);                                                                              \
                                                                                                   \
    ut_type(u, k) _a = _k;                                                                         \
    ut_type(u, v) _b = {};                                                                         \
                                                                                                   \
    avl_pop(u, &_a, &_b);                                                                          \
                                                                                                   \
    _b;                                                                                            \
  })

#define ut_put(u, _k, _v)                                                                          \
  do {                                                                                             \
    ut_type_check(u);                                                                              \
                                                                                                   \
    ut_type(u, k) _a = _k;                                                                         \
    ut_type(u, v) _b = _v;                                                                         \
                                                                                                   \
    avl_put(u, &_a, &_b);                                                                          \
  } while (0)

#define ut_for_all(u, _k, _v)                                                                      \
  for (ut_type(u, k) _k = {}; avl_for_init(u, 1); avl_for_end(u))                                  \
    for (ut_type(u, v) _v = {}; avl_for(u, &_k, &_v);)

#define ut_rfor_all(u, _k, _v)                                                                     \
  for (ut_type(u, k) _k = {}; avl_for_init(u, 0); avl_for_end(u))                                  \
    for (ut_type(u, v) _v = {}; avl_for(u, &_k, &_v);)

#endif /* !U_IAVL_H__ */
