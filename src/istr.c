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

#include <u/istr.h>
#include <u/utils/alloc.h>
#include <u/utils/debug.h>
#include <u/utils/misc.h>
#include <u/utils/va.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct str_t str_t;
struct str_t {
  size_t len;
  size_t cap;
  char buf[];
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
static str_t* str_resize(str_t* self) {
  size_t cap = 0;
  str_t* new = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

  new = u_realloc(self, sizeof(str_t) + cap);
  u_nil_if(new);

  new->cap = cap;

  return new;

err:
  return self;
}

u_str_t str_new_char(char ch) {
  str_t* self = nullptr;

  self = u_zalloc(sizeof(str_t) + 16);
  u_nil_if(self);

  self->buf[0] = ch;
  self->len    = 1;
  self->cap    = 16;

  self->buf[self->len] = '\0';

  return (u_str_t)self->buf;

err:
  u_free_if(self);

  return nullptr;
}

u_str_t str_new_cstr(u_cstr_t cstr) {
  str_t* self = nullptr;
  size_t len  = 0;

  if (cstr != nullptr) {
    len = strlen(cstr);
  }

  self = u_zalloc(sizeof(str_t) + (len == 0 ? 16 : len * 2));
  u_nil_if(self);

  if (len != 0) {
    strncpy(self->buf, cstr, len);
  }

  self->len      = len;
  self->cap      = len == 0 ? 16 : len * 2;
  self->buf[len] = '\0';

  return (u_str_t)self->buf;

err:
  u_free_if(self);

  return nullptr;
}

u_str_t str_new_str(u_str_t _str) {
  str_t* self = nullptr;
  str_t* str  = container_of(_str, str_t, buf);
  size_t len  = 0;

  if (_str != nullptr) {
    len = str->len;
  }

  self = u_zalloc(sizeof(str_t) + (len == 0 ? 16 : len * 2));
  u_nil_if(self);

  if (len != 0) {
    strncpy(self->buf, str->buf, len);
  }

  self->len      = len;
  self->cap      = len == 0 ? 16 : len * 2;
  self->buf[len] = '\0';

  return (u_str_t)self->buf;

err:
  u_free_if(self);

  return nullptr;
}

size_t str_len(u_str_t* _self) {
  str_t* self = nullptr;

  u_chk_if(_self == nullptr, 0);
  u_chk_if(*_self == nullptr, 0);

  self = container_of(*_self, str_t, buf);

  return self->len;
}

void str_put_char(u_str_t* _self, char ch) {
  str_t* self = nullptr;

  u_nchk_if(_self == nullptr);
  u_nchk_if(*_self == nullptr);

  self = container_of(*_self, str_t, buf);

  if (self->len + sizeof(char) > self->cap) {
    self = str_resize(self);
    u_err_if(self == nullptr);

    *_self = (u_str_t)self->buf;
  }

  self->buf[self->len]   = ch;
  self->buf[++self->len] = '\0';

err:
}

void str_put_cstr(u_str_t* _self, u_cstr_t cstr) {
  str_t* self = nullptr;
  size_t len  = strlen(cstr);

  u_nchk_if(_self == nullptr);
  u_nchk_if(*_self == nullptr);
  u_nchk_if(cstr == nullptr);

  self = container_of(*_self, str_t, buf);

  if (self->len + len > self->cap) {
    self = str_resize(self);
    u_err_if(self == nullptr);

    *_self = (u_str_t)self->buf;
  }

  strncpy(self->buf + self->len, cstr, len);

  self->len += len;
  self->buf[self->len] = '\0';

err:
}

void str_put_str(u_str_t* _self, u_str_t _str) {
  str_t* self = nullptr;
  str_t* str  = nullptr;

  u_nchk_if(_self == nullptr);
  u_nchk_if(*_self == nullptr);
  u_nchk_if(_str == nullptr);

  self = container_of(*_self, str_t, buf);
  str  = container_of(_str, str_t, buf);

  if (self->len + str->len > self->cap) {
    self = str_resize(self);
    u_err_if(self == nullptr);

    *_self = (u_str_t)self->buf;
  }

  strncpy(self->buf + self->len, str->buf, str->len);

  self->len += str->len;
  self->buf[self->len] = '\0';

err:
}

void str_ins_char(u_str_t* _self, size_t idx, char ch) {
  str_t* self = nullptr;

  u_nchk_if(_self == nullptr);
  u_nchk_if(*_self == nullptr);

  self = container_of(*_self, str_t, buf);

  if (self->len + sizeof(char) > self->cap) {
    self = str_resize(self);
    u_err_if(self == nullptr);

    *_self = (u_str_t)self->buf;
  }

  if (idx != self->len) {
    memmove(self->buf + idx + sizeof(char), self->buf + idx, self->len - idx);
  }

  self->buf[idx]         = ch;
  self->buf[++self->len] = '\0';

err:
}

void str_ins_cstr(u_str_t* _self, size_t idx, u_cstr_t cstr) {
  str_t* self = nullptr;
  size_t len  = strlen(cstr);

  u_nchk_if(_self == nullptr);
  u_nchk_if(*_self == nullptr);
  u_nchk_if(cstr == nullptr);

  self = container_of(*_self, str_t, buf);

  if (self->len + len > self->cap) {
    self = str_resize(self);
    u_err_if(self == nullptr);

    *_self = (u_str_t)self->buf;
  }

  if (idx != self->len) {
    memmove(self->buf + idx + len, self->buf + idx, self->len - idx);
  }

  strncpy(self->buf + idx, cstr, len);

  self->len += len;
  self->buf[self->len] = '\0';

err:
}

void str_ins_str(u_str_t* _self, size_t idx, u_str_t _str) {
  str_t* self = nullptr;
  str_t* str  = nullptr;

  u_nchk_if(_self == nullptr);
  u_nchk_if(*_self == nullptr);
  u_nchk_if(_str == nullptr);

  self = container_of(*_self, str_t, buf);
  str  = container_of(_str, str_t, buf);

  if (self->len + str->len > self->cap) {
    self = str_resize(self);
    u_err_if(self == nullptr);

    *_self = (u_str_t)self->buf;
  }

  if (idx != self->len) {
    memmove(self->buf + idx + str->len, self->buf + idx, self->len - idx);
  }

  strncpy(self->buf + idx, str->buf, str->len);

  self->len += str->len;
  self->buf[self->len] = '\0';

err:
}
