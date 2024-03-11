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

#ifndef U_ALLOC_H__
#define U_ALLOC_H__

#ifdef USE_MIMALLOC
#  include <mimalloc.h>

#  define u_free(p) mi_free(p)

#  define u_malloc(s)                                                                              \
    ({                                                                                             \
      any_t _ptr = mi_malloc(s);                                                                   \
      if (errno == 2)                                                                              \
        errno = 0;                                                                                 \
      _ptr;                                                                                        \
    })

#  define u_zalloc(s)                                                                              \
    ({                                                                                             \
      any_t _ptr = mi_zalloc(s);                                                                   \
      if (errno == 2)                                                                              \
        errno = 0;                                                                                 \
      _ptr;                                                                                        \
    })

#  define u_calloc(c, s)                                                                           \
    ({                                                                                             \
      any_t _ptr = mi_calloc(c, s);                                                                \
      if (errno == 2)                                                                              \
        errno = 0;                                                                                 \
      _ptr;                                                                                        \
    })

#  define u_realloc(p, s)                                                                          \
    ({                                                                                             \
      any_t _ptr = mi_realloc(p, s);                                                               \
      if (errno == 2)                                                                              \
        errno = 0;                                                                                 \
      _ptr;                                                                                        \
    })

#else

#  include <stdlib.h>

#  define u_free(p) free(p)

#  define u_malloc(s)     malloc(s)
#  define u_zalloc(s)     calloc(1, s)
#  define u_calloc(c, s)  calloc(c, s)
#  define u_realloc(p, s) realloc(p, s)

#endif

#endif /* !U_ALLOC_H__ */
