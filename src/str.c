#include "str.h"

#define _str (*str)

/* clang-format off */
size_t __str_char_size(char c)     { return sizeof(char); }
size_t __str_c_str_size(c_str s)   { return strlen(s); }
size_t __str_string_size(str_t s)  { return s->len; }
c_str  __str_char_start(void* c)   { return (c_str)c; }
c_str  __str_c_str_start(void* s)  { return (c_str)(*(c_str*)s); }
c_str  __str_string_start(void* s) { return (c_str)((*(str_t*)s)->c_str); }
/* clang-format on */

static size_t str_cap(size_t len) {
  if (len < 8) {
    return 8;
  } else if (len < 1024) {
    return len * 2;
  }

  return len + 1024;
}

str_t __str_new(c_str c_string, size_t len) {
  size_t cap = 0;
  str_t str  = nullptr;

  u_ret_if(c_string == nullptr, nullptr);
  u_ret_if(len < 0, nullptr);

  cap = str_cap(len);

  str = u_talloc(sizeof(struct __str) + 1 + cap, str_t);
  u_alloc_if(str);

  str->len = len;
  str->cap = cap;
  if (c_string[0] != '\0') {
    strncpy(str->c_str, c_string, len);
  }

  return str;

err:
  return nullptr;
}

str_t __str_new_f(c_str fmt, ...) {
  char buf[4096] = {0};
  int len        = 0;
  size_t cap     = 0;
  str_t str      = nullptr;
  va_list ap;

  va_start(ap, fmt);
  len = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  cap = str_cap(len);

  str = u_talloc(sizeof(struct __str) + 1 + cap, str_t);
  u_alloc_if(str);

  str->len = len;
  str->cap = cap;
  if (buf[0] != '\0') {
    strncpy(str->c_str, buf, len);
  }

  return str;

err:
  return nullptr;
}

void __str_clear(str_t* str) {
  u_ret_no_if(str == nullptr);
  u_ret_no_if(_str == nullptr);

  _str->len      = 0;
  _str->c_str[0] = '\0';
}

void __str_cleanup(str_t* str) {
  u_ret_no_if(str == nullptr);
  u_ret_no_if(_str == nullptr);

  u_free(_str);
  _str = nullptr;
}

bool __str_empty(str_t* str) {
  u_ret_if(str == nullptr, true);
  u_ret_if(_str == nullptr, true);

  return (_str)->len == 0;
}

