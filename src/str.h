#pragma once

#include "u.h"

typedef char* c_str;
struct __str {
  size_t len;
  size_t cap;
  char c_str[0];
};

typedef struct __str* str_t;

/* clang-format off */
size_t __str_char_size(char c);
size_t __str_c_str_size(c_str s);
size_t __str_string_size(str_t s);
#define __str_size(s)                                                                              \
  _Generic(s,                                                                                      \
           char:               __str_char_size,                                                    \
           char*:              __str_c_str_size,                                                   \
           const char*:        __str_c_str_size,                                                   \
           struct __str*:      __str_string_size                                                   \
  )(s)

c_str __str_char_start(void* c);
c_str __str_c_str_start(void* s);
c_str __str_string_start(void* s);
#define __str_start(s)                                                                             \
  ({                                                                                               \
    auto __str = (s);                                                                              \
    _Generic(s,                                                                                    \
             char:               __str_char_start,                                                 \
             char*:              __str_c_str_start,                                                \
             const char* :       __str_c_str_start,                                                \
             struct __str*:      __str_string_start                                                \
    )((void*)&__str); \
  })
/* clang-format on */

/*
 * 返回c字符, c字符数组, str_t的长度
 *
 * assert(1 == str_len(c('a')));
 *
 * assert(5 == str_len("hello"));
 *
 * str_t str = str_new("hello world.");
 *
 * assert(12 == str_new(str));
 * */
#define str_len(s) __str_size(s)
