#include "str.h"
#include "u.h"

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

int __str_resize(str_t* str, size_t len) {
  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(len <= _str->len, -1);

  _str = (str_t)u_realloc(_str, len);
  u_alloc_if(_str);

  _str->cap = len;

  return 0;

err:
  return -1;
}

int __str_push(str_t* str, c_str c_string, size_t size) {
  int ret = 0;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(c_string == nullptr, -1);

  if (size > _str->cap - _str->len) {
    ret = __str_resize(str, str_cap(_str->len) + size);
    u_goto_if(ret != 0);
  }

  strncpy(&_str->c_str[_str->len], c_string, size);
  _str->len += size;

  return 0;

err:
  return -1;
}

int __str_push_f(str_t* str, c_str fmt, ...) {
  char buf[4096] = {0};
  int size       = 0;
  int ret        = 0;
  va_list ap;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(fmt == nullptr, -1);

  va_start(ap, fmt);
  size = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (size > _str->cap - _str->len) {
    ret = __str_resize(str, str_cap(_str->len) + size);
    u_goto_if(ret != 0);
  }

  strncpy(&_str->c_str[_str->len], buf, size);
  _str->len += size;

  return 0;

err:
  return -1;
}

int __str_insert(str_t* str, size_t idx, c_str c_string, size_t size) {
  int ret = 0;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(idx < 0 || idx > _str->len, -1);
  u_ret_if(c_string == nullptr, -1);

  if (size > _str->cap - _str->len) {
    ret = __str_resize(str, str_cap(_str->len) + size);
    u_goto_if(ret != 0);
  }

  if (idx < _str->len) {
    memmove(&_str->c_str[idx + size], &_str->c_str[idx], _str->len - idx);
  }

  strncpy(&_str->c_str[idx], c_string, size);
  _str->len += size;

  return 0;

err:
  return -1;
}

int __str_insert_f(str_t* str, size_t idx, c_str fmt, ...) {
  char buf[4096] = {0};
  int size       = 0;
  int ret        = 0;
  va_list ap;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(idx < 0 || idx > _str->len, -1);
  u_ret_if(fmt == nullptr, -1);

  va_start(ap, fmt);
  size = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (size > _str->cap - _str->len) {
    ret = __str_resize(str, str_cap(_str->len) + size);
    u_goto_if(ret != 0);
  }

  memmove(&_str->c_str[idx + size], &_str->c_str[idx], _str->len - idx);
  strncpy(&_str->c_str[idx], buf, size);
  _str->len += size;

  return 0;

err:
  return -1;
}
