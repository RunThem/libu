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
typedef struct {}* $heap_t;

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
      any_t ref;                                                                                   \
      int len;                                                                                     \
      int cap;                                                                                     \
                                                                                                   \
      struct {                                                                                     \
        $heap_t meta;                                                                              \
        T val;                                                                                     \
      } _[0]; /* Don't use this field. */                                                          \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_heap_new(self, order, cmp_fn)                                                          \
    ({                                                                                             \
      typecheck($heap_t, self->_[0].meta, "mete type not's Heap<T>");                              \
                                                                                                   \
      self = $heap_new(sizeof(self->_[0].val), order, cmp_fn);                                     \
                                                                                                   \
      self->ref;                                                                                   \
    })


#  define u_heap_clear(self)                                                                       \
    do {                                                                                           \
      typecheck($heap_t, self->_[0].meta, "mete type not's Heap<T>");                              \
                                                                                                   \
      $heap_clear(self->ref);                                                                      \
    } while (0)


#  define u_heap_cleanup(self)                                                                     \
    do {                                                                                           \
      typecheck($heap_t, self->_[0].meta, "mete type not's Heap<T>");                              \
                                                                                                   \
      $heap_cleanup(self->ref);                                                                    \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)


#  define u_heap_peek(self)                                                                        \
    ({                                                                                             \
      typecheck($heap_t, self->_[0].meta, "mete type not's Heap<T>");                              \
                                                                                                   \
      typeof_unqual(self->_[0].val)* __val__ = $heap_at(self->ref);                                \
                                                                                                   \
      *__val__;                                                                                    \
    })


#  define u_heap_pop(self)                                                                         \
    ({                                                                                             \
      typecheck($heap_t, self->_[0].meta, "mete type not's Heap<T>");                              \
                                                                                                   \
      typeof_unqual(self->_[0].val) __val__ = {};                                                  \
                                                                                                   \
      $heap_pop(self->ref, &__val__);                                                              \
                                                                                                   \
      __val__;                                                                                     \
    })


#  define u_heap_put(self, _val)                                                                   \
    do {                                                                                           \
      typecheck($heap_t, self->_[0].meta, "mete type not's Heap<T>");                              \
                                                                                                   \
      typeof_unqual(self->_[0].val) __val__ = _val;                                                \
                                                                                                   \
      heap_put(self, &__val__);                                                                    \
    } while (0)

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IHEAP_H__ */
