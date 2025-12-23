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
 * doc:
 *    [](https://coolshell.cn/articles/8239.html)
 *    [](https://github.com/supermartian/lockfree-queue)
 *    [](https://github.com/golang-design/lockfree/blob/master/queue.go)
 *
 * */

#pragma once

#ifndef U_ILFQ_H__
#  define U_ILFQ_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/* clang-format off */

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern any_t $lfq_new     ();
extern void  $lfq_cleanup (any_t);
extern int   $lfq_len     (any_t);
extern bool  $lfq_put     (any_t, any_t);
extern any_t $lfq_pop     (any_t);

/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define u_lfq_t(T) typeof(T*)

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#  define u_lfq_new()                                                                              \
    ({                                                                                             \
      $lfq_new();                                                                                  \
    })


#  define u_lfq_len(self)                                                                          \
    ({                                                                                             \
      $lfq_len(self);                                                                              \
    })


#  define u_lfq_is_empty(self)                                                                     \
    ({                                                                                             \
      0 == $lfq_len(self);                                                                         \
    })


#  define u_lfq_cleanup(self)                                                                      \
    do {                                                                                           \
      assert(0 == $lfq_len(self));                                                                 \
                                                                                                   \
      $lfq_cleanup(self);                                                                          \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)


#  define u_lfq_pop(self)                                                                          \
    ({                                                                                             \
      assert(0 != $lfq_len(self));                                                                 \
                                                                                                   \
      (typeof(self)) $lfq_pop(self);                                                               \
    })


#  define u_lfq_put(self, obj)                                                                     \
    ({                                                                                             \
      $lfq_put(self, obj);                                                                         \
    })

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ILFQ_H__ */
