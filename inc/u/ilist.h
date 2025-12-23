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

extern any_t $list_new     ();
extern void  $list_cleanup (any_t);
extern any_t $list_head    (any_t);
extern any_t $list_tail    (any_t);
extern any_t $list_prev    (any_t, any_t);
extern any_t $list_next    (any_t, any_t);
extern void  $list_del     (any_t, any_t);
extern any_t $list_add     (any_t, any_t, any_t);
extern any_t $list_each    (any_t, bool);
extern any_t $list_reach   (any_t, bool);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_list_t(T)                                                                              \
    typeof(const struct [[gnu::packed]] {                                                          \
      any_t ref;                                                                                   \
      int len;                                                                                     \
                                                                                                   \
      struct {                                                                                     \
        $list_t meta;                                                                              \
              T * mut;                                                                             \
        const T * ref;                                                                             \
      } _[0]; /* Don't use this field. */                                                          \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_list_new(self)                                                                         \
    ({                                                                                             \
      typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                              \
                                                                                                   \
      self = $list_new();                                                                          \
                                                                                                   \
      self->ref;                                                                                   \
    })


#  define u_list_cleanup(self, ...)                                                                \
    do {                                                                                           \
      typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                              \
                                                                                                   \
      u_va_has_if(__VA_ARGS__) ( u_list_each_ref(self, it) { __VA_ARGS__ } )                       \
                                                                                                   \
      $list_cleanup(self);                                                                         \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)


#  define u_list_head_ref(self)                                                                    \
    ({                                                                                             \
      (typeof(self->_[0].ref)) u_list_head_mut(self);                                              \
    })


#  define u_list_head_mut(self)                                                                    \
    ({                                                                                             \
      typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                              \
                                                                                                   \
      typeof(self->_[0].mut) __mut__ = $list_head(self->ref);                                      \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_list_tail_ref(self)                                                                    \
    ({                                                                                             \
      (typeof(self->_[0].ref)) u_list_tail_mut(self);                                              \
    })


#  define u_list_tail_mut(self)                                                                    \
    ({                                                                                             \
      typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                              \
                                                                                                   \
      typeof(self->_[0].mut) __mut__ = $list_tail(self->ref);                                      \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_list_prev_ref(self, _uptr)                                                             \
    ({                                                                                             \
      (typeof(self->_[0].ref)) u_list_prev_mut(self, _uptr);                                       \
    })


#  define u_list_prev_mut(self, _uptr)                                                             \
    ({                                                                                             \
      typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                              \
                                                                                                   \
      typeof(self->_[0].mut) __mut__ = $list_prev(self->ref, _uptr);                               \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_list_next_ref(self, _uptr)                                                             \
    ({                                                                                             \
      (typeof(self->_[0].ref)) u_list_next_mut(self, _uptr);                                       \
    })


#  define u_list_next_mut(self, _uptr)                                                             \
    ({                                                                                             \
      typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                              \
                                                                                                   \
      typeof(self->_[0].mut) __mut__ = $list_next(self->ref, _uptr);                               \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_list_remove(self, _uptr)                                                               \
    do {                                                                                           \
      typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                              \
                                                                                                   \
      $list_del(self->ref, _uptr);                                                                 \
    } while (0)


#  define u_list_remove_head(self) u_list_remove(self, u_list_head_mut(self))


#  define u_list_remove_tail(self) u_list_remove(self, u_list_tail_mut(self))


#  define u_list_insert(self, _uidxptr, _uptr)                                                     \
    do {                                                                                           \
      typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                              \
                                                                                                   \
      typeof_unqual(self->_[0].mut)* __mut__ = $list_add(self->ref, _uidxptr, _uptr);              \
    } while (0)


#  define u_list_insert_head(self, _uptr) u_list_insert(self, nullptr, _uptr)


#  define u_list_insert_tail(self, _uptr) u_list_insert(self, u_list_tail_mut(self), _uptr)


#  define u_list_each_ref(self, it)                                                                \
    typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                                \
                                                                                                   \
    $list_each(self->ref, !0);                                                                     \
    for (auto it = (typeof(self->_[0].ref)) {}; (it = $list_each(self->ref, !!0)); )


#  define u_list_each_if_ref(self, it, cond) u_list_each_ref(self, it) if (cond)


#  define u_list_each_mut(self, it)                                                                \
    typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                                \
                                                                                                   \
    $list_each(self->ref, !0);                                                                     \
    for (auto it = (typeof(self->_[0].mut)) {}; (it = $list_each(self->ref, !!0)); )


#  define u_list_each_if_mut(self, it, cond) u_list_each_mut(self, it) if (cond)


#  define u_list_reach_ref(self, it)                                                               \
    typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                                \
                                                                                                   \
    $list_reach(self->ref, !0);                                                                    \
    for (auto it = (typeof(self->_[0].ref)) {}; (it = $list_reach(self->ref, !!0)); )


#  define u_list_reach_if_ref(self, it, cond) u_list_reach_ref(self, it) if (cond)


#  define u_list_reach_mut(self, it)                                                               \
    typecheck($list_t, self->_[0].meta, "mete type not's List<T>");                                \
                                                                                                   \
    $list_reach(self->ref, !0);                                                                    \
    for (auto it = (typeof(self->_[0].mut)) {}; (it = $list_reach(self->ref, !!0)); )


#  define u_list_reach_if_mut(self, it, cond) u_list_reach_mut(self, it) if (cond)

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ILIST_H__ */
