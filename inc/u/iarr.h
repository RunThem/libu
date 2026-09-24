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

#ifndef U_IARR_H__
#  define U_IARR_H__

#  ifdef __cplusplus
extern "C" {
#  endif

#  include "utils.h"

/* clang-format off */

/***************************************************************************************************
 * iType
 **************************************************************************************************/

/**
 * ::Class Array<T>
 *
 * 普通 C 数组上的迭代 / 查询宏: 没有句柄, 没有分配, 也没有生命周期管理
 *
 *   int a[] = {1, 2, 3};
 *
 *   u_arr_each(a, it) {           // it 是元素副本, 改它不影响数组
 *     printf("%d\n", it);
 *   }
 *
 *   u_arr_each_mut(a, it) {       // it 是 T*, 指向元素本身
 *     *it *= 10;
 *   }
 *
 *   int n = u_arr_len(a);                    // 编译期常量
 *   int* p = u_arr_find_if_mut(a, *it > 2);  // 未命中返回 NULL
 *   if (u_arr_any_if(a, it > 20)) { ... }
 *
 * 约定:
 *   - self 必须是真数组: 传指针会被 __must_be_array 编译期挡住 (数组做函数参数会退化成指针)
 *   - each / reach 给的是元素副本, 要写数组用 _mut 变体
 *   - 没有 _ref 变体: 数组这里值用 each, 指针用 _mut
 *   - 同一行只能展开一次迭代宏: 循环变量按行号命名 ($__<行号>i), 同一行会重定义
 *   - 依赖 GNU 扩展 (typeof / 语句表达式)
 */

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
/* linux array size macro */
#  define __same_type(a, b)      __builtin_types_compatible_p(typeof(a), typeof(b))
#  define __build_bug_on_zero(e) (sizeof(struct { int : -!!(e); }))
#  define __must_be_array(a)     __build_bug_on_zero(__same_type((a), &(a)[0]))
/**
 * ::Array<T>::len(arr) -> int
 *
 * 编译期常量; arr 必须是真数组, 传指针编译报错
 */
#  define u_arr_len(arr)         (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))


/**
 * ::Array<T>::each(self, it) -> Iter<it = T>
 *
 * 正序迭代, it 是元素副本
 */
#  define u_arr_each(self, it)                                                                     \
    u_va_let(int, i, 0);                                                                           \
    for (auto it = self[u_va_var(i)];                                                              \
         u_va_var(i) < (int)u_arr_len(self) && ((it = self[u_va_var(i)]), 1); u_va_var(i)++)


/**
 * ::Array<T>::each_if(self, it, cond) -> Iter<it = T>
 */
#  define u_arr_each_if(self, it, cond) u_arr_each(self, it) if (cond)


/**
 * ::Array<T>::each_mut(self, it) -> Iter<it = T*>
 *
 * 正序迭代, it 指向元素本身
 */
#  define u_arr_each_mut(self, it)                                                                 \
    u_va_let(int, i, 0);                                                                           \
    for (auto it = &self[u_va_var(i)];                                                             \
         u_va_var(i) < (int)u_arr_len(self) && ((it = &self[u_va_var(i)]), 1); u_va_var(i)++)


/**
 * ::Array<T>::each_if_mut(self, it, cond) -> Iter<it = T*>
 */
#  define u_arr_each_if_mut(self, it, cond) u_arr_each_mut(self, it) if (cond)


/**
 * ::Array<T>::reach(self, it) -> Iter<it = T>
 *
 * 倒序迭代, it 是元素副本
 */
#  define u_arr_reach(self, it)                                                                    \
    u_va_let(int, i, u_arr_len(self)-1);                                                           \
    for (auto it = self[u_va_var(i)];                                                              \
         u_va_var(i) >= 0 && ((it = self[u_va_var(i)]), 1); u_va_var(i)--)


/**
 * ::Array<T>::reach_if(self, it, cond) -> Iter<it = T>
 */
#  define u_arr_reach_if(self, it, cond) u_arr_reach(self, it) if (cond)


/**
 * ::Array<T>::reach_mut(self, it) -> Iter<it = T*>
 *
 * 倒序迭代, it 指向元素本身
 */
#  define u_arr_reach_mut(self, it)                                                                \
    u_va_let(int, i, u_arr_len(self)-1);                                                           \
    for (auto it = &self[u_va_var(i)];                                                             \
         u_va_var(i) >= 0 && ((it = &self[u_va_var(i)]), 1); u_va_var(i)--)


/**
 * ::Array<T>::reach_if_mut(self, it, cond) -> Iter<it = T*>
 */
#  define u_arr_reach_if_mut(self, it, cond) u_arr_reach_mut(self, it) if (cond)


/**
 * ::Array<T>::find_if(self, cond) -> T
 * ::Array<T>::find_nif(self, cond) -> T
 *
 * 正序查找第一个满足条件的元素, 未命中返回零值
 */
#  define u_arr_find_if(self, cond)                                                                \
    ({                                                                                             \
      auto __val__ = (typeof(self[0])) {};                                                         \
                                                                                                   \
      u_arr_each_if(self, it, cond) { __val__ = (it); break; }                                     \
                                                                                                   \
      __val__;                                                                                     \
    })


#  define u_arr_find_nif(self, cond) u_arr_find_if(self, !(cond))


/**
 * ::Array<T>::find_if_mut(self, cond) -> T*
 * ::Array<T>::find_nif_mut(self, cond) -> T*
 *
 * 正序查找, 未命中返回 NULL
 */
#  define u_arr_find_if_mut(self, cond)                                                            \
    ({                                                                                             \
      auto __mut__ = (typeof(&self[0])) {};                                                        \
                                                                                                   \
      u_arr_each_if_mut(self, it, cond) { __mut__ = (it); break; }                                 \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_arr_find_nif_mut(self, cond) u_arr_find_if_mut(self, !(cond))


/**
 * ::Array<T>::rfind_if(self, cond) -> T
 * ::Array<T>::rfind_nif(self, cond) -> T
 *
 * 倒序查找, 未命中返回零值
 */
#  define u_arr_rfind_if(self, cond)                                                               \
    ({                                                                                             \
      auto __val__ = (typeof(self[0])) {};                                                         \
                                                                                                   \
      u_arr_reach_if(self, it, cond) { __val__ = (it); break; }                                    \
                                                                                                   \
      __val__;                                                                                     \
    })


#  define u_arr_rfind_nif(self, cond) u_arr_rfind_if(self, !(cond))


/**
 * ::Array<T>::rfind_if_mut(self, cond) -> T*
 * ::Array<T>::rfind_nif_mut(self, cond) -> T*
 *
 * 倒序查找, 未命中返回 NULL
 */
#  define u_arr_rfind_if_mut(self, cond)                                                           \
    ({                                                                                             \
      auto __mut__ = (typeof(&self[0])) {};                                                        \
                                                                                                   \
      u_arr_reach_if_mut(self, it, cond) { __mut__ = (it); break; }                                \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_arr_rfind_nif_mut(self, cond) u_arr_rfind_if_mut(self, !(cond))


/**
 * ::Array<T>::all_if(self, cond) -> bool
 *
 * 空数组返回 true
 */
#  define u_arr_all_if(self, cond)                                                                 \
    ({                                                                                             \
      bool __result__ = !0;                                                                        \
                                                                                                   \
      u_arr_each_if(self, it, !(cond)) { __result__ = !!0; break; }                                \
                                                                                                   \
      __result__;                                                                                  \
    })


/**
 * ::Array<T>::any_if(self, cond) -> bool
 *
 * 空数组返回 false
 */
#  define u_arr_any_if(self, cond)                                                                 \
    ({                                                                                             \
      bool __result__ = !!0;                                                                       \
                                                                                                   \
      u_arr_each_if(self, it, cond) { __result__ = !0; break; }                                    \
                                                                                                   \
      __result__;                                                                                  \
    })

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_IARR_H__ */
