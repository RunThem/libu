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

/* clang-format off */

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern any_t $heap_new     (i32_t, u_order_e, u_cmp_fn);
extern void  $heap_clear   (any_t);
extern void  $heap_cleanup (any_t);
extern any_t $heap_at      (any_t);
extern void  $heap_pop     (any_t, any_t);
extern void  $heap_put     (any_t, any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_heap_t(T)                                                                              \
    typeof(const struct [[gnu::packed]] {                                                          \
      void* ref;                                                                                   \
      int len;                                                                                     \
      int cap;                                                                                     \
                                                                                                   \
      struct { T val; } _[0]; /* Don't use this field. */                                          \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_heap_new(T, order, cmp_fn)                                                             \
    ({                                                                                             \
      u_heap_t(T) self = $heap_new(sizeof(T), order, cmp_fn);                                      \
                                                                                                   \
      self->ref;                                                                                   \
    })

#  define u_heap_is_empty(self)                                                                    \
    ({                                                                                             \
      0 == $heap_len(self->ref);                                                                   \
    })

#  define u_heap_clear(self)                                                                       \
    do {                                                                                           \
      $heap_clear(self->ref);                                                                      \
    } while (0)

#  define u_heap_cleanup(self)                                                                     \
    do {                                                                                           \
      $heap_cleanup(self->ref);                                                                    \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

#  define u_heap_at(self)                                                                          \
    ({                                                                                             \
      typeof_unqual(self->_[0].val)* __val__ = $heap_at(self->ref);                                \
                                                                                                   \
      *__val__;                                                                                    \
    })

#  define u_heap_pop(self)                                                                         \
    ({                                                                                             \
      typeof_unqual(self->_[0]) __it__ = {};                                                       \
                                                                                                   \
      $heap_pop(self->ref, &__it__.val);                                                           \
                                                                                                   \
      __it__.val;                                                                                  \
    })

#  define u_heap_put(self, item)                                                                   \
    do {                                                                                           \
      typeof_unqual(self->_[0]) __it__ = {item};                                                   \
                                                                                                   \
      heap_put(self, &__it__.val);                                                                 \
    } while (0)

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IHEAP_H__ */
