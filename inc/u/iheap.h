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
#ifndef U_IHEAP_H__
#  define U_IHEAP_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* __u_heap_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern any_t heap_new(size_t, bool, u_cmp_fn);

extern void heap_clear(any_t);

extern void heap_cleanup(any_t);

extern size_t heap_len(any_t);

extern void heap_at(any_t, any_t);

extern void heap_pop(any_t, any_t);

extern void heap_put(any_t, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_heap_t(...) typeof(__u_heap_t(*)(__VA_ARGS__))

#  define _u_heap_defs(T)                                                                          \
    u_heap_t(T) : (T) {                                                                            \
    }

#  define u_heap_type(u)     typeof(_Generic(u, u_heap_defs))
#  define u_heap_type_val(u) _Generic(u, u_heap_defs)

#  if defined(NDEBUG)
#    define u_heap_type_check(u)
#  else
#    define u_heap_type_check(u) static_assert(typeeq((__u_heap_t){}, u(u_heap_type_val(u))))
#  endif

/***************************************************************************************************
 * iApi heap
 **************************************************************************************************/
#  define u_heap_init(u, attr, fn)                                                                 \
    do {                                                                                           \
      u_heap_type_check(u);                                                                        \
                                                                                                   \
      u = heap_new(sizeof(u_heap_type(u)), attr, fn);                                              \
    } while (0)

#  define u_heap_new(...)                                                                          \
    ({                                                                                             \
      u_heap_t(u_va_at(0, __VA_ARGS__)) u = nullptr;                                               \
                                                                                                   \
      u_heap_init(u, u_va_at(1, __VA_ARGS__), u_va_at(2, __VA_ARGS__));                            \
                                                                                                   \
      u;                                                                                           \
    })

#  define u_heap_len(u)                                                                            \
    ({                                                                                             \
      u_heap_type_check(u);                                                                        \
                                                                                                   \
      heap_len(u);                                                                                 \
    })

#  define u_heap_is_empty(u)                                                                       \
    ({                                                                                             \
      u_heap_type_check(u);                                                                        \
                                                                                                   \
      0 == heap_len(u);                                                                            \
    })

#  define u_heap_clear(u)                                                                          \
    do {                                                                                           \
      u_heap_type_check(u);                                                                        \
                                                                                                   \
      heap_clear(u);                                                                               \
    } while (0)

#  define u_heap_cleanup(u)                                                                        \
    do {                                                                                           \
      u_heap_type_check(u);                                                                        \
                                                                                                   \
      heap_cleanup(u);                                                                             \
                                                                                                   \
      u = nullptr;                                                                                 \
    } while (0)

#  define u_heap_at(u)                                                                             \
    ({                                                                                             \
      u_heap_type_check(u);                                                                        \
                                                                                                   \
      u_heap_type(u) _a = {};                                                                      \
                                                                                                   \
      heap_at(u, &_a);                                                                             \
                                                                                                   \
      _a;                                                                                          \
    })

#  define u_heap_pop(u)                                                                            \
    ({                                                                                             \
      u_heap_type_check(u);                                                                        \
                                                                                                   \
      u_heap_type(u) _a = {};                                                                      \
                                                                                                   \
      heap_pop(u, &_a);                                                                            \
                                                                                                   \
      _a;                                                                                          \
    })

#  define u_heap_put(u, ...)                                                                       \
    do {                                                                                           \
      u_heap_type_check(u);                                                                        \
                                                                                                   \
      auto _a = u_va_at(0, __VA_ARGS__);                                                           \
                                                                                                   \
      heap_put(u, &_a);                                                                            \
    } while (0)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IHEAP_H__ */
