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

#ifndef U_IVEC_H__
#define U_IVEC_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils.h"

/***************************************************************************************************
 * Meta Type
 **************************************************************************************************/
typedef struct {
} u_vec_meta_t;

/***************************************************************************************************
 * iType
 **************************************************************************************************/

/**
 * ::Class Vec<T>
 */
#define u_vec_t(T)                                                                                 \
  typeof(const struct [[gnu::packed]] {                                                            \
    any_t ref;                                                                                     \
    int len;                                                                                       \
    int cap;                                                                                       \
                                                                                                   \
    struct {                                                                                       \
      u_vec_meta_t meta;                                                                           \
                                                                                                   \
      T val;                                                                                       \
      T* mut;                                                                                      \
      const T* ref;                                                                                \
    } _[0]; /* Don't use this field. */                                                            \
  }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/

/**
 * ::Vec<T>::new(self) -> Self
 * ::Vec<T>::new(self, cap) -> Self
 */
#define u_vec_new(self, ...)                                                                       \
  ({                                                                                               \
    extern pub any_t __u_vec_new(i32_t, i32_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                          \
    }                                                                                              \
                                                                                                   \
    (self) = __u_vec_new(sizeof((self)->_->val), u_va_0th(16, __VA_ARGS__));                       \
                                                                                                   \
    (self)->ref;                                                                                   \
  })

/**
 * ::Vec<T>::clear(self) -> !
 * ::Vec<T>::clear(self, proc) -> !
 */
#define u_vec_clear(self, ...)                                                                     \
  do {                                                                                             \
    extern pub void __u_vec_clear(any_t);                                                          \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                          \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    u_va_has_if(__VA_ARGS__)(u_vec_each (self, it) { __VA_ARGS__; });                              \
                                                                                                   \
    __u_vec_clear((self)->ref);                                                                    \
  } while (0)

/**
 * ::Vec<T>::cleanup(self) -> !
 * ::Vec<T>::cleanup(self, proc) -> !
 */
#define u_vec_cleanup(self, ...)                                                                   \
  do {                                                                                             \
    extern pub void __u_vec_cleanup(any_t);                                                        \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                          \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
    }                                                                                              \
                                                                                                   \
    u_va_has_if(__VA_ARGS__)(u_vec_each (self, it) { __VA_ARGS__; });                              \
                                                                                                   \
    __u_vec_cleanup((self)->ref);                                                                  \
                                                                                                   \
    (self) = NULL;                                                                                 \
  } while (0)

/**
 * ::Vec<T>::resize(self, cap) -> bool
 */
#define u_vec_resize(self, _cap)                                                                   \
  ({                                                                                               \
    extern pub bool __u_vec_resize(any_t, i32_t);                                                  \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                          \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Cap = _cap;                                                                             \
      assert(Self->cap < Cap);                                                                     \
    }                                                                                              \
                                                                                                   \
    __u_vec_resize((self)->ref, _cap);                                                             \
  })

/**
 * ::Vec<T>::at(self, idx) -> T
 * ::Vec<T>::at(self, idx, val) -> T
 */
#define u_vec_at(self, _idx, ...)                                                                  \
  ({                                                                                               \
    ;                                                                                              \
    *u_vec_at_mut(self, _idx) u_va_has_if(__VA_ARGS__)(= u_va_at(0, __VA_ARGS__));                 \
  })

/**
 * ::Vec<T>::at_ref(self, idx) -> const T*
 */
#define u_vec_at_ref(self, _idx)                                                                   \
  ({                                                                                               \
    ;                                                                                              \
    (typeof((self)->_->ref))u_vec_at_mut(self, _idx);                                              \
  })

/**
 * ::Vec<T>::at_mut(self, idx) -> T*
 */
#define u_vec_at_mut(self, _idx)                                                                   \
  ({                                                                                               \
    extern pub any_t __u_vec_at(any_t, i32_t);                                                     \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                          \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Idx = _idx;                                                                             \
      assert(Idx < Self->len);                                                                     \
    }                                                                                              \
                                                                                                   \
    (typeof((self)->_->mut))__u_vec_at((self)->ref, _idx);                                         \
  })

/**
 * ::Vec<T>::remove(self, idx) -> T
 */
#define u_vec_remove(self, _idx)                                                                   \
  ({                                                                                               \
    extern pub void __u_vec_del(any_t, i32_t);                                                     \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                          \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Idx = _idx;                                                                             \
      assert(Idx < Self->len);                                                                     \
    }                                                                                              \
                                                                                                   \
    auto __val__ = u_vec_at(self, _idx);                                                           \
                                                                                                   \
    __u_vec_del((self)->ref, _idx);                                                                \
                                                                                                   \
    __val__;                                                                                       \
  })

/**
 * ::Vec<T>::remove_front(self) -> T
 */
#define u_vec_remove_front(self) u_vec_remove(self, 0)

/**
 * ::Vec<T>::remove_back(self) -> T
 */
#define u_vec_remove_back(self) u_vec_remove(self, (self)->len - 1)

/**
 * ::Vec<T>::insert(self, idx, val) -> !
 */
#define u_vec_insert(self, _idx, _val)                                                             \
  do {                                                                                             \
    extern pub any_t __u_vec_add(any_t, i32_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                          \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Idx = _idx;                                                                             \
      assert(Idx <= Self->len);                                                                    \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_->mut) __mut__ = __u_vec_add((self)->ref, _idx);                               \
                                                                                                   \
    *__mut__ = _val;                                                                               \
  } while (0)

/**
 * ::Vec<T>::insert_front(self, val) -> !
 */
#define u_vec_insert_front(self, _val) u_vec_insert(self, 0, _val)

/**
 * ::Vec<T>::insert_back(self, val) -> !
 */
#define u_vec_insert_back(self, _val) u_vec_insert(self, (self)->len, _val)

/**
 * ::Vec<T>::each(self, it) -> Iter<it = T>
 */
#define u_vec_each(self, it)                                                                       \
  {                                                                                                \
    extern pub any_t __u_vec_each(any_t, bool);                                                    \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                            \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_each((self)->ref, !0);                                                           \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_->val)){}; ({                                                    \
         extern pub any_t __u_vec_each(any_t, bool);                                               \
                                                                                                   \
         typeof((self)->_->ref) __ref__ = __u_vec_each((self)->ref, !!0);                          \
                                                                                                   \
         if (__ref__)                                                                              \
           it = *__ref__;                                                                          \
                                                                                                   \
         __ref__;                                                                                  \
       });)

/**
 * ::Vec<T>::each_if(self, it, cond) -> Iter<it = T>
 */
#define u_vec_each_if(self, it, cond)                                                              \
  u_vec_each (self, it)                                                                            \
    if (cond)

/**
 * ::Vec<T>::each_ref(self, it) -> Iter<it = const T*>
 */
#define u_vec_each_ref(self, it)                                                                   \
  {                                                                                                \
    extern pub any_t __u_vec_each(any_t, bool);                                                    \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                            \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_each((self)->ref, !0);                                                           \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_->ref)){}; ({                                                    \
         extern pub any_t __u_vec_each(any_t, bool);                                               \
                                                                                                   \
         it = __u_vec_each((self)->ref, !!0);                                                      \
       });)

/**
 * ::Vec<T>::each_if_ref(self, it, cond) -> Iter<it = const T*>
 */
#define u_vec_each_if_ref(self, it, cond)                                                          \
  u_vec_each_ref (self, it)                                                                        \
    if (cond)

/**
 * ::Vec<T>::each_mut(self, it) -> Iter<it = T*>
 */
#define u_vec_each_mut(self, it)                                                                   \
  {                                                                                                \
    extern pub any_t __u_vec_each(any_t, bool);                                                    \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                            \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_each((self)->ref, !0);                                                           \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_->mut)){}; ({                                                    \
         extern pub any_t __u_vec_each(any_t, bool);                                               \
                                                                                                   \
         it = __u_vec_each((self)->ref, !!0);                                                      \
       });)

/**
 * ::Vec<T>::each_if_mut(self, it, cond) -> Iter<it = T*>
 */
#define u_vec_each_if_mut(self, it, cond)                                                          \
  u_vec_each_mut (self, it)                                                                        \
    if (cond)

/**
 * ::Vec<T>::reach(self, it) -> Iter<it = T>
 */
#define u_vec_reach(self, it)                                                                      \
  {                                                                                                \
    extern pub any_t __u_vec_reach(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                            \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_reach((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_->val)){}; ({                                                    \
         extern pub any_t __u_vec_reach(any_t, bool);                                              \
                                                                                                   \
         typeof((self)->_->ref) __ref__ = __u_vec_reach((self)->ref, !!0);                         \
                                                                                                   \
         if (__ref__)                                                                              \
           it = *__ref__;                                                                          \
                                                                                                   \
         __ref__;                                                                                  \
       });)

/**
 * ::Vec<T>::reach_if(self, it, cond) -> Iter<it = T>
 */
#define u_vec_reach_if(self, it, cond)                                                             \
  u_vec_reach (self, it)                                                                           \
    if (cond)

/**
 * ::Vec<T>::reach_ref(self, it) -> Iter<it = const T*>
 */
#define u_vec_reach_ref(self, it)                                                                  \
  {                                                                                                \
    extern pub any_t __u_vec_reach(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                            \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_reach((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_->ref)){}; ({                                                    \
         extern pub any_t __u_vec_reach(any_t, bool);                                              \
                                                                                                   \
         it = __u_vec_reach((self)->ref, !!0);                                                     \
       });)

/**
 * ::Vec<T>::reach_if_ref(self, it, cond) -> Iter<it = const T*>
 */
#define u_vec_reach_if_ref(self, it, cond)                                                         \
  u_vec_reach_ref (self, it)                                                                       \
    if (cond)

/**
 * ::Vec<T>::reach_mut(self, it) -> Iter<it = T*>
 */
#define u_vec_reach_mut(self, it)                                                                  \
  {                                                                                                \
    extern pub any_t __u_vec_reach(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_->meta, "meta type not's Vec<T>");                            \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_reach((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_->mut)){}; ({                                                    \
         extern pub any_t __u_vec_reach(any_t, bool);                                              \
                                                                                                   \
         it = __u_vec_reach((self)->ref, !!0);                                                     \
       });)

/**
 * ::Vec<T>::reach_if_mut(self, it, cond) -> Iter<it = T*>
 */
#define u_vec_reach_if_mut(self, it, cond)                                                         \
  u_vec_reach_mut (self, it)                                                                       \
    if (cond)

/**
 * ::Vec<T>::find_if(self, cond) -> T
 */
#define u_vec_find_if(self, cond)                                                                  \
  ({                                                                                               \
    auto __val__ = (typeof(self->_->val)){};                                                       \
                                                                                                   \
    u_vec_each_if (self, it, cond) {                                                               \
      __val__ = (it);                                                                              \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __val__;                                                                                       \
  })

/**
 * ::Vec<T>::find_nif(self, cond) -> T
 */
#define u_vec_find_nif(self, cond) u_vec_find_if(self, !(cond))

/**
 * ::Vec<T>::find_if_ref(self, cond) -> const T*
 */
#define u_vec_find_if_ref(self, cond) ({ (typeof((self)->_->ref))u_vec_find_if_mut(self, cond); })

/**
 * ::Vec<T>::find_nif_ref(self, cond) -> const T*
 */
#define u_vec_find_nif_ref(self, cond) u_vec_find_if_ref(self, !(cond))

/**
 * ::Vec<T>::find_if_mut(self, cond) -> T*
 */
#define u_vec_find_if_mut(self, cond)                                                              \
  ({                                                                                               \
    auto __mut__ = (typeof((self)->_->mut)){};                                                     \
                                                                                                   \
    u_vec_each_if_mut (self, it, cond) {                                                           \
      __mut__ = it;                                                                                \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __mut__;                                                                                       \
  })

/**
 * ::Vec<T>::find_nif_mut(self, cond) -> T*
 */
#define u_vec_find_nif_mut(self, cond) u_vec_find_if_mut(self, !(cond))

/**
 * ::Vec<T>::rfind_if(self, cond) -> T
 */
#define u_vec_rfind_if(self, cond)                                                                 \
  ({                                                                                               \
    auto __val__ = (typeof((self)->_->val)){};                                                     \
                                                                                                   \
    u_vec_reach_if (self, it, cond) {                                                              \
      __val__ = (it);                                                                              \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __val__;                                                                                       \
  })

/**
 * ::Vec<T>::rfind_nif(self, cond) -> T
 */
#define u_vec_rfind_nif(self, cond) u_vec_rfind_if(self, !(cond))

/**
 * ::Vec<T>::rfind_if_ref(self, cond) -> const T*
 */
#define u_vec_rfind_if_ref(self, cond)                                                             \
  ({                                                                                               \
    ;                                                                                              \
    (typeof((self)->_->ref))u_vec_rfind_if_mut(self, cond);                                        \
  })

/**
 * ::Vec<T>::rfind_nif_ref(self, cond) -> const T*
 */
#define u_vec_rfind_nif_ref(self, cond) u_vec_rfind_if_ref(self, !(cond))

/**
 * ::Vec<T>::rfind_if_mut(self, cond) -> T*
 */
#define u_vec_rfind_if_mut(self, cond)                                                             \
  ({                                                                                               \
    auto __mut__ = (typeof((self)->_->mut)){};                                                     \
                                                                                                   \
    u_vec_reach_if_mut (self, it, cond) {                                                          \
      __mut__ = it;                                                                                \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __mut__;                                                                                       \
  })

/**
 * ::Vec<T>::rfind_nif_mut(self, cond) -> T*
 */
#define u_vec_rfind_nif_mut(self, cond) u_vec_rfind_if_mut(self, !(cond))

#if 0
#  define u_vec_map_by(self, block)                                                                \
    ({                                                                                             \
      typeof(self) __self__ = u_vec_new(self);                                                     \
                                                                                                   \
      u_vec_each (self, it) {                                                                      \
        block;                                                                                     \
                                                                                                   \
        u_vec_insert_back(__self__, it);                                                           \
      }                                                                                            \
                                                                                                   \
      __self__;                                                                                    \
    })

#  define u_vec_map_by_mut(self, block)                                                            \
    do {                                                                                           \
      for (i32_t __idx__ = {}; __idx__ < (self)->len; __idx__++) {                                 \
        auto it = u_vec_at(self, __idx__);                                                         \
        block;                                                                                     \
                                                                                                   \
        u_vec_at(self, __idx__, it);                                                               \
      }                                                                                            \
    } while (0)

#  define u_vec_filter_if(self, cond)                                                              \
    ({                                                                                             \
      typeof(self) __self__ = u_vec_new(self);                                                     \
                                                                                                   \
      u_vec_each (self, it) {                                                                      \
        if (cond)                                                                                  \
          u_vec_insert_back(__self__, it);                                                         \
      }                                                                                            \
                                                                                                   \
      __self__;                                                                                    \
    })

#  define u_vec_filter_if_mut(self, cond, ...)                                                     \
    do {                                                                                           \
      for (i32_t __idx__ = {}; __idx__ < (self)->len; __idx__++) {                                 \
        auto it = u_vec_at(self, __idx__);                                                         \
        if (cond) {                                                                                \
          __VA_ARGS__                                                                              \
                                                                                                   \
          $vec_del((self)->ref, __idx__--);                                                        \
        }                                                                                          \
      }                                                                                            \
    } while (0)
#endif

/**
 * ::Vec<T>::all_if(self, cond) -> bool
 */
#define u_vec_all_if(self, cond)                                                                   \
  ({                                                                                               \
    bool __result__ = !0;                                                                          \
                                                                                                   \
    u_vec_each_if (self, it, !(cond)) {                                                            \
      __result__ = !!0;                                                                            \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Vec<T>::all_if_ref(self, cond) -> bool
 */
#define u_vec_all_if_ref(self, cond)                                                               \
  ({                                                                                               \
    bool __result__ = !0;                                                                          \
                                                                                                   \
    u_vec_each_if_ref (self, it, !(cond)) {                                                        \
      __result__ = !!0;                                                                            \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Vec<T>::any_if(self, cond) -> bool
 */
#define u_vec_any_if(self, cond)                                                                   \
  ({                                                                                               \
    bool __result__ = !!0;                                                                         \
                                                                                                   \
    u_vec_each_if (self, it, cond) {                                                               \
      __result__ = !0;                                                                             \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Vec<T>::any_if_ref(self, cond) -> bool
 */
#define u_vec_any_if_ref(self, cond)                                                               \
  ({                                                                                               \
    bool __result__ = !!0;                                                                         \
                                                                                                   \
    u_vec_each_if_ref (self, it, cond) {                                                           \
      __result__ = !0;                                                                             \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_IVEC_H__ */
