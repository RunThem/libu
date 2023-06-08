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

c_str __str_at(str_t* str, size_t idx) {
  u_ret_if(str == nullptr, nullptr);
  u_ret_if(_str == nullptr, nullptr);
  u_ret_if(idx >= _str->len, nullptr);

  return &_str->c_str[idx];
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

int __str_erase(str_t* str, size_t idx, ssize_t len) {
  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(idx < 0 || idx > _str->len, -1);

  if (len == -1) {
    memmove(_str->c_str, &_str->c_str[idx], _str->len - idx);

    _str->c_str[_str->len - idx] = '\0';
    _str->len -= idx;
  } else if (len == -2 || _str->len <= idx + len) {
    _str->c_str[idx] = '\0';
    _str->len        = idx;
  } else {
    memmove(&_str->c_str[idx], &_str->c_str[idx + len], _str->len - idx - len);

    _str->c_str[_str->len - len] = '\0';
    _str->len -= len;
  }

  return 0;
}

int __str_comp(str_t* str, c_str c_string, size_t len) {
  u_ret_if(str == nullptr, false);
  u_ret_if(_str == nullptr, false);
  u_ret_if(c_string == nullptr, false);
  u_ret_if(len != _str->len, false);

  return strncmp(_str->c_str, c_string, len);
}

ssize_t __str_find(str_t* str, c_str c_string, size_t len, size_t idx) {
  c_str p = nullptr;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(c_string == nullptr, -1);
  u_ret_if(len > _str->len, -1);
  u_ret_if(idx >= _str->len, -1);
  u_ret_if(len + idx >= _str->len, -1);

  p = strstr(&_str->c_str[idx], c_string);
  u_goto_if(p == nullptr);

  return p - _str->c_str;

err:
  return -1;
}

bool __str_prefix(str_t* str, c_str c_string, size_t len) {
  return __str_find(str, c_string, len, 0) == 0;
}

bool __str_suffix(str_t* str, c_str c_string, size_t len) {
  return __str_find(str, c_string, len, _str->len - len) == (_str->len - len);
}

bool __str_contain(str_t* str, c_str c_string, size_t len) {
  return __str_find(str, c_string, len, 0) != -1;
}

size_t __str_count(str_t* str, c_str c_string, size_t len) {
  c_str tmp    = nullptr;
  c_str ptr    = nullptr;
  size_t count = 0;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(c_string == nullptr, -1);
  u_ret_if(len > _str->len, -1);

  ptr = _str->c_str;

  while (true) {
    tmp = strstr(ptr, c_string);

    if (tmp == nullptr) {
      break;
    }

    ptr = tmp + 1;
    count++;
  }

  return count;
}

str_t __str_cut(str_t* str, size_t idx, size_t len) {
  str_t sub_str = nullptr;

  u_ret_if(str == nullptr, nullptr);
  u_ret_if(_str == nullptr, nullptr);
  u_ret_if(idx >= _str->len, nullptr);
  u_ret_if(idx + len > _str->len, nullptr);

  if (len == 0) {
    sub_str = __str_new(&_str->c_str[idx], _str->len - idx);
    u_goto_if(sub_str == nullptr);

    __str_erase(str, idx, -2);
  } else {
    sub_str = __str_new(&_str->c_str[idx], len);
    u_goto_if(sub_str == nullptr);

    __str_erase(str, idx, (ssize_t)len);
  }

  return sub_str;

err:
  return nullptr;
}

int __str_repeat(str_t* str, size_t count) {
  int ret = 0;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(count <= 1, -1);

  ret = __str_resize(str, str_cap(_str->len * count));
  u_goto_if(ret != 0);

  for (size_t i = 0; i < count; i++) {
    strncpy(&_str->c_str[i * _str->len], _str->c_str, _str->len);
  }

  _str->len *= count;

  return 0;

err:
  return -1;
}

int __str_replace(str_t* str,
                  c_str src_string,
                  size_t src_len,
                  c_str dst_string,
                  size_t dst_len,
                  size_t count) {
  c_str p       = nullptr;
  size_t idx    = 0;
  size_t _count = 0;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(src_string == nullptr, -1);
  u_ret_if(src_len <= 0, -1);
  u_ret_if(dst_string == nullptr, -1, );
  u_ret_if(dst_len <= 0, -1);
  u_ret_if(count < 0, -1);

  while (count == 0 || _count < count) {
    p = strstr(&_str->c_str[idx], src_string);
    if (p == nullptr) {
      break;
    }

    idx = p - _str->c_str;

    __str_erase(str, idx, (ssize_t)src_len);
    __str_insert(str, idx, dst_string, dst_len);

    _count++;
    idx += dst_len;
  }

  return _count == count;
}
