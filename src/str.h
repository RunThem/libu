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
 * assert(12 == str_len(str));
 * */
#define str_len(s) __str_size(s)

/*
 * 返回新建字符串并赋值, len 必须小于或等于 str_len(c_str)
 *
 * str_t str = str_new("hello");
 *
 * assert(5 == str->len);
 *
 * str_t str = str_new("hello world", 5);
 *
 * assert(5 == str->len);
 * assert(!strncmp("hello", str->c_str, 5));
 * */
str_t __str_new(c_str c_string, size_t len);
#define str_new(s, arg...) __str_new(__str_start(s), va_0th(__str_size(s), arg))

/*
 * 与 `str_new` 类型, 只是是以格式化字符串的形式创建一个字符串
 *
 * str_t str = str_new_f("hell%c", 'o');
 *
 * assert(5 == str->len);
 * assert(!strncmp("hello", str->c_str, 5));
 */
str_t __str_new_f(c_str fmt, ...);
#define str_new_f(fmt, arg...) __str_new_f(__str_start(fmt), arg)

/*
 * 清除字符串中内容, 使长度为0
 *
 * str_t str = str_new("hello");
 *
 * str_clear(&str);
 *
 * assert(0 == str->len);
 * assert('\0' == str->c_str[0]);
 * */
void __str_clear(str_t* str);
#define str_clear(s) __str_clear(s)

/*
 * 释放字符串
 *
 * str_t str = str_new("hello");
 *
 * str_cleanup(&str);
 *
 * assert(nullptr == str);
 * */
void __str_cleanup(str_t* str);
#define str_cleanup(s) __str_cleanup(s)
