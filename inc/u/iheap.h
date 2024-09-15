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
}* __u_heap_ref_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern any_t heap_new     (i64_t, u_order_e, u_cmp_fn);
extern void  heap_clear   (any_t);
extern void  heap_cleanup (any_t);
extern i64_t heap_len     (any_t);
extern void  heap_at      (any_t, any_t);
extern void  heap_pop     (any_t, any_t);
extern void  heap_put     (any_t, any_t);
/* clang-format on */

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_heap_t(T) typeof(__u_heap_ref_t(*)(T*))

/***************************************************************************************************
 * iApi heap
 **************************************************************************************************/
#  define u_heap_init(self, order, cmp_fn)                                                         \
    do {                                                                                           \
      u_check(self, 1, __u_heap_ref_t);                                                            \
                                                                                                   \
      self = heap_new(sizeof(u_types(self, 0)), order, cmp_fn);                                    \
    } while (0)

#  define u_heap_new(T, order, cmp_fn)                                                             \
    ({                                                                                             \
      u_heap_t(T) self = nullptr;                                                                  \
                                                                                                   \
      self = heap_new(sizeof(T), order, cmp_fn);                                                   \
                                                                                                   \
      self;                                                                                        \
    })

#  define u_heap_len(self)                                                                         \
    ({                                                                                             \
      u_check(self, 1, __u_heap_ref_t);                                                            \
                                                                                                   \
      heap_len(self);                                                                              \
    })

#  define u_heap_is_empty(self)                                                                    \
    ({                                                                                             \
      u_check(self, 1, __u_heap_ref_t);                                                            \
                                                                                                   \
      0 == heap_len(self);                                                                         \
    })

#  define u_heap_clear(self)                                                                       \
    do {                                                                                           \
      u_check(self, 1, __u_heap_ref_t);                                                            \
                                                                                                   \
      heap_clear(self);                                                                            \
    } while (0)

#  define u_heap_cleanup(self)                                                                     \
    do {                                                                                           \
      u_check(self, 1, __u_heap_ref_t);                                                            \
                                                                                                   \
      heap_cleanup(self);                                                                          \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

#  define u_heap_at(self)                                                                          \
    ({                                                                                             \
      u_check(self, 1, __u_heap_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0) __a = {};                                                                   \
                                                                                                   \
      heap_at(self, &__a);                                                                         \
                                                                                                   \
      __a;                                                                                         \
    })

#  define u_heap_pop(self)                                                                         \
    ({                                                                                             \
      u_check(self, 1, __u_heap_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0) __a = {};                                                                   \
                                                                                                   \
      heap_pop(self, &__a);                                                                        \
                                                                                                   \
      __a;                                                                                         \
    })

#  define u_heap_put(self, item)                                                                   \
    do {                                                                                           \
      u_check(self, 1, __u_heap_ref_t);                                                            \
                                                                                                   \
      u_types(self, 0) __a = item;                                                                 \
                                                                                                   \
      heap_put(self, &__a);                                                                        \
    } while (0)

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IHEAP_H__ */
