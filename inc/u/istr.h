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

#ifndef U_ISTR_H__
#define U_ISTR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "utils/type.h"
#include "utils/va.h"

#include <u/istr.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef typeof(char*) u_cstr_t;

typedef struct {
}* u_str_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern u_str_t str_new();

extern size_t str_len(u_str_t);

extern u_cstr_t str_tocstr(u_str_t);

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#define us_len(s)                                                                                  \
  ({                                                                                               \
    size_t _len = 0;                                                                               \
                                                                                                   \
    if (__builtin_basetypeid(s) == 128) {                                                          \
      _len = strlen(s);                                                                            \
    } else if (__builtin_basetypeid(s) == 129) {                                                   \
      _len = str_len(s);                                                                           \
    }                                                                                              \
                                                                                                   \
    _len;                                                                                          \
  })

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_ISTR_H__ */
