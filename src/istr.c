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

#include <u/u.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct [[gnu::packed]] {
  /* SSO */
  byte_t buff[48];
  int cap;
  int len;
  byte_t* ptr; /* { ... } \0 */
} str_t, *str_ref_t;

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri inline void __str_ins(u_str_t _self, int idx, byte_t* ptr, int len) {
  str_ref_t self = nullptr;
  byte_t* buff   = nullptr;

  self = u_container_of(_self, str_t, len);

  /* resize */
  if (len > self->cap - self->len) {
    self->cap = u_align_of_2pow(len + self->len);

    if (self->ptr == self->buff) {
      self->ptr = u_zalloc(self->cap + 1);
      memcpy(self->ptr, ptr, self->len);
    } else {
      buff = u_realloc(self->ptr, self->cap);
      u_end_if(buff);

      self->ptr = ptr;
    }
  }

  if (idx != self->len) {
    memmove(&self->ptr[idx + len], &self->ptr[idx], self->len - idx);
  }

  memcpy(&self->ptr[idx], ptr, len);

  self->len += len;
  self->ptr[self->len] = '\0';

  return;

end:
}

pri inline int __str_cmp(u_str_t self, byte_t* ptr, int len) {
  u_chk_if(len == 0, 0);

  if (self->len == len) {
    return strncmp(self->ptr, ptr, len);
  }

  return self->len > len ? 1 : -1;
}

pri inline int __str_prefix(u_str_t _self, byte_t* ptr, int len) {
  str_ref_t self = nullptr;

  u_chk_if(len == 0, false);
  u_chk_if(len > _self->len, false);

  self = u_container_of(_self, str_t, len);

  return 0 == memcmp(&self->ptr[0], ptr, len);
}

pri inline int __str_suffix(u_str_t _self, byte_t* ptr, int len) {
  str_ref_t self = nullptr;

  u_chk_if(len == 0, false);
  u_chk_if(len > _self->len, false);

  self = u_container_of(_self, str_t, len);

  return 0 == memcmp(&self->ptr[self->len - len], ptr, len);
}

pub u_str_t str_new() {
  str_ref_t self = nullptr;

  self = u_talloc(str_t);
  u_end_if(self);

  self->ptr = self->buff;
  self->len = 0;
  self->cap = sizeof(self->buff) - 1;

  self->ptr[self->len] = '\0';

  return (u_str_t)(&self->len);

end:
  return nullptr;
}

pub void str_clear(u_str_t _self) {
  str_ref_t self = nullptr;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  self->len            = 0;
  self->ptr[self->len] = '\0';
}

pub void str_cleanup(u_str_t _self) {
  str_ref_t self = nullptr;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  if (self->ptr != self->buff) {
    u_free(self->ptr);
  }

  u_free(self);
}

pub void str_slen(u_str_t _self, int len) {
  str_ref_t self = nullptr;

  u_chk_if(_self);
  u_chk_if(len > _self->len);

  self = u_container_of(_self, str_t, len);

  self->len            = len;
  self->ptr[self->len] = '\0';
}

pub void str_ins_str(u_str_t self, int idx, u_str_t str) {
  u_chk_if(self);
  u_chk_if(idx > self->len);
  u_chk_if(str);
  u_chk_if(str->len == 0);

  __str_ins(self, idx, (byte_t*)str->ptr, str->len);
}

pub void str_ins_cstr(u_str_t self, int idx, u_cstr_t cstr) {
  u_chk_if(self);
  u_chk_if(idx > self->len);
  u_chk_if(cstr);
  u_chk_if(cstr[0] == '\0');

  __str_ins(self, idx, cstr, (int)strlen(cstr));
}

pub void str_ins_char(u_str_t self, int idx, char ch) {
  u_chk_if(self);
  u_chk_if(idx > self->len);

  __str_ins(self, idx, &ch, 1);
}

pub int str_cmp_str(u_str_t self, u_str_t str) {
  u_chk_if(self, 0);
  u_chk_if(str, 0);

  return __str_cmp(self, (byte_t*)str->ptr, str->len);
}

pub int str_cmp_cstr(u_str_t self, u_cstr_t cstr) {
  u_chk_if(self, 0);
  u_chk_if(cstr, 0);

  return __str_cmp(self, cstr, (int)strlen(cstr));
}

pub void str_2lower(u_str_t _self) {
  str_ref_t self = nullptr;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  u_each (i, self->len) {
    self->ptr[i] = (byte_t)tolower(self->ptr[i]);
  }
}

pub void str_2upper(u_str_t _self) {
  str_ref_t self = nullptr;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  u_each (i, self->len) {
    self->ptr[i] = (byte_t)toupper(self->ptr[i]);
  }
}

pub void str_ltrim(u_str_t _self) {
  str_ref_t self = nullptr;
  int i          = 0;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  while (i < self->len && isspace(self->ptr[i])) {
    i++;
  }

  if (i != 0) {
    memmove(&self->ptr[0], &self->ptr[i], self->len - i);
  }

  self->len -= i;
  self->ptr[self->len] = '\0';
}

pub void str_rtrim(u_str_t _self) {
  str_ref_t self = nullptr;
  int i          = 0;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  i = self->len - 1;
  while (i >= 0 && isspace(self->ptr[i])) {
    i--;
  }

  self->len            = i + 1;
  self->ptr[self->len] = '\0';
}

pub bool str_prefix_str(u_str_t self, u_str_t str) {
  u_chk_if(self, false);
  u_chk_if(str, false);

  return __str_prefix(self, (byte_t*)str->ptr, self->len);
}

pub bool str_prefix_cstr(u_str_t self, u_cstr_t cstr) {
  u_chk_if(self, false);
  u_chk_if(cstr, false);

  return __str_prefix(self, cstr, (int)strlen(cstr));
}

pub bool str_prefix_char(u_str_t self, char ch) {
  u_chk_if(self, false);

  return __str_prefix(self, &ch, 1);
}

pub bool str_suffix_str(u_str_t self, u_str_t str) {
  u_chk_if(self, false);
  u_chk_if(str, false);

  return __str_suffix(self, (byte_t*)str->ptr, self->len);
}

pub bool str_suffix_cstr(u_str_t self, u_cstr_t cstr) {
  u_chk_if(self, false);
  u_chk_if(cstr, false);

  return __str_suffix(self, cstr, (int)strlen(cstr));
}

pub bool str_suffix_char(u_str_t self, char ch) {
  u_chk_if(self, false);

  return __str_suffix(self, &ch, 1);
}
