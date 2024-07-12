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

#ifndef U_ILFQ_H__
#define U_ILFQ_H__

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_lfq_ref_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern u_lfq_ref_t  lfq_new       ();
extern void         lfq_cleanup   (u_lfq_ref_t);
extern size_t       lfq_len       (u_lfq_ref_t);
extern bool         lfq_put       (u_lfq_ref_t, any_t);
extern void*        lfq_pop       (u_lfq_ref_t);
/* clang-format on */

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#define u_lfq_new()                                                                                \
  ({                                                                                               \
    ;                                                                                              \
    lfq_new();                                                                                     \
  })

#define u_lfq_len(self)                                                                            \
  ({                                                                                               \
    ;                                                                                              \
    lfq_len(self);                                                                                 \
  })

#define u_lfq_is_empty(self)                                                                       \
  ({                                                                                               \
    ;                                                                                              \
    0 == lfq_len(self);                                                                            \
  })

#define u_lfq_cleanup(self)                                                                        \
  do {                                                                                             \
    lfq_cleanup(self);                                                                             \
                                                                                                   \
    self = nullptr;                                                                                \
  } while (0)

#define u_lfq_pop(self)                                                                            \
  ({                                                                                               \
    ;                                                                                              \
    lfq_pop(self);                                                                                 \
  })

#define u_lfq_put(self, obj)                                                                       \
  ({                                                                                               \
    ;                                                                                              \
    lfq_put(self, obj);                                                                            \
  })

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_ILFQ_H__ */
