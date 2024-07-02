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

#ifndef U_ILFQ_H__
#define U_ILFQ_H__

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_lfq_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern any_t lfq_new();

extern void lfq_cleanup(any_t);

extern size_t lfq_len(any_t);

extern bool lfq_put(any_t, any_t);

extern void* lfq_pop(any_t);

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#define u_lfq_new()                                                                                \
  ({                                                                                               \
    u_lfq_t u = nullptr;                                                                           \
                                                                                                   \
    u = lfq_new();                                                                                 \
                                                                                                   \
    u;                                                                                             \
  })

#define u_lfq_len(u)                                                                               \
  ({                                                                                               \
    ;                                                                                              \
    lfq_len(u);                                                                                    \
  })

#define u_lfq_is_empty(u)                                                                          \
  ({                                                                                               \
    ;                                                                                              \
    0 == lfq_len(u);                                                                               \
  })

#define u_lfq_cleanup(u)                                                                           \
  do {                                                                                             \
    lfq_cleanup(u);                                                                                \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

#define u_lfq_pop(u)                                                                               \
  ({                                                                                               \
    ;                                                                                              \
    lfq_pop(u);                                                                                    \
  })

#define u_lfq_put(u, obj)                                                                          \
  ({                                                                                               \
    ;                                                                                              \
    lfq_put(u, obj);                                                                               \
  })

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_ILFQ_H__ */
