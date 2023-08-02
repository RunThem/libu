#include "str.h"

#include <stdarg.h>
#include <string.h>

/* clang-format off */
size_t __str_char_size(char c)     { return sizeof(char); }
size_t __str_c_str_size(c_str s)   { return strlen(s); }
size_t __str_string_size(str_t s)  { return s.len; }

c_str  __str_char_start(void* c)   { return (c_str)c; }
c_str  __str_c_str_start(void* s)  { return (c_str)(*(c_str*)s); }
c_str  __str_string_start(void* s) { return (c_str)((*(str_t*)s).c_str); }
/* clang-format on */

static size_t __str_cap(size_t size) {
  if (size < 16) {
    return 16;
  } else if (size < 1024) {
    return size * 2;
  }

  return size + 1024;
}

ret_t __str_init(any_t _self, size_t cap) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);

  self->len = 0;
  self->cap = __str_cap(cap);

  self->c_str = u_calloc(self->cap, sizeof(char));
  u_alloc_if(self->c_str);

  return 0;

err:
  bzero(self, sizeof(str_t));
  return -2;
}

ret_t __str_resize(any_t _self, size_t cap) {
  c_str ptr   = nullptr;
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->cap >= cap, -1);

  ptr = u_realloc(self->c_str, sizeof(char) * cap);
  u_alloc_if(ptr);

  self->c_str = ptr;
  self->cap   = cap;

  return 0;

err:
  return -2;
}

str_t __str_clone(any_t _self) {
  ret_t code  = 0;
  str_t str   = {0};
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, str);

  code = __str_init(&str, self->len);
  u_goto_if(code != 0);

  memcpy(str.c_str, self->c_str, self->len);

  str.len = self->len;

  return str;

err:
  str.len = 0;
  str.cap = 0;

  return str;
}

ret_t __str_cleanup(any_t _self) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);

  u_free_if(self->c_str);

  bzero(self, sizeof(str_t));

  return 0;
}

char __str_at(any_t _self, size_t idx) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, '\0');
  u_ret_if(self->len <= idx, '\0');

  u_ret_if(self->c_str == nullptr, '\0');

  return self->c_str[idx];
}

ret_t __str_push(any_t _self, size_t idx, c_str cstr, size_t len) {
  ret_t code  = 0;
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx > self->len, -1);
  u_ret_if(cstr == nullptr, -1);
  u_ret_if(len == 0, -1);

  if (self->cap - self->len < len) {
    code = __str_resize(self, __str_cap(self->cap + len));
    u_goto_if(code != 0);
  }

  if (idx < self->len) {
    memmove(self->c_str + idx + len, self->c_str + idx, self->len - idx);
  }

  memcpy(self->c_str + idx, cstr, len);
  self->len += len;

  return 0;

err:
  return -2;
}

ret_t __str_pushf(any_t _self, size_t idx, c_str fmt, ...) {
  ret_t code     = 0;
  char buf[4096] = {0};
  size_t len     = 0;
  str_t* self    = as(_self, str_t*);
  va_list ap     = {0};

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx > self->len, -1);
  u_ret_if(fmt == nullptr, -1);

  va_start(ap, fmt);
  len = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (self->cap - self->len < len) {
    code = __str_resize(self, __str_cap(self->cap + len));
    u_goto_if(code != 0);
  }

  if (idx < self->len) {
    memmove(self->c_str + idx + len, self->c_str + idx, self->len - idx);
  }

  memcpy(self->c_str + idx, buf, len);
  self->len += len;

  return 0;

err:
  return -2;
}

ret_t __str_erase(any_t _self, size_t idx, size_t len) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx >= self->len, -1);
  u_ret_if(len + idx > self->len, -1);

  memcpy(self->c_str + idx, self->c_str + idx + len, self->len - idx - len);

  self->c_str[self->len - len] = '\0';
  self->len -= idx + len;

  return 0;
}

bool __str_cmp(any_t _self, c_str cstr, size_t len) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, false);
  u_ret_if(cstr == nullptr, false);
  u_ret_if(len != self->len, false);

  return !strncmp(self->c_str, cstr, len);
}

ssize_t __str_find(any_t _self, c_str cstr, size_t len) {
  any_t pos   = nullptr;
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(cstr == nullptr, -1);
  u_ret_if(len > self->len, -1);

  pos = strstr(self->c_str, cstr);
  u_goto_if(pos == nullptr);

  return pos - any(self->c_str);

err:
  return -1;
}

bool __str_prefix(any_t _self, c_str cstr, size_t len) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, false);
  u_ret_if(cstr == nullptr, false);
  u_ret_if(len == 0, false);

  return !strncmp(self->c_str, cstr, len);
}

bool __str_subfix(any_t _self, c_str cstr, size_t len) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, false);
  u_ret_if(cstr == nullptr, false);
  u_ret_if(len == 0, false);

  return !strncmp(self->c_str + self->len - len, cstr, len);
}

size_t __str_count(any_t _self, c_str cstr, size_t len) {
  size_t count = 0;
  c_str pos    = nullptr;
  c_str tmp    = nullptr;
  str_t* self  = as(_self, str_t*);

  u_ret_if(_self == nullptr, 0);
  u_ret_if(cstr == nullptr, 0);
  u_ret_if(len == 0, 0);
  u_ret_if(len > self->len, 0);

  pos = self->c_str;

  while (true) {
    tmp = strstr(pos, cstr);
    if (tmp == nullptr) {
      break;
    }

    pos = tmp + 1;
    count++;
  }

  return count;
}

#if 0

bool __str_contain(str_t* str, c_str c_string, size_t len) {
  return __str_find(str, c_string, len, 0) != -1;
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

int __str_2lower(str_t* str) {
  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);

  for (size_t i = 0; i < _str->len; i++) {
    _str->c_str[i] = chr(tolower(_str->c_str[i]));
  }

  return 0;
}

int __str_2upper(str_t* str) {
  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);

  for (size_t i = 0; i < _str->len; i++) {
    _str->c_str[i] = chr(toupper(_str->c_str[i]));
  }

  return 0;
}

int __str_trim(str_t* str, c_str c_string, size_t len) {
  c_str ptr = nullptr;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(c_string == nullptr, -1);
  u_ret_if(len <= 0, -1);

  while (true) {
    ptr = strpbrk(_str->c_str, c_string);
    if (ptr == nullptr) {
      break;
    }

    __str_erase(str, ptr - _str->c_str, 1);
  }

  return 0;
}

#endif
