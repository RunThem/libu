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

#ifndef U_ILIST_H__
#  define U_ILIST_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/* clang-format off */

/***************************************************************************************************
 * Api
 **************************************************************************************************/
typedef struct {}* $list_t;

extern any_t  $list_new      ();
extern void   $list_cleanup  (any_t);
extern any_t  $list_head     (any_t);
extern any_t  $list_tail     (any_t);
extern any_t  $list_prev     (any_t, any_t);
extern any_t  $list_next     (any_t, any_t);
extern void   $list_pop      (any_t, any_t);
extern void   $list_put      (any_t, any_t, any_t);
extern bool   $list_each     (any_t, any_t*);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_list_t(T)                                                                              \
    typeof(const struct [[gnu::packed]] {                                                          \
      $list_t ref;                                                                                 \
      int len;                                                                                     \
                                                                                                   \
      struct { T* val; } _[0]; /* Don't use this field. */                                         \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_list_new(T)                                                                            \
    ({                                                                                             \
      u_list_t(T) self = $list_new();                                                              \
                                                                                                   \
      (any_t)self->ref;                                                                            \
    })

#  define u_list_cleanup(self, ...)                                                                \
    do {                                                                                           \
      typecheck($list_t, self->ref, "mete type not's List<T>");                                    \
                                                                                                   \
      list_cleanup(self);                                                                          \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

#  define u_list_head(self)                                                                        \
    ({                                                                                             \
      typecheck($list_t, self->ref, "mete type not's List<T>");                                    \
                                                                                                   \
      typeof_unqual(self->_[0].val) __val__ = $list_head(self->ref);                               \
                                                                                                   \
      __val__;                                                                                     \
    })

#  define u_list_tail(self)                                                                        \
    ({                                                                                             \
      typecheck($list_t, self->ref, "mete type not's List<T>");                                    \
                                                                                                   \
      typeof_unqual(self->_[0].val) __val__ = $list_tail(self->ref);                               \
                                                                                                   \
      __val__;                                                                                     \
    })

#  define u_list_prev(self, item)                                                                  \
    ({                                                                                             \
      typecheck($list_t, self->ref, "mete type not's List<T>");                                    \
                                                                                                   \
      typeof_unqual(self->_[0].val) __val__ = $list_prev(self->ref, item);                         \
                                                                                                   \
      __val__;                                                                                     \
    })

#  define u_list_next(self, item)                                                                  \
    ({                                                                                             \
      typecheck($list_t, self->ref, "mete type not's List<T>");                                    \
                                                                                                   \
      typeof_unqual(self->_[0].val) __val__ = $list_next(self->ref, item);                         \
                                                                                                   \
      __val__;                                                                                     \
    })

#  define u_list_pop(self, ...)                                                                    \
    ({                                                                                             \
      typecheck($list_t, self->ref, "mete type not's List<T>");                                    \
                                                                                                   \
      typeof_unqual(self->_[0].val) __val__ = u_va_0th($list_head(self->ref), __VA_ARGS__);        \
                                                                                                   \
      $list_pop(self->ref, __val__);                                                               \
                                                                                                   \
      __val__;                                                                                     \
    })

#  define u_list_put(self, pos, ...)                                                               \
    do {                                                                                           \
      typecheck($list_t, self->ref, "mete type not's List<T>");                                    \
                                                                                                   \
      u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                   \
        typeof_unqual(self->_[0].val) __pos__ = pos;                                               \
        typeof_unqual(self->_[0].val) __val__ = u_va_at(0, __VA_ARGS__);                           \
      )(                                                                                           \
        typeof_unqual(self->_[0].val) __pos__ = $list_tail(self->ref);                             \
        typeof_unqual(self->_[0].val) __val__ = pos;                                               \
      )                                                                                            \
                                                                                                   \
      $list_put(self->ref, __pos__, __val__);                                                      \
    } while (0)

#  define u_list_each(self, it)                                                                    \
    typecheck($list_t, self->ref, "mete type not's List<T>");                                      \
                                                                                                   \
    $list_each(self->ref, nullptr);                                                                \
    for (typeof_unqual(self->_[0].val) it = {}; $list_each(self->ref, (any_t*)&it);)

#  define u_list_each_if(self, it, cond)                                                           \
    typecheck($list_t, self->ref, "mete type not's List<T>");                                      \
                                                                                                   \
    $list_each(self->ref, nullptr);                                                                \
    for (typeof_unqual(self->_[0].val) it = {}; $list_each(self->ref, (any_t*)&it);)               \
      if (cond)

#  define u_list_find_if(self, cond)                                                               \
    ({                                                                                             \
      typecheck($list_t, self->ref, "mete type not's List<T>");                                    \
                                                                                                   \
      typeof_unqual(self->_[0].val) __ = {};                                                       \
                                                                                                   \
      u_list_each_if(self, it, cond) {                                                             \
        __ = it;                                                                                   \
        break;                                                                                     \
      }                                                                                            \
                                                                                                   \
      __;                                                                                          \
    })

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ILIST_H__ */
