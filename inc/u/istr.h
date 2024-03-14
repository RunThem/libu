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

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern u_str_t str_new_char(char);
extern u_str_t str_new_cstr(u_cstr_t);
extern u_str_t str_new_str(u_str_t);

extern size_t str_len(u_str_t*);

extern void str_put_char(u_str_t*, char);
extern void str_put_cstr(u_str_t*, u_cstr_t);
extern void str_put_str(u_str_t*, u_str_t);

extern void str_ins_char(u_str_t*, size_t, char);
extern void str_ins_cstr(u_str_t*, size_t, u_cstr_t);
extern void str_ins_str(u_str_t*, size_t, u_str_t);

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
/* clang-format off */
#define us_new(s)                                                                                \
  ({                                                                                               \
    auto _fn = _Generic(s, int: str_new_char, u_cstr_t: str_new_cstr, u_str_t: str_new_str);       \
                                                                                                   \
    _fn(s);                                                                                        \
  })
/* clang-format on */

#define us_len(s)                                                                                  \
  ({                                                                                               \
    auto _fn = _Generic(s, u_cstr_t: strlen, u_str_t: str_len);                                    \
                                                                                                   \
    _fn(&s);                                                                                       \
  })

#define us_put(s, _s)                                                                              \
  do {                                                                                             \
    auto _fn = _Generic(_s, int: str_put_char, u_cstr_t: str_put_cstr, u_str_t: str_put_str);      \
                                                                                                   \
    _fn(&s, _s);                                                                                   \
  } while (0)

#define us_ins(s, i, _s)                                                                           \
  do {                                                                                             \
    auto _fn = _Generic(_s, int: str_ins_char, u_cstr_t: str_ins_cstr, u_str_t: str_ins_str);      \
                                                                                                   \
    _fn(&s, i, _s);                                                                                \
  } while (0)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* !U_ISTR_H__ */
