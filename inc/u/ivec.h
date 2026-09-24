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
 *
 * 连续内存的动态数组, 存值语义 (元素按值拷贝进数组)
 *
 *   u_vec_t(int) v = u_vec_new(v);   // 默认 cap == 16
 *   u_vec_insert_back(v, 1);         // v == [1]
 *   u_vec_insert_back(v, 3);         // v == [1, 3]
 *   u_vec_insert(v, 1, 2);           // 在 idx 1 处插入, v == [1, 2, 3]
 *
 *   u_vec_each_mut(v, it) {
 *     (*it) *= 10;                   // v == [10, 20, 30]
 *   }
 *
 *   u_vec_cleanup(v);                // 释放内存, 并把 v 置为 NULL
 *
 * 只读视图 (直接读, 不要写): v->ref / v->len / v->cap
 *
 * 约定:
 *   - 元素按值拷贝, T 需要能整体赋值
 *   - 插入可能触发 realloc (cap * 1.5), 之前从 at_mut / each_mut 拿到的指针全部失效
 *   - remove 用 memmove 补齐空位, 被删元素之后的元素会前移
 *   - idx 越界 / self 为 NULL 在 debug 下触发断言
 *   - clear 只把 len 置零并保留内存, cleanup 才释放内存
 *   - 迭代游标存在 Vec 内部: 同一条 Vec 不支持嵌套迭代, 迭代中不要增删
 *   - cleanup 不析构元素; T 里有需要释放的资源, 用 cleanup(self, proc) 自己处理
 */
#define u_vec_t(T)                                                                                 \
  typeof(struct {                                                                                  \
    const any_t ref;                                                                               \
    const int len;                                                                                 \
    const int cap;                                                                                 \
                                                                                                   \
    struct {                                                                                       \
      u_vec_meta_t meta;                                                                           \
                                                                                                   \
      int len_t;                                                                                   \
      int cap_t;                                                                                   \
      int idx_t;                                                                                   \
      T entry_t;                                                                                   \
      T* entry_mut_t;                                                                              \
      const T* entry_ref_t;                                                                        \
                                                                                                   \
      struct {                                                                                     \
        int cap;                                                                                   \
        any_t unused;                                                                              \
      } new_t;                                                                                     \
                                                                                                   \
      struct {                                                                                     \
        int cap;                                                                                   \
        any_t unused;                                                                              \
      } resize_t;                                                                                  \
                                                                                                   \
      struct {                                                                                     \
        int idx;                                                                                   \
        any_t unused;                                                                              \
      } at_t;                                                                                      \
                                                                                                   \
      struct {                                                                                     \
        int idx;                                                                                   \
        T entry;                                                                                   \
      } at_swap_t;                                                                                 \
                                                                                                   \
      struct {                                                                                     \
        int idx;                                                                                   \
        any_t unused;                                                                              \
      } remove_t;                                                                                  \
                                                                                                   \
      struct {                                                                                     \
        int idx;                                                                                   \
        T entry;                                                                                   \
      } insert_t;                                                                                  \
                                                                                                   \
    } _[0]; /* Don't use this field. */                                                            \
  }*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/

/**
 * ::Vec<T>::new(self) -> Self
 * ::Vec<T>::new(self, cap: int) -> Self
 *
 * 默认 cap == 16
 */
#define u_vec_new(self, ...)                                                                       \
  ({                                                                                               \
    extern pub any_t __u_vec_new(i32_t, i32_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
      u_va_has_if(__VA_ARGS__)(                                                                    \
          typecheck((self)->_[0].cap_t, u_va_at(0, __VA_ARGS__), "cap type not's int"));           \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.new_t) __cap__ = {u_va_0th(16, __VA_ARGS__), NULL};                                   \
                                                                                                   \
    (self) = __u_vec_new(sizeof(M.entry_t), __cap__.cap);                                          \
                                                                                                   \
    (self)->ref;                                                                                   \
  })

/**
 * ::Vec<T>::clear(self) -> !
 * ::Vec<T>::clear(self, proc: <block>) -> !
 *
 * 仅将 Self.len 置零, 保留内存
 */
#define u_vec_clear(self, ...)                                                                     \
  do {                                                                                             \
    extern pub void __u_vec_clear(any_t);                                                          \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
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
 * ::Vec<T>::cleanup(self, proc: <block>) -> !
 *
 * 释放数组和自身, 并把 Self 置为 NULL
 * 元素不析构, T 里有需要释放的资源就传 proc (proc 拿到的是可写视图)
 */
#define u_vec_cleanup(self, ...)                                                                   \
  do {                                                                                             \
    extern pub void __u_vec_cleanup(any_t);                                                        \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
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
 * ::Vec<T>::resize(self, cap: int) -> bool
 *
 * 仅允许扩容: cap 必须大于当前 cap (debug 断言), 不改变 Self.len
 */
#define u_vec_resize(self, _cap)                                                                   \
  ({                                                                                               \
    extern pub bool __u_vec_resize(any_t, i32_t);                                                  \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
      typecheck((self)->_[0].cap_t, _cap, "cap type not's int");                                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Cap = _cap;                                                                             \
      assert(Self->cap < Cap);                                                                     \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.resize_t) __cap__ = {_cap, NULL};                                                     \
                                                                                                   \
    __u_vec_resize((self)->ref, __cap__.cap);                                                      \
  })

/**
 * ::Vec<T>::at(self, idx: int) -> T
 * ::Vec<T>::at(self, idx: int, entry: T) -> T
 *
 * 只读形式返回当前值; 带 entry 的形式是写入, 返回写入后的值
 * idx 越界在 debug 下触发断言
 */
/* clang-format off */
#define u_vec_at(self, _idx, ...)                                                                  \
  ({                                                                                               \
    extern pub any_t __u_vec_at(any_t, i32_t);                                                     \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
      typecheck((self)->_[0].idx_t, _idx, "idx type not's int");                                   \
      u_va_has_if(__VA_ARGS__)(                                                                    \
          typecheck((self)->_[0].entry_t, u_va_at(0, __VA_ARGS__), "entry type not's T"));         \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Idx = _idx;                                                                             \
      assert(Idx >= 0 && Idx < Self->len);                                                         \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    u_va_elseif(u_va_has(__VA_ARGS__)) (                                                           \
      typeof(M.at_swap_t) __tuple__ = {_idx, __VA_ARGS__};                                         \
    )(                                                                                             \
      typeof(M.at_t) __tuple__ = {_idx, NULL};                                                     \
    );                                                                                             \
                                                                                                   \
    typeof(M.entry_mut_t) __entry_mut__ = __u_vec_at((self)->ref, __tuple__.idx);                  \
    assert(__entry_mut__);                                                                         \
                                                                                                   \
    *__entry_mut__ u_va_has_if(__VA_ARGS__)(= __tuple__.entry);                                    \
  })
/* clang-format on */

/**
 * ::Vec<T>::at_ref(self, idx: int) -> const T*
 *
 * 指向数组内部, 插入 / 扩容后失效
 */
#define u_vec_at_ref(self, _idx)                                                                   \
  ({                                                                                               \
    extern pub any_t __u_vec_at(any_t, i32_t);                                                     \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
      typecheck((self)->_[0].idx_t, _idx, "idx type not's int");                                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Idx = _idx;                                                                             \
      assert(Idx >= 0 && Idx < Self->len);                                                         \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __idx__              = {_idx, NULL};                                            \
    typeof(M.entry_ref_t) __entry_ref__ = __u_vec_at((self)->ref, __idx__.idx);                    \
    assert(__entry_ref__);                                                                         \
                                                                                                   \
    &__entry_ref__[0];                                                                             \
  })

/**
 * ::Vec<T>::at_mut(self, idx: int) -> T*
 *
 * 指向数组内部, 可直接改; 插入 / 扩容后失效
 */
#define u_vec_at_mut(self, _idx)                                                                   \
  ({                                                                                               \
    extern pub any_t __u_vec_at(any_t, i32_t);                                                     \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
      typecheck((self)->_[0].idx_t, _idx, "idx type not's int");                                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Idx = _idx;                                                                             \
      assert(Idx >= 0 && Idx < Self->len);                                                         \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.at_t) __idx__ = {_idx, NULL};                                                         \
                                                                                                   \
    typeof(M.entry_mut_t) __entry_mut__ = __u_vec_at((self)->ref, __idx__.idx);                    \
    assert(__entry_mut__);                                                                         \
                                                                                                   \
    &__entry_mut__[0];                                                                             \
  })

/**
 * ::Vec<T>::remove(self, idx: int) -> !
 *
 * 空位由后面的元素前移补齐 (memmove)
 */
#define u_vec_remove(self, _idx)                                                                   \
  do {                                                                                             \
    extern pub void __u_vec_del(any_t, i32_t);                                                     \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
      typecheck((self)->_[0].idx_t, _idx, "idx type not's int");                                   \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Idx = _idx;                                                                             \
      assert(Idx >= 0 && Idx < Self->len);                                                         \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.remove_t) __idx__ = {_idx, NULL};                                                     \
                                                                                                   \
    __u_vec_del((self)->ref, __idx__.idx);                                                         \
  } while (0)

/**
 * ::Vec<T>::remove_front(self) -> !
 */
#define u_vec_remove_front(self) u_vec_remove(self, 0)

/**
 * ::Vec<T>::remove_back(self) -> !
 */
#define u_vec_remove_back(self) u_vec_remove(self, (self)->len - 1)

/**
 * ::Vec<T>::insert(self, idx: int, entry: T) -> !
 *
 * idx == len 等价于 insert_back; 可能触发扩容 (cap * 1.5)
 */
#define u_vec_insert(self, _idx, _entry)                                                           \
  do {                                                                                             \
    extern pub any_t __u_vec_add(any_t, i32_t);                                                    \
                                                                                                   \
    {                                                                                              \
      typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                        \
      typecheck((self)->_[0].idx_t, _idx, "idx type not's int");                                   \
      typecheck((self)->_[0].entry_t, _entry, "entry type not's T");                               \
                                                                                                   \
      auto Self = (self);                                                                          \
      assert(Self != NULL);                                                                        \
                                                                                                   \
      auto Idx = _idx;                                                                             \
      assert(Idx >= 0 && Idx <= Self->len);                                                        \
    }                                                                                              \
                                                                                                   \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.insert_t) __tuple__        = {_idx, _entry};                                          \
    typeof(M.entry_mut_t) __entry_mut__ = __u_vec_add((self)->ref, __tuple__.idx);                 \
    assert(__entry_mut__);                                                                         \
                                                                                                   \
    *__entry_mut__ = __tuple__.entry;                                                              \
  } while (0)

/**
 * ::Vec<T>::insert_front(self, entry) -> !
 */
#define u_vec_insert_front(self, _entry) u_vec_insert(self, 0, _entry)

/**
 * ::Vec<T>::insert_back(self, entry) -> !
 */
#define u_vec_insert_back(self, _entry) u_vec_insert(self, (self)->len, _entry)

/**
 * ::Vec<T>::each(self, it: <var-name>) -> Iter<it = T>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_each(self, it)                                                                       \
  {                                                                                                \
    extern pub any_t __u_vec_each(any_t, bool);                                                    \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                          \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_each((self)->ref, !0);                                                           \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].entry_t)){}; ({                                              \
         extern pub any_t __u_vec_each(any_t, bool);                                               \
                                                                                                   \
         typeof((self)->_[0].entry_ref_t) __ref__ = __u_vec_each((self)->ref, !!0);                \
                                                                                                   \
         if (__ref__)                                                                              \
           it = *__ref__;                                                                          \
                                                                                                   \
         __ref__;                                                                                  \
       });)

/**
 * ::Vec<T>::each_if(self, it: <var-name>, cond: <expr>) -> Iter<it = T>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_each_if(self, it, cond)                                                              \
  u_vec_each (self, it)                                                                            \
    if (cond)

/**
 * ::Vec<T>::each_ref(self, it: <var-name>) -> Iter<it = const T*>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_each_ref(self, it)                                                                   \
  {                                                                                                \
    extern pub any_t __u_vec_each(any_t, bool);                                                    \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                          \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_each((self)->ref, !0);                                                           \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].entry_ref_t)){}; ({                                          \
         extern pub any_t __u_vec_each(any_t, bool);                                               \
                                                                                                   \
         it = __u_vec_each((self)->ref, !!0);                                                      \
       });)

/**
 * ::Vec<T>::each_if_ref(self, it: <var-name>, cond: <expr>) -> Iter<it = const T*>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_each_if_ref(self, it, cond)                                                          \
  u_vec_each_ref (self, it)                                                                        \
    if (cond)

/**
 * ::Vec<T>::each_mut(self, it: <var-name>) -> Iter<it = T*>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_each_mut(self, it)                                                                   \
  {                                                                                                \
    extern pub any_t __u_vec_each(any_t, bool);                                                    \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                          \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_each((self)->ref, !0);                                                           \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].entry_mut_t)){}; ({                                          \
         extern pub any_t __u_vec_each(any_t, bool);                                               \
                                                                                                   \
         it = __u_vec_each((self)->ref, !!0);                                                      \
       });)

/**
 * ::Vec<T>::each_if_mut(self, it: <var-name>, cond: <expr>) -> Iter<it = T*>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_each_if_mut(self, it, cond)                                                          \
  u_vec_each_mut (self, it)                                                                        \
    if (cond)

/**
 * ::Vec<T>::reach(self, it: <var-name>) -> Iter<it = T>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_reach(self, it)                                                                      \
  {                                                                                                \
    extern pub any_t __u_vec_reach(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                          \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_reach((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].entry_t)){}; ({                                              \
         extern pub any_t __u_vec_reach(any_t, bool);                                              \
                                                                                                   \
         typeof((self)->_[0].entry_ref_t) __ref__ = __u_vec_reach((self)->ref, !!0);               \
                                                                                                   \
         if (__ref__)                                                                              \
           it = *__ref__;                                                                          \
                                                                                                   \
         __ref__;                                                                                  \
       });)

/**
 * ::Vec<T>::reach_if(self, it: <var-name>, cond: <expr>) -> Iter<it = T>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_reach_if(self, it, cond)                                                             \
  u_vec_reach (self, it)                                                                           \
    if (cond)

/**
 * ::Vec<T>::reach_ref(self, it: <var-name>) -> Iter<it = const T*>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_reach_ref(self, it)                                                                  \
  {                                                                                                \
    extern pub any_t __u_vec_reach(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                          \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_reach((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].entry_ref_t)){}; ({                                          \
         extern pub any_t __u_vec_reach(any_t, bool);                                              \
                                                                                                   \
         it = __u_vec_reach((self)->ref, !!0);                                                     \
       });)

/**
 * ::Vec<T>::reach_if_ref(self, it: <var-name>, cond: <expr>) -> Iter<it = const T*>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_reach_if_ref(self, it, cond)                                                         \
  u_vec_reach_ref (self, it)                                                                       \
    if (cond)

/**
 * ::Vec<T>::reach_mut(self, it: <var-name>) -> Iter<it = T*>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_reach_mut(self, it)                                                                  \
  {                                                                                                \
    extern pub any_t __u_vec_reach(any_t, bool);                                                   \
                                                                                                   \
    typecheck(u_vec_meta_t, (self)->_[0].meta, "meta type not's Vec<T>");                          \
                                                                                                   \
    auto Self = (self);                                                                            \
    assert(Self != NULL);                                                                          \
                                                                                                   \
    (void)__u_vec_reach((self)->ref, !0);                                                          \
  }                                                                                                \
                                                                                                   \
  for (auto it = (typeof((self)->_[0].entry_mut_t)){}; ({                                          \
         extern pub any_t __u_vec_reach(any_t, bool);                                              \
                                                                                                   \
         it = __u_vec_reach((self)->ref, !!0);                                                     \
       });)

/**
 * ::Vec<T>::reach_if_mut(self, it: <var-name>, cond: <expr>) -> Iter<it = T*>
 *
 * 迭代游标存于 Vec 内部, 同一 Vec 不支持嵌套迭代
 */
#define u_vec_reach_if_mut(self, it, cond)                                                         \
  u_vec_reach_mut (self, it)                                                                       \
    if (cond)

/**
 * ::Vec<T>::find_if(self, cond: <expr>) -> T
 *
 * 未命中返回零值
 */
#define u_vec_find_if(self, cond)                                                                  \
  ({                                                                                               \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.entry_t) __entry__ = {};                                                              \
                                                                                                   \
    u_vec_each_if (self, it, cond) {                                                               \
      __entry__ = (it);                                                                            \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __entry__;                                                                                     \
  })

/**
 * ::Vec<T>::find_nif(self, cond: <expr>) -> T
 *
 * 未命中返回零值
 */
#define u_vec_find_nif(self, cond) u_vec_find_if(self, !(cond))

/**
 * ::Vec<T>::find_if_ref(self, cond: <expr>) -> const T*
 *
 * 未命中返回 NULL
 */
#define u_vec_find_if_ref(self, cond)                                                              \
  ({ (typeof((self)->_[0].entry_ref_t))u_vec_find_if_mut(self, cond); })

/**
 * ::Vec<T>::find_nif_ref(self, cond: <expr>) -> const T*
 *
 * 未命中返回 NULL
 */
#define u_vec_find_nif_ref(self, cond) u_vec_find_if_ref(self, !(cond))

/**
 * ::Vec<T>::find_if_mut(self, cond: <expr>) -> T*
 *
 * 未命中返回 NULL
 */
#define u_vec_find_if_mut(self, cond)                                                              \
  ({                                                                                               \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.entry_mut_t) __entry_mut__ = {};                                                      \
                                                                                                   \
    u_vec_each_if_mut (self, it, cond) {                                                           \
      __entry_mut__ = it;                                                                          \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __entry_mut__;                                                                                 \
  })

/**
 * ::Vec<T>::find_nif_mut(self, cond: <expr>) -> T*
 *
 * 未命中返回 NULL
 */
#define u_vec_find_nif_mut(self, cond) u_vec_find_if_mut(self, !(cond))

/**
 * ::Vec<T>::rfind_if(self, cond: <expr>) -> T
 *
 * 未命中返回零值
 */
#define u_vec_rfind_if(self, cond)                                                                 \
  ({                                                                                               \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.entry_t) __entry__ = {};                                                              \
                                                                                                   \
    u_vec_reach_if (self, it, cond) {                                                              \
      __entry__ = (it);                                                                            \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __entry__;                                                                                     \
  })

/**
 * ::Vec<T>::rfind_nif(self, cond: <expr>) -> T
 *
 * 未命中返回零值
 */
#define u_vec_rfind_nif(self, cond) u_vec_rfind_if(self, !(cond))

/**
 * ::Vec<T>::rfind_if_ref(self, cond: <expr>) -> const T*
 *
 * 未命中返回 NULL
 */
#define u_vec_rfind_if_ref(self, cond)                                                             \
  ({ (typeof((self)->_[0].entry_ref_t))u_vec_rfind_if_mut(self, cond); })

/**
 * ::Vec<T>::rfind_nif_ref(self, cond: <expr>) -> const T*
 *
 * 未命中返回 NULL
 */
#define u_vec_rfind_nif_ref(self, cond) u_vec_rfind_if_ref(self, !(cond))

/**
 * ::Vec<T>::rfind_if_mut(self, cond: <expr>) -> T*
 *
 * 未命中返回 NULL
 */
#define u_vec_rfind_if_mut(self, cond)                                                             \
  ({                                                                                               \
    typeof((self)->_[0]) M = {};                                                                   \
                                                                                                   \
    typeof(M.entry_mut_t) __entry_mut__ = {};                                                      \
                                                                                                   \
    u_vec_reach_if_mut (self, it, cond) {                                                          \
      __entry_mut__ = it;                                                                          \
      break;                                                                                       \
    }                                                                                              \
                                                                                                   \
    __entry_mut__;                                                                                 \
  })

/**
 * ::Vec<T>::rfind_nif_mut(self, cond: <expr>) -> T*
 *
 * 未命中返回 NULL
 */
#define u_vec_rfind_nif_mut(self, cond) u_vec_rfind_if_mut(self, !(cond))

/**
 * ::Vec<T>::map_by(self, proc: <block>) -> Self
 *
 * 返回新的 Vec, 原 Vec 不动 (两个都要自己 cleanup)
 * 在 proc 中修改 it 实现转换, 块返回值忽略
 */
#define u_vec_map_by(self, proc)                                                                   \
  ({                                                                                               \
    typeof(self) __self__ = u_vec_new(__self__, (self)->len);                                      \
                                                                                                   \
    u_vec_each (self, it) {                                                                        \
      proc;                                                                                        \
                                                                                                   \
      u_vec_insert_back(__self__, it);                                                             \
    }                                                                                              \
                                                                                                   \
    __self__;                                                                                      \
  })

/**
 * ::Vec<T>::filter_if(self, cond: <expr>) -> Self
 *
 * 返回新的 Vec, 原 Vec 不动 (两个都要自己 cleanup)
 */
#define u_vec_filter_if(self, cond)                                                                \
  ({                                                                                               \
    typeof(self) __self__ = u_vec_new(__self__, (self)->len);                                      \
                                                                                                   \
    u_vec_each_if (self, it, cond) {                                                               \
      u_vec_insert_back(__self__, it);                                                             \
    }                                                                                              \
                                                                                                   \
    __self__;                                                                                      \
  })

/**
 * ::Vec<T>::filter_if_ref(self, cond: <expr>) -> Self
 */
#define u_vec_filter_if_ref(self, cond)                                                            \
  ({                                                                                               \
    typeof(self) __self__ = u_vec_new(__self__, (self)->len);                                      \
                                                                                                   \
    u_vec_each_if_ref (self, it, cond) {                                                           \
      u_vec_insert_back(__self__, *it);                                                            \
    }                                                                                              \
                                                                                                   \
    __self__;                                                                                      \
  })

/**
 * ::Vec<T>::all_if(self, cond: <expr>) -> bool
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
 * ::Vec<T>::all_if_ref(self, cond: <expr>) -> bool
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
 * ::Vec<T>::any_if(self, cond: <expr>) -> bool
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
 * ::Vec<T>::any_if_ref(self, cond: <expr>) -> bool
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
