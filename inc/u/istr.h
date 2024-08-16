/* MIT License
 *
 * Copyright (c) 2024 RunThem <iccy.fun@outlook.com>
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

#ifndef U_ISTR_H__
#  define U_ISTR_H__

#  ifdef __cplusplus
extern "C" {
#  endif

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct [[gnu::packed]] {
  const int len;
  const byte_t* ptr;
}* u_str_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
/* clang-format off */
extern u_str_t str_new();
extern void    str_clear       (u_str_t);
extern void    str_cleanup     (u_str_t);
extern void    str_slen        (u_str_t, int);

extern void    str_ins_str     (u_str_t, int, u_str_t);
extern void    str_ins_cstr    (u_str_t, int, u_cstr_t);
extern void    str_ins_char    (u_str_t, int, char);

extern int     str_cmp_str     (u_str_t, u_str_t);
extern int     str_cmp_cstr    (u_str_t, u_cstr_t);

extern void    str_2lower      (u_str_t);
extern void    str_2upper      (u_str_t);

extern void    str_ltrim       (u_str_t);
extern void    str_rtrim       (u_str_t);

extern bool    str_prefix_str  (u_str_t, u_str_t);
extern bool    str_prefix_cstr (u_str_t, u_cstr_t);
extern bool    str_prefix_char (u_str_t, char);

extern bool    str_suffix_str  (u_str_t, u_str_t);
extern bool    str_suffix_cstr (u_str_t, u_cstr_t);
extern bool    str_suffix_char (u_str_t, char);
/* clang-format on */

/***************************************************************************************************
 * iApi str
 **************************************************************************************************/
/* clang-format off */
#  define u_str_t(...)                                                                             \
    ({                                                                                             \
      u_str_t self = str_new();                                                                    \
                                                                                                   \
      u_va_if(u_va_has(__VA_ARGS__)) (                                                             \
        u_str_ins(self, 0, __VA_ARGS__);                                                           \
      )                                                                                            \
                                                                                                   \
      self;                                                                                        \
    })

#  define u_str_clear(self)                                                                        \
    do {                                                                                           \
      str_clear(self);                                                                             \
    } while (0)

#  define u_str_cleanup(self)                                                                      \
    do {                                                                                           \
      str_cleanup(self);                                                                           \
                                                                                                   \
      self = nullptr;                                                                              \
    } while (0)

#  define u_str_slen(self, len)                                                                    \
    do {                                                                                           \
      str_slen(self, len);                                                                         \
    } while (0)

#  define u_str_2lower(self)                                                                       \
    do {                                                                                           \
      str_2lower(self);                                                                            \
    } while (0)

#  define u_str_2upper(self)                                                                       \
    do {                                                                                           \
      str_2upper(self);                                                                            \
    } while (0)

#  define u_str_ltrim(self)                                                                       \
    do {                                                                                           \
      str_ltrim(self);                                                                            \
    } while (0)

#  define u_str_rtrim(self)                                                                       \
    do {                                                                                           \
      str_rtrim(self);                                                                            \
    } while (0)

#  define u_str_cmp(self, str)                                                                     \
    ({                                                                                             \
      auto __fn = _Generic(str, u_str_t: str_cmp_str, char*: str_cmp_cstr);                        \
                                                                                                   \
      __fn(self, str);                                                                             \
    })

#  define u_str_cat(self, ...)                                                                     \
    do {                                                                                           \
      u_str_ins(self, self->len, __VA_ARGS__);                                                     \
    } while (0)

#  define u_str_ins(self, idx, ...)                                                                \
    do {                                                                                           \
      u_va_elseif(u_va_cnt_is(1, __VA_ARGS__)) (                                                   \
        auto __fn = _Generic(__VA_ARGS__,                                                          \
            u_str_t: str_ins_str,                                                                  \
            char*: str_ins_cstr,                                                                   \
            int: str_ins_char,                                                                     \
            char: str_ins_char);                                                                   \
                                                                                                   \
        __fn(self, idx, __VA_ARGS__);                                                              \
      ) (                                                                                          \
        byte_t __buff[4096] = {0};                                                                 \
        snprintf(__buff, sizeof(__buff), __VA_ARGS__);                                             \
                                                                                                   \
        str_ins_cstr(self, idx, __buff);                                                           \
      )                                                                                            \
    } while (0)

#  define u_str_prefix(self, str)                                                                  \
    ({                                                                                             \
      auto __fn = _Generic(str,                                                                    \
          u_str_t: str_prefix_str,                                                                 \
          char*: str_prefix_cstr,                                                                  \
          int: str_prefix_char,                                                                    \
          char: str_prefix_char);                                                                  \
                                                                                                   \
      __fn(self, str);                                                                             \
    })

#  define u_str_suffix(self, str)                                                                  \
    ({                                                                                             \
      auto __fn = _Generic(str,                                                                    \
          u_str_t: str_suffix_str,                                                                 \
          char*: str_suffix_cstr,                                                                  \
          int: str_suffix_char,                                                                    \
          char: str_suffix_char);                                                                  \
                                                                                                   \
      __fn(self, str);                                                                             \
    })

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ISTR_H__ */
