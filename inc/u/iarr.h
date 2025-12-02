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

/* clang-format off */

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
/* linux array size macro */
#  define __same_type(a, b)      __builtin_types_compatible_p(typeof(a), typeof(b))
#  define __build_bug_on_zero(e) (sizeof(struct { int : -!!(e); }))
#  define __must_be_array(a)     __build_bug_on_zero(__same_type((a), &(a)[0]))
#  define u_arr_len(arr)         (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))


#  define u_arr_each(self, it)                                                                     \
    u_va_let(int, i, 0);                                                                           \
    for (auto it = self[u_va_var(i)]; u_va_var(i) < u_arr_len(self); u_va_var(i)++)


#  define u_arr_each_if(self, it, cond) u_arr_each(self, it) if (cond)


#  define u_arr_each_mut(self, it)                                                                 \
    u_va_let(int, i, 0);                                                                           \
    for (auto it = &self[u_va_var(i)]; u_va_var(i) < u_arr_len(self); u_va_var(i)++)


#  define u_arr_each_if_mut(self, it, cond) u_arr_each_mut(self, it) if (cond)


#  define u_arr_reach(self, it)                                                                    \
    u_va_let(int, i, u_arr_len(self)-1);                                                           \
    for (auto it = self[u_va_var(i)]; u_va_var(i) >= 0; u_va_var(i)--)


#  define u_arr_reach_if(self, it, cond) u_arr_reach(self, it) if (cond)


#  define u_arr_reach_mut(self, it)                                                                \
    u_va_let(int, i, u_arr_len(self)-1);                                                           \
    for (auto it = &self[u_va_var(i)]; u_va_var(i) >= 0; u_va_var(i)--)


#  define u_arr_reach_if_mut(self, it, cond) u_arr_reach_ref(self, it) if (cond)


#  define u_arr_find_if(self, cond)                                                                \
    ({                                                                                             \
      auto __val__ = (typeof(self[0])) {};                                                         \
                                                                                                   \
      u_arr_each_if(self, it, cond) { __val__ = (it); break; }                                     \
                                                                                                   \
      __val__;                                                                                     \
    })


#  define u_arr_find_nif(self, cond) u_arr_find_if(self, !(cond))


#  define u_arr_find_if_mut(self, cond)                                                            \
    ({                                                                                             \
      auto __mut__ = (typeof(&self[0])) {};                                                        \
                                                                                                   \
      u_arr_each_if_mut(self, it, cond) { __mut__ = (it); break; }                                 \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_arr_find_nif_mut(self, cond) u_arr_find_if_mut(self, !(cond))


#  define u_arr_rfind_if(self, cond)                                                               \
    ({                                                                                             \
      auto __val__ = (typeof(self[0])) {};                                                         \
                                                                                                   \
      u_arr_reach_if(self, it, cond) { __val__ = (it); break; }                                    \
                                                                                                   \
      __val__;                                                                                     \
    })


#  define u_arr_rfind_nif(self, cond) u_arr_rfind_if(self, !(cond))


#  define u_arr_rfind_if_mut(self, cond)                                                           \
    ({                                                                                             \
      auto __mut__ = (typeof(&self[0])) {};                                                        \
                                                                                                   \
      u_arr_reach_if_ref(self, it, cond) { __mut__ = (it); break; }                                \
                                                                                                   \
      __mut__;                                                                                     \
    })


#  define u_arr_rfind_nif_mut(self, cond) u_arr_rfind_if_mut(self, !(cond))


#  define u_arr_all_if(self, cond)                                                                 \
    ({                                                                                             \
      bool __result__ = !0;                                                                        \
                                                                                                   \
      u_arr_each_if(self, it, !(cond)) { __result__ = !!0; break; }                                \
                                                                                                   \
      __result__;                                                                                  \
    })


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
