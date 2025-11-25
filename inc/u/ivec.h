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

#ifndef U_IVEC_H__
#  define U_IVEC_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/* clang-format off */

/***************************************************************************************************
 * Api
 **************************************************************************************************/
typedef struct {}* $vec_t;

extern any_t $vec_new     (i32_t, i32_t);
extern bool  $vec_resize  (any_t, i32_t);
extern void  $vec_clear   (any_t);
extern void  $vec_cleanup (any_t);
extern any_t $vec_at      (any_t, i32_t);
extern void  $vec_del     (any_t, i32_t);
extern any_t $vec_add     (any_t, i32_t);
extern any_t $vec_each    (any_t, bool);
extern any_t $vec_reach   (any_t, bool);

#if 0
extern void  vec_sort     (any_t, u_cmp_fn, u_order_e);
extern bool  vec_is_sort  (any_t, u_cmp_fn, u_order_e);
extern i64_t vec_pole     (any_t, u_cmp_fn, u_order_e);
extern bool  vec_each     (any_t, bool*, i64_t*, any_t*, u_order_e);
extern bool  vec_map_by   (any_t, bool*, i64_t*, any_t);
extern bool  vec_filter_by(any_t, bool*, i64_t*, any_t, bool*);
#endif

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_vec_t(T)                                                                               \
    typeof(const struct [[gnu::packed]] {                                                          \
      any_t ref;                                                                                   \
      int len;                                                                                     \
      int cap;                                                                                     \
                                                                                                   \
      struct {                                                                                     \
        $vec_t meta;                                                                               \
        T val;                                                                                     \
        const T * ref;                                                                             \
              T * mut;                                                                             \
      } _[0]; /* Don't use this field. */                                                          \
    }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_vec_new(self, ...)                                                                     \
    ({                                                                                             \
      typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                \
                                                                                                   \
      i32_t __cap__ = u_va_0th(16, __VA_ARGS__);                                                   \
      assert(0 < __cap__ && __cap__ <= INT32_MAX);                                                 \
                                                                                                   \
      self = $vec_new(sizeof(self->_[0].val), __cap__);                                            \
                                                                                                   \
      self->ref;                                                                                   \
    })


#  define u_vec_is_valid(self, _idx)                                                               \
    ({                                                                                             \
      typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                \
                                                                                                   \
      (bool) (0 <= _idx && _idx < self->len);                                                      \
    })


#  define u_vec_resize(self, _cap)                                                                 \
    ({                                                                                             \
      typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                \
                                                                                                   \
      assert(_cap > self->cap);                                                                    \
                                                                                                   \
      $vec_resize(self->ref, _cap);                                                                \
    })


#  define u_vec_clear(self, ...)                                                                   \
    do {                                                                                           \
      typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                \
                                                                                                   \
      u_va_has_if(__VA_ARGS__) ( u_vec_each(self, it) { __VA_ARGS__ } )                            \
                                                                                                   \
      $vec_clear(self->ref);                                                                       \
    } while (0)


#  define u_vec_cleanup(self, ...)                                                                 \
    do {                                                                                           \
      typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                \
                                                                                                   \
      u_va_has_if(__VA_ARGS__) ( u_vec_each(self, it) { __VA_ARGS__ } )                            \
                                                                                                   \
      $vec_cleanup(self->ref);                                                                     \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)


#  define u_vec_at(self, _i, ...)                                                                  \
    ({                                                                                             \
      *u_vec_at_mut(self, _i) u_va_has_if(__VA_ARGS__) ( = u_va_at(0, __VA_ARGS__) );              \
    })


#  define u_vec_at_ref(self, _i)                                                                   \
    ({                                                                                             \
      (typeof(self->_[0].ref)) u_vec_at_mut(self, _i);                                             \
    })                                                                                             \


#  define u_vec_at_mut(self, _i)                                                                   \
    ({                                                                                             \
      assert(u_vec_is_valid(self, _i));                                                            \
                                                                                                   \
      typeof(self->_[0].mut) __mut__ = $vec_at(self->ref, _i);                                     \
                                                                                                   \
      assert(__mut__);                                                                             \
                                                                                                   \
      __mut__;                                                                                     \
    })                                                                                             \


#  define u_vec_try_at(self, _i, ...)                                                              \
    u_va_let(typeof_unqual(self->_[0].ref), ref, $vec_at(self->ref, _i));                          \
    if (u_vec_is_valid(self, _i) && u_va_var(ref))                                                 \
      for (auto u_va_0th(it, __VA_ARGS__) = *u_va_var(ref); u_va_var(ref); u_va_var(ref) = nullptr)


#  define u_vec_try_at_ref(self, _i, ...)                                                          \
    u_va_let(typeof_unqual(self->_[0].ref), ref, $vec_at(self->ref, _i));                          \
    if (u_vec_is_valid(self, _i) && u_va_var(ref))                                                 \
      for (auto u_va_0th(it, __VA_ARGS__) = (typeof(self->_[0].ref)) u_va_var(ref); u_va_var(ref); u_va_var(ref) = nullptr)


#  define u_vec_try_at_mut(self, _i, ...)                                                          \
    u_va_let(typeof_unqual(self->_[0].mut), ref, $vec_at(self->ref, _i));                          \
    if (u_vec_is_valid(self, _i) && u_va_var(ref))                                                 \
      for (auto u_va_0th(it, __VA_ARGS__) = (typeof(self->_[0].mut)) u_va_var(ref); u_va_var(ref); u_va_var(ref) = nullptr)


#  define u_vec_remove(self, _i)                                                                   \
    ({                                                                                             \
      assert(u_vec_is_valid(self, _i));                                                            \
                                                                                                   \
      auto __val__ = u_vec_at(self, _i);                                                           \
                                                                                                   \
      $vec_del(self->ref, _i);                                                                     \
                                                                                                   \
      __val__;                                                                                     \
    })


#  define u_vec_remove_front(self) u_vec_remove(self, 0)


#  define u_vec_remove_back(self) u_vec_remove(self, self->len-1)


/*
 * [0, 1, 2, 3, 4, _]
 * */
#  define u_vec_insert(self, _i, _val)                                                             \
    do {                                                                                           \
      typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                \
                                                                                                   \
      assert(0 <= _i && _i <= self->len);                                                          \
                                                                                                   \
      typeof(self->_[0].mut) __mut__ = $vec_add(self->ref, _i);                                    \
                                                                                                   \
      *__mut__ = _val;                                                                             \
    } while (0)


#  define u_vec_insert_front(self, _val) u_vec_insert(self, 0, _val)


#  define u_vec_insert_back(self, _val) u_vec_insert(self, self->len, _val)


#  define u_vec_each(self, it)                                                                     \
    typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                  \
                                                                                                   \
    (void)$vec_each(self->ref, !0);                                                                \
    for (auto it = (typeof(self->_[0].val)) {}; ({ typeof(it)* __ref__ = $vec_each(self->ref, !!0); if (__ref__) it = *__ref__; __ref__; }); )


#  define u_vec_each_if(self, it, cond) u_vec_each(self, it) if (cond)


#  define u_vec_each_ref(self, it)                                                                 \
    typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                  \
                                                                                                   \
    (void)$vec_each(self->ref, !0);                                                                \
    for (auto it = (typeof(self->_[0].ref)) {}; (it = $vec_each(self->ref, !!0)); )


#  define u_vec_each_if_ref(self, it, cond) u_vec_each_ref(self, it) if (cond)


#  define u_vec_each_mut(self, it)                                                                 \
    typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                  \
                                                                                                   \
    (void)$vec_each(self->ref, !0);                                                                \
    for (auto it = (typeof(self->_[0].mut)) {}; (it = $vec_each(self->ref, !!0)); )


#  define u_vec_each_if_mut(self, it, cond) u_vec_each_mut(self, it) if (cond)


#  define u_vec_reach(self, it)                                                                    \
    typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                  \
                                                                                                   \
    (void)$vec_reach(self->ref, !0);                                                               \
    for (auto it = (typeof(self->_[0].val)) {}; ({ typeof(it)* __ref__ = $vec_reach(self->ref, !!0); if (__ref__) it = *__ref__; __ref__; }); )


#  define u_vec_reach_if(self, it, cond) u_vec_reach(self, it) if (cond)


#  define u_vec_reach_ref(self, it)                                                                \
    typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                  \
                                                                                                   \
    (void)$vec_reach(self->ref, !0);                                                               \
    for (auto it = (typeof(self->_[0].ref)) {}; (it = $vec_reach(self->ref, !!0)); )


#  define u_vec_reach_if_ref(self, it, cond) u_vec_reach_ref(self, it) if (cond)


#  define u_vec_reach_mut(self, it)                                                                \
    typecheck($vec_t, self->_[0].meta, "mete type not's Vec<T>");                                  \
                                                                                                   \
    (void)$vec_reach(self->ref, !0);                                                               \
    for (auto it = (typeof(self->_[0].mut)) {}; (it = $vec_reach(self->ref, !!0)); )


#  define u_vec_reach_if_mut(self, it, cond) u_vec_reach_mut(self, it) if (cond)


#  define u_vec_find_if(self, cond)                                                                \
    ({                                                                                             \
      auto __val__ = (typeof(self->_[0].val)) {};                                                  \
                                                                                                   \
      u_vec_each_if(self, it, cond) { __val__ = (it); break; }                                     \
                                                                                                   \
      __val__;                                                                                     \
    })


#  define u_vec_find_nif(self, cond) u_vec_find_if(self, !(cond))


#  define u_vec_find_if_ref(self, cond)                                                            \
    ({                                                                                             \
      (typeof(self->_[0].ref)) u_vec_find_if_mut(self, cond);                                      \
    })


#  define u_vec_find_nif_ref(self, cond) u_vec_find_if_ref(self, !(cond))


#  define u_vec_find_if_mut(self, cond)                                                            \
    ({                                                                                             \
      auto __mut__ = (typeof(self->_[0].mut)) {};                                                  \
                                                                                                   \
      u_vec_each_if_ref(self, it, cond) { __mut__ = (typeof_unqual(self->_[0].mut))(it); break; }  \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_vec_find_nif_mut(self, cond) u_vec_find_if_mut(self, !(cond))


#  define u_vec_rfind_if(self, cond)                                                               \
    ({                                                                                             \
      auto __val__ = (typeof(self->_[0].val)) {};                                                  \
                                                                                                   \
      u_vec_reach_if(self, it, cond) { __val__ = (it); break; }                                    \
                                                                                                   \
      __val__;                                                                                     \
    })


#  define u_vec_rfind_nif(self, cond) u_vec_rfind_if(self, !(cond))


#  define u_vec_rfind_if_ref(self, cond)                                                           \
    ({                                                                                             \
      (typeof(self->_[0].ref))u_vec_rfind_if_mut(self, cond);                                      \
    })


#  define u_vec_rfind_nif_ref(self, cond) u_vec_rfind_if_ref(self, !(cond))


#  define u_vec_rfind_if_mut(self, cond)                                                           \
    ({                                                                                             \
      auto __mut__ = (typeof(self->_[0].mut)) {};                                                  \
                                                                                                   \
      u_vec_reach_if_ref(self, it, cond) { __mut__ = (typeof_unqual(self->_[0].mut))(it); break; } \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_vec_rfind_nif_mut(self, cond) u_vec_rfind_if_mut(self, !(cond))


#  define u_vec_map_by(self, block)                                                                \
    ({                                                                                             \
      typeof(self) __self__ = u_vec_new(typeof(self->_[0].val));                                   \
                                                                                                   \
      u_vec_each(self, it) {                                                                       \
        block;                                                                                     \
                                                                                                   \
        u_vec_insert_back(__self__, it);                                                           \
      }                                                                                            \
                                                                                                   \
      __self__;                                                                                    \
    })


#  define u_vec_map_by_mut(self, block)                                                            \
    do {                                                                                           \
      for (i32_t __idx__ = {}; __idx__ < self->len; __idx__++) {                                   \
        auto it = u_vec_at(self, __idx__);                                                         \
        block;                                                                                     \
                                                                                                   \
        u_vec_at(self, __idx__, it);                                                               \
      }                                                                                            \
    } while (0)


#  define u_vec_filter_if(self, cond)                                                              \
    ({                                                                                             \
      typeof(self) __self__ = u_vec_new(typeof(self->_[0].val));                                   \
                                                                                                   \
      u_vec_each(self, it) {                                                                       \
        if (cond) u_vec_insert_back(__self__, it);                                                 \
      }                                                                                            \
                                                                                                   \
      __self__;                                                                                    \
    })


#  define u_vec_filter_if_mut(self, cond, ...)                                                     \
    do {                                                                                           \
      for (i32_t __idx__ = {}; __idx__ < self->len; __idx__++) {                                   \
        auto it = u_vec_at(self, __idx__);                                                         \
        if (cond) {                                                                                \
          __VA_ARGS__                                                                              \
                                                                                                   \
          $vec_del(self->ref, __idx__--);                                                          \
        }                                                                                          \
      }                                                                                            \
    } while (0)


#  define u_vec_all_if(self, cond)                                                                 \
    ({                                                                                             \
      bool __result__ = !0;                                                                        \
      u_vec_each_if(self, it, !(cond)) { __result__ = !!0; break; }                                \
                                                                                                   \
      __result__;                                                                                  \
    })


#  define u_vec_all_if_ref(self, cond)                                                             \
    ({                                                                                             \
      bool __result__ = !0;                                                                        \
      u_vec_each_if_ref(self, it, !(cond)) { __result__ = !!0; break; }                            \
                                                                                                   \
      __result__;                                                                                  \
    })


#  define u_vec_any_if(self, cond)                                                                 \
    ({                                                                                             \
      bool __result__ = !!0;                                                                       \
      u_vec_each_if(self, it, cond) { __result__ = !0; break; }                                    \
                                                                                                   \
      __result__;                                                                                  \
    })


#  define u_vec_any_if_ref(self, cond)                                                             \
    ({                                                                                             \
      bool __result__ = !!0;                                                                       \
      u_vec_each_if_ref(self, it, !(cond)) { __result__ = !0; break; }                             \
                                                                                                   \
      __result__;                                                                                  \
    })

/* clang-format on */

#  if 0
#    define u_vec_minidx(self, cmp_fn)                                                             \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        vec_pole(self, cmp_fn, U_ORDER_ASCEND);                                                    \
      })

#    define u_vec_maxidx(self, cmp_fn)                                                             \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        vec_pole(self, cmp_fn, U_ORDER_DESCEND);                                                   \
      })

#    define u_vec_min(self, cmp_fn)                                                                \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        i64_t __idx           = {};                                                                \
        u_types(self, 0)* __b = {};                                                                \
                                                                                                   \
        __idx = vec_pole(self, cmp_fn, U_ORDER_ASCEND);                                            \
        __b   = vec_at(self, __idx);                                                               \
                                                                                                   \
        *__b;                                                                                      \
      })

#    define u_vec_max(self, cmp_fn)                                                                \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        i64_t __idx           = {};                                                                \
        u_types(self, 0)* __b = {};                                                                \
                                                                                                   \
        __idx = vec_pole(self, cmp_fn, U_ORDER_DESCEND);                                           \
        __b   = vec_at(self, __idx);                                                               \
                                                                                                   \
        *__b;                                                                                      \
      })

#    define u_vec_sort(self, cmp_fn, ...)                                                          \
      do {                                                                                         \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        vec_sort(self, cmp_fn, u_va_0th(U_ORDER_ASCEND, __VA_ARGS__));                             \
      } while (0)

#    define u_vec_is_sort(self, cmp_fn, ...)                                                       \
      ({                                                                                           \
        u_check(self, 1, __u_vec_ref_t);                                                           \
                                                                                                   \
        vec_is_sort(self, cmp_fn, u_va_0th(U_ORDER_ASCEND, __VA_ARGS__));                          \
      })

#    define u_vec_map_by(self, fn)                                                                 \
      ({                                                                                           \
        typeof(self) __self = vec_new(sizeof(u_types(self, 0)));                                   \
                                                                                                   \
        for (i64_t i = 0; i < vec_len(self); i++) {                                                \
          u_types(self, 0)* it = vec_at(self, i);                                                  \
          u_types(self, 0) nit = fn(i, *it);                                                       \
          vec_put(__self, -1, &nit);                                                               \
        }                                                                                          \
                                                                                                   \
        __self;                                                                                    \
      })

#  endif

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IVEC_H__ */
