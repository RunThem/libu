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
 * code:
 *    assert(1 == str_len(c('a')));
 *
 * code:
 *    assert(5 == str_len("hello"));
 *
 * code:
 *    str_t str = str_new("hello world.");
 *
 *    assert(12 == str_len(str));
 * */
#define str_len(s) __str_size(s)

/*
 * 返回新建字符串并赋值, len 必须小于或等于 str_len(c_str)
 *
 * code
 *    str_t str = str_new("hello");
 *
 *    assert(5 == str->len);
 *
 * code:
 *    str_t str = str_new("hello world", 5);
 *
 *    assert(5 == str->len);
 *    assert(!strncmp("hello", str->c_str, 5));
 * */
str_t __str_new(c_str c_string, size_t len);
#define str_new(s, arg...) __str_new(__str_start(s), va_0th(__str_size(s), arg))

/*
 * 与 `str_new` 类型, 只是是以格式化字符串的形式创建一个字符串
 *
 * code:
 *    str_t str = str_new_f("hell%c", 'o');
 *
 *    assert(5 == str->len);
 *    assert(!strncmp("hello", str->c_str, 5));
 */
str_t __str_new_f(c_str fmt, ...);
#define str_new_f(fmt, arg...) __str_new_f(__str_start(fmt), arg)

/*
 * 清除字符串中内容, 使长度为0
 *
 * code:
 *    str_t str = str_new("hello");
 *
 *    str_clear(&str);
 *
 *    assert(0 == str->len);
 *    assert('\0' == str->c_str[0]);
 * */
void __str_clear(str_t* str);
#define str_clear(s) __str_clear(s)

/*
 * 释放字符串
 *
 * code:
 *    str_t str = str_new("hello");
 *
 *    str_cleanup(&str);
 *
 *    assert(nullptr == str);
 * */
void __str_cleanup(str_t* str);
#define str_cleanup(s) __str_cleanup(s)

/*
 * 判断字符串是否为空
 *
 * code:
 *    str_t str = str_new("");
 *
 *    assert(true == str_empty(&str));
 *
 * code:
 *    str_t str = str_new("hello");
 *
 *    assert(false == str_empty(&str));
 * */
bool __str_empty(str_t* str);
#define str_empty(s) __str_empty(s)

/*
 * 扩容字符串, len 必须大于 str_len(str)
 *
 * code:
 *    str_t str = str_new("");
 *
 *    str_resize(&str, 100);
 *
 *    assert(100 == str->cap);
 * */
int __str_resize(str_t* str, size_t len);
#define str_resize(s, len) __str_resize(s, len)

/*
 * 访问字符串中的某个字符
 *
 * code:
 *    str_t str = str_new("hello");
 *
 *    auto c = str_at(&str, 2);
 *
 *    assert('l' == *c);
 * */
c_str __str_at(str_t* str, size_t idx);
#define str_at(s, idx) __str_at(s, idx)

/*
 * 在字符串结尾追加字符串
 *
 * code:
 *    str_t str = str_new("hello");
 *
 *    str_push(&str, " world.");
 *
 *    assert(!strncmp("hello world.", str->c_str, str->len));
 * */
int __str_push(str_t* str, c_str c_string, size_t size);
#define str_push(s, c_s, arg...) __str_push(s, __str_start(c_s), va_0th(__str_size(c_s), arg))

/*
 * 与 `str_push` 类似
 *
 * code:
 *    str_t str = str_new("hello");
 *
 *    str_push_f(&str, " %s.", "world");
 *
 *    assert(!strncmp("hello world.", str->c_str, str->len));
 * */
int __str_push_f(str_t* str, c_str fmt, ...);
#define str_push_f(s, fmt, arg...) __str_push_f(s, __str_start(fmt) va_slice(0, arg))

/*
 * 在字符串中间插入字符串
 *
 * code:
 *    str_t str = str_new("hello");
 *
 *    str_insert(&str, 2, " world.");
 *
 *    assert(!strncmp("he world.llo", str->c_str, str->len));
 * */
int __str_insert(str_t* str, size_t idx, c_str c_string, size_t size);
#define str_insert(s, idx, c_s, arg...)                                                            \
  __str_insert(s, idx, __str_start(c_s), va_0th(__str_size(c_s), arg))

/*
 * 与 `str_insert` 类似
 *
 * code:
 *    str_t str = str_new("hello");
 *
 *    str_insert_f(&str, 2, " %s.", "world");
 *
 *    assert(!strncmp("he world.llo", str->c_str, str->len));
 * */
int __str_insert_f(str_t* str, size_t idx, c_str fmt, ...);
#define str_insert_f(s, idx, fmt, arg...) __str_insert_f(s, idx, __str_start(fmt) va_slice(0, arg))

/*
 * 删除字符串中某一部分
 *
 * code:
 *    str_t str = str_new("hello, world");
 *
 *    str_erase(&str, 5);
 *
 *    assert(!strncmp("hello world", str->c_str, str->len));
 *
 * code:
 *    str_t str = str_new("hello, world");
 *
 *    str_erase(&str, 5, 2);
 *
 *    assert(!strncmp("helloworld", str->c_str, str->len));
 *
 * code:
 *    str_t str = str_new("hello, world");
 *
 *    str_erase(&str, 5, 100);
 *
 *    assert(!strncmp("hello", str->c_str, str->len));
 *
 * code:
 *    str_t str = str_new("hello, world");
 *
 *    str_erase(&str, 5, -1);
 *
 *    assert(!strncmp(", world", str->c_str, str->len));
 *
 * code:
 *    str_t str = str_new("hello, world");
 *
 *    str_erase(&str, 5, -2);
 *
 *    assert(!strncmp("hello", str->c_str, str->len));
 *
 * */
int __str_erase(str_t* str, size_t idx, ssize_t len);
#define str_erase(s, idx, arg...) __str_erase(s, idx, va_0th(1, arg))

/*
 * 比较两个字符串
 *
 * code:
 *    str_t str = str_new("hello");
 *    str_t str_1 = str_new("hello");
 *
 *    assert(!str_comp(&str, "hello"));
 *    assert(!str_comp(&str, str_1));
 * */
int __str_comp(str_t* str, c_str c_string, size_t len);
#define str_comp(s, c_s) __str_comp(s, __str_start(c_s), __str_size(c_s))

/*
 * 在字符串中查照
 *
 * code:
 *    str_t str = str_new("hello world");
 *
 *    assert(4 == str_find(&str, "o"));
 *    assert(7 == str_find(&str, "o", 5));
 * */
ssize_t __str_find(str_t* str, c_str c_string, size_t len, size_t idx);
#define str_find(s, c_s, arg...) __str_find(s, __str_start(c_s), __str_size(c_s), va_0th(0, arg))

/*
 * 判断字符串前缀
 *
 * code:
 *    str_t str = str_new("hello world");
 *
 *    assert(str_prefix(&str, "hello"));
 *    assert(!str_prefix(&str, "help"));
 * */
bool __str_prefix(str_t* str, c_str c_string, size_t len);
#define str_prefix(s, c_s, arg...) __str_prefix(s, __str_start(c_s), va_0th(__str_size(c_s), arg))

/*
 * 判断字符串后缀
 *
 * code:
 *    str_t str = str_new("hello world");
 *
 *    assert(str_suffix(&str, "world"));
 *    assert(!str_suffix(&str, "word"));
 * */
bool __str_suffix(str_t* str, c_str c_string, size_t len);
#define str_suffix(s, c_s, arg...) __str_suffix(s, __str_start(c_s), va_0th(__str_size(c_s), arg))

/*
 * 判断字符串是否包含另一个字符串
 *
 * code:
 *    str_t str = str_new("hello world");
 *
 *    assert(str_contain(&str, "world"));
 *    assert(!str_contain(&str, "word"));
 * */
bool __str_contain(str_t* str, c_str c_string, size_t len);
#define str_contain(s, c_s, arg...) __str_contain(s, __str_start(c_s), va_0th(__str_size(c_s), arg))

/*
 * 统计字符串包含子串个数
 *
 * code:
 *    str_t str = str_new("hello world");
 *
 *    assert(3 == str_count(&str, 'l'));
 *    assert(2 == str_count(&str, "o"));
 * */
size_t __str_count(str_t* str, c_str c_string, size_t len);
#define str_count(s, c_s, arg...) __str_count(s, __str_start(c_s), va_0th(__str_size(c_s), arg))
