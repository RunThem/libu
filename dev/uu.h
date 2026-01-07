/* MIT License
 *
 * Copyright (c) 2026 - RunThem <iccy.fun@outlook.com>
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

#ifndef __UU_H__
#define __UU_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/***************************************************************************************************
 * Memory allocator
 **************************************************************************************************/
#if 0
#  define UU_MEMORY
#  define UU_MALLOC(size)
#  define UU_REALLOC(ptr, size)
#  define UU_FREE(ptr)
#endif

#ifndef UU_MEMORY
#  include <stdlib.h>
#  define UU_MALLOC(size)       malloc(size)
#  define UU_REALLOC(ptr, size) realloc(ptr, size)
#  define UU_FREE(ptr)          free(ptr)
#endif

/***************************************************************************************************
 * Vec
 **************************************************************************************************/
/**
 * ::Class Vec<T>
 */
#define uu_vec(T) __typeof__(T*)

/**
 * ::Vec<T>::init(self) -> Self
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);
    assert(v);
  }
  {
    uu_vec(int*) v = NULL;

    v = uu_vec_init(v);
    assert(v);
  }
 * ```
 */
#define uu_vec_init(self)                                                                          \
  ({                                                                                               \
    extern void* __uu_vec_init(uint32_t);                                                          \
                                                                                                   \
    self = (void*)__uu_vec_init(sizeof(*self));                                                    \
                                                                                                   \
    self;                                                                                          \
  })

/**
 * ::Vec<T>::deinit(self) -> !
 * ::Vec<T>::deinit(self, ...) -> !
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0); // {0}
    assert(1 == uu_vec_len(v));

    uu_vec_deinit(v);
    assert(v == NULL);
  }
  {
    uu_vec(int*) w = uu_vec_init(w);

    uu_vec_insert_tail(w, malloc(sizeof(int))); // {0xptr}
    assert(1 == uu_vec_len(w));

    uu_vec_deinit(w, { free(it); });
    assert(w == NULL);
  }
 * ```
 */
#define uu_vec_deinit(self, ...)                                                                   \
  do {                                                                                             \
    extern void __uu_vec_deinit(void*);                                                            \
                                                                                                   \
    uu_vec_each(self, it){__VA_ARGS__};                                                            \
                                                                                                   \
    __uu_vec_deinit(self);                                                                         \
                                                                                                   \
    self = NULL;                                                                                   \
  } while (0)

/**
 * ::Vec<T>::len(self) -> uint32_t
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);
    assert(0 == uu_vec_len(v));

    uu_vec_insert_tail(v, 0); // {0}
    assert(1 == uu_vec_len(v));
  }
 * ```
 */
#define uu_vec_len(self)                                                                           \
  ({                                                                                               \
    extern uint32_t __uu_vec_len(void*);                                                           \
                                                                                                   \
    __uu_vec_len(self);                                                                            \
  })

/**
 * ::Vec<T>::is_empty(self) -> bool
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);
    assert(uu_vec_is_empty(v));

    uu_vec_insert_tail(v, 0); // {0}
    assert(!uu_vec_is_empty(v));
  }
 * ```
 */
#define uu_vec_is_empty(self)                                                                      \
  ({                                                                                               \
    extern uint32_t __uu_vec_len(void*);                                                           \
                                                                                                   \
    0 == __uu_vec_len(self);                                                                       \
  })

/**
 * ::Vec<T>::swap(self, i, val) -> !
 * ::Vec<T>::swap_head(self, val) -> !
 * ::Vec<T>::swap_tail(self, val) -> !
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0); // {0}
    assert(0 == uu_vec_at_head(v));

    uu_vec_swap_head(v, 100);
    assert(100 == uu_vec_at_head(v));
  }
 * ```
 */
#define uu_vec_swap_head(self, _val) uu_vec_swap(self, 0, _val)
#define uu_vec_swap_tail(self, _val) uu_vec_swap(self, uu_vec_len(self) - 1, _val)
#define uu_vec_swap(self, _idx, _val)                                                              \
  do {                                                                                             \
    extern void* __uu_vec_at(void*, uint32_t);                                                     \
                                                                                                   \
    uint32_t __idx__          = _idx;                                                              \
    __typeof__(*self) __val__ = _val;                                                              \
    __typeof__(self) __mut__  = __uu_vec_at(self, __idx__);                                        \
                                                                                                   \
    *__mut__ = __val__;                                                                            \
  } while (0)

/**
 * ::Vec<T>::at(self, i) -> T
 * ::Vec<T>::at_head(self) -> T
 * ::Vec<T>::at_tail(self) -> T
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0); // {0}
    uu_vec_insert_tail(v, 1); // {0, 1}
    uu_vec_insert_tail(v, 2); // {0, 1, 2}
    assert(3 == uu_vec_len(v));

    assert(0 == uu_vec_at_head(v));
    assert(1 == uu_vec_at(v, 1));
    assert(2 == uu_vec_at_tail(v));
  }
 * ```
 */
#define uu_vec_at_head(self) uu_vec_at(self, 0)
#define uu_vec_at_tail(self) uu_vec_at(self, uu_vec_len(self) - 1)
#define uu_vec_at(self, _idx)                                                                      \
  ({                                                                                               \
    extern void* __uu_vec_at(void*, uint32_t);                                                     \
                                                                                                   \
    uint32_t __idx__         = _idx;                                                               \
    __typeof__(self) __mut__ = __uu_vec_at(self, __idx__);                                         \
                                                                                                   \
    *__mut__;                                                                                      \
  })

/**
 * ::Vec<T>::insert(self, i, val) -> !
 * ::Vec<T>::insert_head(self, val) -> !
 * ::Vec<T>::insert_tail(self, val) -> !
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    uu_vec_insert_head(v, 3); // {3, 0, 1, 2}
    uu_vec_insert(v, 1, 4);    // {3, 4, 0, 1, 2}
    uint32_t len = uu_vec_len(v);
    uu_vec_insert(v, len, 5);  // {3, 4, 0, 1, 2, 5}

    assert(3 == uu_vec_at(v, 0));
    assert(4 == uu_vec_at(v, 1));
    assert(0 == uu_vec_at(v, 2));
    assert(1 == uu_vec_at(v, 3));
    assert(2 == uu_vec_at(v, 4));
    assert(5 == uu_vec_at(v, 5));
  }
 * ```
 */
#define uu_vec_insert_head(self, _val) uu_vec_insert(self, 0, _val)
#define uu_vec_insert_tail(self, _val) uu_vec_insert(self, uu_vec_len(self), _val)
#define uu_vec_insert(self, _idx, _val)                                                            \
  do {                                                                                             \
    extern void* __uu_vec_insert(void*, uint32_t);                                                 \
                                                                                                   \
    uint32_t __idx__          = _idx;                                                              \
    __typeof__(*self) __val__ = _val;                                                              \
    __typeof__(self) __mut__  = __uu_vec_insert(self, __idx__);                                    \
                                                                                                   \
    *__mut__ = __val__;                                                                            \
  } while (0)

/**
 * ::Vec<T>::remove(self, i) -> !
 * ::Vec<T>::remove_head(self) -> !
 * ::Vec<T>::remove_tail(self) -> !
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    assert(3 == uu_vec_len(v));

    uu_vec_remove(v, 1);    // {0, 2}
    uu_vec_remove_tail(v);  // {0}
    uu_vec_remove_head(v); // {}
    assert(0 == uu_vec_len(v));
  }
 * ```
 */
#define uu_vec_remove_head(self) uu_vec_remove(self, 0)
#define uu_vec_remove_tail(self) uu_vec_remove(self, uu_vec_len(self) - 1)
#define uu_vec_remove(self, _idx)                                                                  \
  do {                                                                                             \
    extern void __uu_vec_remove(void*, uint32_t);                                                  \
                                                                                                   \
    uint32_t __idx__ = _idx;                                                                       \
                                                                                                   \
    __uu_vec_remove(self, __idx__);                                                                \
  } while (0)

/**
 * ::Vec<T>::each(self, it) -> Iter<T>
 * ::Vec<T>::each_if(self, it, cond) -> Iter<T>
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    assert(3 == uu_vec_len(v));

    int i = 0;
    uu_vec_each(v, it) {
      assert(i == it);
      i++;
    }

    assert(i == uu_vec_len(v));
  }
 * ```
 */
#define uu_vec_each_if(self, it, cond) uu_vec_each(self, it) if (cond)
#define uu_vec_each(self, it)                                                                      \
  {                                                                                                \
    extern void* __uu_vec_each(void*, int);                                                        \
    (void)__uu_vec_each(self, 1);                                                                  \
  };                                                                                               \
                                                                                                   \
  for (__typeof__(*self) it = {0}; ({                                                              \
         extern void* __uu_vec_each(void*, int);                                                   \
                                                                                                   \
         __typeof__(it)* __ref__ = __uu_vec_each(self, 2);                                         \
                                                                                                   \
         it = __ref__ ? *__ref__ : it;                                                             \
                                                                                                   \
         __ref__;                                                                                  \
       });)

/**
 * ::Vec<T>::reach(self, it) -> Iter<T>
 * ::Vec<T>::reach_if(self, it, cond) -> Iter<T>
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    assert(3 == uu_vec_len(v));

    int i = uu_vec_len(v) - 1;
    uu_vec_reach(v, it) {
      assert(i == it);
      i--;
    }

    assert(-1 == i);
  }
 * ```
 */
#define uu_vec_reach_if(self, it, cond) uu_vec_reach(self, it) if (cond)
#define uu_vec_reach(self, it)                                                                     \
  {                                                                                                \
    extern void* __uu_vec_each(void*, int);                                                        \
    (void)__uu_vec_each(self, 3);                                                                  \
  };                                                                                               \
                                                                                                   \
  for (__typeof__(*self) it = {0}; ({                                                              \
         extern void* __uu_vec_each(void*, int);                                                   \
                                                                                                   \
         __typeof__(it)* __ref__ = __uu_vec_each(self, 4);                                         \
                                                                                                   \
         it = __ref__ ? *__ref__ : it;                                                             \
                                                                                                   \
         __ref__;                                                                                  \
       });)

/**
 * ::Vec<T>::count_if(self, it, cond) -> uint32_t
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    assert(3 == uu_vec_len(v));

    uint32_t count = uu_vec_count_if(v, it, it % 2 == 0);
    assert(2 == count);

    count = uu_vec_count_if(v, it, it % 2 != 0);
    assert(1 == count);
  }
 * ```
 */
#define uu_vec_count_if(self, it, cond)                                                            \
  ({                                                                                               \
    uint32_t __cnt__ = 0;                                                                          \
    uu_vec_each_if(self, it, cond) __cnt__++;                                                      \
                                                                                                   \
    __cnt__;                                                                                       \
  })

/**
 * ::Vec<T>::find_if(self, it, cond) -> uint32_t | -1
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    uu_vec_insert_tail(v, 1);  // {0, 1, 2, 1}
    assert(4 == uu_vec_len(v));

    uint32_t idx = uu_vec_find_if(v, it, it == 1);
    assert(1 == idx);

    idx = uu_vec_find_if(v, it, it == 3);
    assert(-1 == idx);
  }
 * ```
 */
#define uu_vec_find_if(self, it, cond)                                                             \
  ({                                                                                               \
    int64_t __idx__ = 0;                                                                           \
                                                                                                   \
    uu_vec_each(self, it) {                                                                        \
      if (cond)                                                                                    \
        break;                                                                                     \
                                                                                                   \
      __idx__++;                                                                                   \
    }                                                                                              \
                                                                                                   \
    (__idx__ == uu_vec_len(self)) ? -1 : __idx__;                                                  \
  })

/**
 * ::Vec<T>::rfind_if(self, it, cond) -> uint32_t | -1
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    uu_vec_insert_tail(v, 1);  // {0, 1, 2, 1}
    assert(4 == uu_vec_len(v));

    uint32_t idx = uu_vec_rfind_if(v, it, it == 1);
    assert(3 == idx);

    idx = uu_vec_rfind_if(v, it, it == 3);
    assert(-1 == idx);
  }
 * ```
 */
#define uu_vec_rfind_if(self, it, cond)                                                            \
  ({                                                                                               \
    int64_t __idx__ = uu_vec_len(self);                                                            \
                                                                                                   \
    uu_vec_reach(self, it) {                                                                       \
      if (cond)                                                                                    \
        break;                                                                                     \
                                                                                                   \
      __idx__--;                                                                                   \
    }                                                                                              \
                                                                                                   \
    (__idx__ == 0) ? -1 : __idx__ - 1;                                                             \
  })

/**
 * ::Vec<T>::map_by(self, it, ...) -> !
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    assert(3 == uu_vec_len(v));

    uu_vec_map_by(v, it, { it = it * 2; });

    assert(0 == uu_vec_at(v, 0));
    assert(2 == uu_vec_at(v, 1));
    assert(4 == uu_vec_at(v, 2));
  }
 * ```
 */
#define uu_vec_map_by(self, it, ...)                                                               \
  do {                                                                                             \
    extern void* __uu_vec_each(void*, int);                                                        \
                                                                                                   \
    (void)__uu_vec_each(self, 1);                                                                  \
                                                                                                   \
    __typeof__(self) __mut__ = NULL;                                                               \
    for (__typeof__(*self) it = {0}; (__mut__ = __uu_vec_each(self, 2));) {                        \
      it = *__mut__;                                                                               \
                                                                                                   \
      __VA_ARGS__                                                                                  \
                                                                                                   \
      *__mut__ = it;                                                                               \
    }                                                                                              \
  } while (0)

/**
 * ::Vec<T>::any_if(self, it, cond) -> bool
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    assert(3 == uu_vec_len(v));

    int result = uu_vec_any_if(v, it, it >= 2);
    assert(result);

    result = uu_vec_any_if(v, it, it > 2);
    assert(!result);
  }
 * ```
 */
#define uu_vec_any_if(self, it, cond)                                                              \
  ({                                                                                               \
    int __result__ = !!0;                                                                          \
                                                                                                   \
    uu_vec_each_if(self, it, cond) {                                                               \
      __result__ = !0;                                                                             \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Vec<T>::all_if(self, it, cond) -> bool
 *
 * ```c
  {
    uu_vec(int) v = uu_vec_init(v);

    uu_vec_insert_tail(v, 0);  // {0}
    uu_vec_insert_tail(v, 1);  // {0, 1}
    uu_vec_insert_tail(v, 2);  // {0, 1, 2}
    assert(3 == uu_vec_len(v));

    int result = uu_vec_all_if(v, it, it >= 0);
    assert(result);

    result = uu_vec_all_if(v, it, it > 0);
    assert(!result);
  }
 * ```
 */
#define uu_vec_all_if(self, it, cond)                                                              \
  ({                                                                                               \
    int __result__ = !0;                                                                           \
                                                                                                   \
    uu_vec_each_if(self, it, !(cond)) {                                                            \
      __result__ = !!0;                                                                            \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __result__;                                                                                    \
  })

/***************************************************************************************************
 * Dict
 **************************************************************************************************/

#ifdef UU_DEBUG
#  define uu_dict_dump(self, dump_fn)                                                              \
    do {                                                                                           \
      extern void __uu_dict_dump(void*, uu_dict_dump_fn);                                          \
                                                                                                   \
      __uu_dict_dump(self, dump_fn);                                                               \
    } while (0)
#endif /* !UU_DEBUG */

/**
 * ::Class Dict<K: fn(K*, K*) -> int, V = void*>
 */
#define uu_dict(K, V) __typeof__(K*)


typedef int (*uu_dict_cmp_fn)(const void* k1, const void* k2);
typedef void (*uu_dict_dump_fn)(const void* key, const void* uptr);

/**
 * ::Dict<K, V = void*>::init(self) -> Self
 *
 * ```c
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);
    assert(d);
  }
  {
    uu_dict(int, int*) d = NULL;

    d = uu_dict_init(d, cmp_fn_int);
    assert(d);
  }
 * ```
 */
#define uu_dict_init(self, cmp_fn)                                                                 \
  ({                                                                                               \
    extern void* __uu_dict_init(uint32_t, uu_dict_cmp_fn);                                         \
                                                                                                   \
    self = (void*)__uu_dict_init(sizeof(*self), cmp_fn);                                           \
                                                                                                   \
    self;                                                                                          \
  })

/**
 * ::Dict<K, V = void*>::deinit(self) -> !
 *
 * ```c
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);
    assert(d);

    uu_dict_deinit(d);
    assert(!d);
  }
 * ```
 */
#define uu_dict_deinit(self, ...)                                                                  \
  do {                                                                                             \
    extern void __uu_dict_deinit(void*);                                                           \
                                                                                                   \
    uu_dict_each(self, key, void*, uptr){__VA_ARGS__};                                             \
                                                                                                   \
    __uu_dict_deinit(self);                                                                        \
                                                                                                   \
    self = NULL;                                                                                   \
  } while (0)

/**
 * ::Dict<K, V = void*>::len(self) -> uint32_t
 *
 * ```c
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);

    uu_dict_insert(d, 1, (void*)0x11);
    uu_dict_insert(d, 2, (void*)0x22);
    uu_dict_insert(d, 3, (void*)0x33);
    uu_dict_insert(d, 4, (void*)0x44);
    uu_dict_insert(d, 5, (void*)0x55);

    assert(5 == uu_dict_len(d));
  }
 * ```
 */
#define uu_dict_len(self)                                                                          \
  ({                                                                                               \
    extern uint32_t __uu_dict_len(void*);                                                          \
                                                                                                   \
    __uu_dict_len(self);                                                                           \
  })

/**
 * ::Dict<K, V = void*>::len(self) -> uint32_t
 *
 * ```c
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);

    assert(uu_dict_is_empty(d));

    uu_dict_insert(d, 1, (void*)0x11);
    uu_dict_insert(d, 2, (void*)0x22);
    uu_dict_insert(d, 3, (void*)0x33);
    uu_dict_insert(d, 4, (void*)0x44);
    uu_dict_insert(d, 5, (void*)0x55);

    assert(!uu_dict_is_empty(d));
  }
 * ```
 */
#define uu_dict_is_empty(self)                                                                     \
  ({                                                                                               \
    extern uint32_t __uu_dict_len(void*);                                                          \
                                                                                                   \
    0 == __uu_dict_len(self);                                                                      \
  })

/**
 * ::Dict<K, V = void*>::at(self, K) -> V
 *
 * ```c
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);

    uu_dict_insert(d, 1, (void*)0x11);
    uu_dict_insert(d, 2, (void*)0x22);
    uu_dict_insert(d, 3, (void*)0x33);
    uu_dict_insert(d, 4, (void*)0x44);
    uu_dict_insert(d, 5, (void*)0x55);

    assert((void*)0x11 == uu_dict_at(d, 1));
    assert((void*)0x22 == uu_dict_at(d, 2));
    assert((void*)0x33 == uu_dict_at(d, 3));
    assert((void*)0x44 == uu_dict_at(d, 4));
    assert((void*)0x55 == uu_dict_at(d, 5));
  }
 * ```
 */
#define uu_dict_at(self, _key)                                                                     \
  ({                                                                                               \
    extern void* __uu_dict_at(void*, void*);                                                       \
                                                                                                   \
    __typeof__(*self) __key__ = _key;                                                              \
                                                                                                   \
    __uu_dict_at(self, (void*)&__key__);                                                           \
  })

/**
 * ::Dict<K, V = void*>::remove(self, K) -> bool
 *
 * ```c
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);

    uu_dict_insert(d, 1, (void*)0x11);
    uu_dict_insert(d, 2, (void*)0x22);
    uu_dict_insert(d, 3, (void*)0x33);
    uu_dict_insert(d, 4, (void*)0x44);
    uu_dict_insert(d, 5, (void*)0x55);

    int result = uu_dict_remove(d, 3);
    assert(result);

    result = uu_dict_remove(d, 3);
    assert(!result);

    assert((void*)0x11 == uu_dict_at(d, 1));
    assert((void*)0x22 == uu_dict_at(d, 2));
    assert((void*)0x44 == uu_dict_at(d, 4));
    assert((void*)0x55 == uu_dict_at(d, 5));

    assert((void*)NULL == uu_dict_at(d, 3));

    assert(4 == uu_dict_len(d));
  }
 * ```
 */
#define uu_dict_remove(self, _key)                                                                 \
  ({                                                                                               \
    extern int __uu_dict_remove(void*, void*);                                                     \
                                                                                                   \
    __typeof__(*self) __key__ = _key;                                                              \
    int __result__            = __uu_dict_remove(self, (void*)&__key__);                           \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Dict<K, V = void*>::insert(self, K) -> bool
 *
 * ```c
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);

    int result = 0;

    result = uu_dict_insert(d, 1, (void*)0x11);
    assert(result);
    result = uu_dict_insert(d, 2, (void*)0x22);
    assert(result);
    result = uu_dict_insert(d, 3, (void*)0x33);
    assert(result);
    result = uu_dict_insert(d, 4, (void*)0x44);
    assert(result);
    result = uu_dict_insert(d, 5, (void*)0x55);
    assert(result);

    result = uu_dict_insert(d, 3, (void*)0x33);
    assert(!result);

    assert((void*)0x11 == uu_dict_at(d, 1));
    assert((void*)0x22 == uu_dict_at(d, 2));
    assert((void*)0x33 == uu_dict_at(d, 3));
    assert((void*)0x44 == uu_dict_at(d, 4));
    assert((void*)0x55 == uu_dict_at(d, 5));

    assert(5 == uu_dict_len(d));
  }
 * ```
 */
#define uu_dict_insert(self, _key, _uptr)                                                          \
  ({                                                                                               \
    extern void* __uu_dict_insert(void*, void*, void*);                                            \
                                                                                                   \
    int __result__            = !!0;                                                               \
    void* __uptr__            = _uptr;                                                             \
    __typeof__(*self) __key__ = _key;                                                              \
    __typeof__(self) __mut__  = __uu_dict_insert(self, (void*)&__key__, __uptr__);                 \
                                                                                                   \
    if (__mut__) {                                                                                 \
      *__mut__   = __key__;                                                                        \
      __result__ = !0;                                                                             \
    };                                                                                             \
                                                                                                   \
    __result__;                                                                                    \
  })

/**
 * ::Dict<K, V = void*>::each(self, key, typeof(K), uptr) -> Iter<K, V>
 * ::Dict<K, V = void*>::each_if(self, key, typeof(K), uptr) -> Iter<K, V>
 *
 * ```c
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);

    uu_dict_insert(d, 1, (void*)0x11);
    uu_dict_insert(d, 2, (void*)0x22);
    uu_dict_insert(d, 3, (void*)0x33);
    uu_dict_insert(d, 4, (void*)0x44);
    uu_dict_insert(d, 5, (void*)0x55);

    int cnt = 0;
    uu_dict_each(d, key, int*, uptr) {
      assert((void*)((uintptr_t)key * 0x11) == uptr);

      cnt++;
    }

    assert(cnt == uu_dict_len(d));
  }
 * ```
 */
#define uu_dict_each_if(self, key, type, uptr, cond) uu_dict_each(self, key, type, uptr) if (cond)
#define uu_dict_each(self, key, type, uptr)                                                        \
  {                                                                                                \
    extern int __uu_dict_each(void*, int, void* [2]);                                              \
    (void)__uu_dict_each(self, !0, NULL);                                                          \
  }                                                                                                \
                                                                                                   \
  for (__typeof__(*self) key = {0}, *__key__ = &key; __key__; __key__ = NULL)                      \
    for (type uptr = NULL; ({                                                                      \
           extern int __uu_dict_each(void*, int, void* [2]);                                       \
                                                                                                   \
           __typeof__(self) __out__[2] = {0};                                                      \
           int __result__              = __uu_dict_each(self, !!0, (void**)&__out__);              \
                                                                                                   \
           if (__result__) {                                                                       \
             key  = *__out__[0];                                                                   \
             uptr = __out__[1];                                                                    \
           }                                                                                       \
                                                                                                   \
           __result__;                                                                             \
         });)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !__UU_H__ */
