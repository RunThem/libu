#pragma once

#include "u.h"

typedef const char* u_str_t;
typedef char* u_mut_str_t;

void stbl_init(void);
void stbl_cleanup(void);

u_str_t str_from(u_str_t str);

u_str_t str_fromf(u_str_t fmt, ...);

void str_cleanup(u_str_t str);

#if 0

/* clang-format off */
extern size_t __str_char_size(char);
extern size_t __str_c_str_size(c_str);
extern size_t __str_string_size(str_t);

extern c_str  __str_char_start(char*);
extern c_str  __str_c_str_start(c_str*);
extern c_str  __str_string_start(str_t*);

#define __str_size(s)                                                                              \
  _Generic(s,                                                                                      \
           char:               __str_char_size,                                                    \
           int:                __str_char_size,                                                    \
           char*:              __str_c_str_size,                                                   \
           const char*:        __str_c_str_size,                                                   \
           str_t:              __str_string_size                                                   \
  )(s)

#define __str_start(s)                                                                             \
  ({                                                                                               \
    char __eof = '\0';                                                                             \
    auto __str = (s);                                                                              \
    (void)__eof;                                                                                   \
    _Generic(s,                                                                                    \
             char:               __str_char_start,                                                 \
             int:                __str_char_start,                                                 \
             char*:              __str_c_str_start,                                                \
             const char* :       __str_c_str_start,                                                \
             str_t:              __str_string_start                                                \
    )((void*)&__str); \
  })
/* clang-format on */

#  define str_isinit(str)        ((str)->c_str == nullptr)
#  define str_len(str)           ((str)->len)
#  define str_cap(str)           ((str)->cap)
#  define str_cstr(str)          ((str)->c_str)
#  define str_empty(str)         ((str)->len == 0)
#  define str_clear(str)         ((str)->len = 0)

str_t __str_from(c_str cstr, size_t len);
#  define str_from(cstr, arg...) __str_from(__str_start(cstr), va_0th(__str_size(cstr), arg))

str_t __str_fromf(c_str fmt, ...);
#  define str_fromf(fmt, arg...) __str_fromf(__str_start(fmt) va_opt(0, arg) arg)

ret_t __str_init(any_t _self, size_t cap);
#  define str_init(str, cap)     __str_init(str, cap)

ret_t __str_resize(any_t _self, size_t cap);
#  define str_resize(str, cap)   __str_resize(str, cap)

char __str_at(any_t _self, size_t idx);
#  define str_at(str, idx)       __str_at(str, idx)
#  define str_at_f(str)          __str_at(str, 0)
#  define str_at_b(str)          __str_at(str, (str)->len - 1)

ret_t __str_push(any_t _self, size_t idx, c_str cstr, size_t len);
#  define str_push(str, idx, cstr, arg...)                                                         \
    __str_push(str, idx, __str_start(cstr), va_0th(__str_size(cstr), arg))
#  define str_push_f(str, cstr, arg...)                                                            \
    __str_push(str, 0, __str_start(cstr), va_0th(__str_size(cstr), arg))
#  define str_push_b(str, cstr, arg...)                                                            \
    __str_push(str, (str)->len, __str_start(cstr), va_0th(__str_size(cstr), arg))

ret_t __str_pushf(any_t _self, size_t idx, c_str fmt, ...);
#  define str_pushf(str, idx, cstr, arg...) __str_pushf(str, idx, __str_start(cstr), arg)
#  define str_pushf_f(str, cstr, arg...)    __str_pushf(str, 0, __str_start(cstr), arg)
#  define str_pushf_b(str, cstr, arg...)    __str_pushf(str, (str)->len, __str_start(cstr), arg)

ret_t __str_erase(any_t _self, size_t idx, size_t len);
#  define str_erase(str, idx, len)          __str_erase(str, idx, len)
#  define str_erase_f(str, idx)             __str_erase(str, 0, idx)
#  define str_erase_b(str, idx)             __str_erase(str, idx, (str)->len - (idx))

ret_t __str_cleanup(any_t _self);
#  define str_cleanup(str)                  __str_cleanup(str)

str_t __str_clone(any_t _self);
#  define str_clone(str)                    __str_clone(str)

bool __str_cmp(any_t _self, c_str cstr, size_t len);
#  define str_cmp(str, cstr, arg...)                                                               \
    __str_cmp(str, __str_start(cstr), va_0th(__str_size(cstr), arg))

ssize_t __str_find(any_t _self, c_str cstr, size_t len);
#  define str_find(str, cstr, arg...)                                                              \
    __str_find(str, __str_start(cstr), va_0th(__str_size(cstr), arg))

bool __str_prefix(any_t _self, c_str cstr, size_t len);
#  define str_prefix(str, cstr, arg...)                                                            \
    __str_prefix(str, __str_start(cstr), va_0th(__str_size(cstr), arg))

bool __str_subfix(any_t _self, c_str cstr, size_t len);
#  define str_subfix(str, cstr, arg...)                                                            \
    __str_subfix(str, __str_start(cstr), va_0th(__str_size(cstr), arg))

size_t __str_count(any_t _self, c_str cstr, size_t len);
#  define str_count(str, cstr, arg...)                                                             \
    __str_count(str, __str_start(cstr), va_0th(__str_size(cstr), arg))

#endif
