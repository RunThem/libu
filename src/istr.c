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
 * Macro
 **************************************************************************************************/
#define U_STR_BUFF_SIZE 47

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct [[gnu::packed]] {
  /* SSO */
  byte_t buff[U_STR_BUFF_SIZE + 1];
  int cap;
  int len;
  byte_t* ptr; /* { ... } \0 */
} str_t, *str_ref_t;

/***************************************************************************************************
 * Let
 **************************************************************************************************/
thread_local byte_t chars[2] = {0, '\0'};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri inline ret_t str_resize(str_ref_t self, int len) {
  byte_t* buff = nullptr;

  self->cap = u_align_of_2pow(len + self->len);

  if (self->ptr == self->buff) {
    buff = u_zalloc(self->cap + 1);
    memcpy(buff, self->ptr, self->len);
  } else {
    buff = u_realloc(self->ptr, self->cap + 1);
    u_end_if(buff);
  }

  self->ptr            = buff;
  self->ptr[self->len] = '\0';

  return 0;

end:
  return -1;
}

pri void inline str_parse(byte_t** ptr, int* len, any_t str, int type) {
  u_die_if(type == 0);

  if (type == 1) {
    chars[0] = (char)(uintptr_t)str;
    *ptr     = chars;
    *len     = 1;
  } else if (type == 2) {
    *ptr = str;
    *len = (int)strlen(str);
  } else if (type == 3) {
    *ptr = (byte_t*)((u_str_t)str)->ptr;
    *len = ((u_str_t)str)->len;
  }
}

pub u_str_t str_new(any_t str, int type, int _len) {
  str_ref_t self = nullptr;
  byte_t* ptr    = nullptr;
  int len        = 0;

  self = u_talloc(str_t);
  u_end_if(self);

  self->ptr = self->buff;
  self->cap = U_STR_BUFF_SIZE;

  if (str != nullptr) {
    str_parse(&ptr, &len, str, type);
    if (_len != 0) {
      len = _len;
    }

    if (len > U_STR_BUFF_SIZE) {
      self->cap = u_align_of_2pow(len);
      self->ptr = u_zalloc(self->cap + 1);
    }

    memcpy(&self->ptr[0], ptr, len);
  }

  self->len            = len;
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

pub void str_gc_cleanup(u_str_t* _self) {
  str_ref_t self = nullptr;

  u_chk_if(_self);
  u_chk_if(*_self);

  self = u_container_of(*_self, str_t, len);
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

pub void str_2lower(u_str_t _self) {
  str_ref_t self = nullptr;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  for (int i = 0; i < self->len; i++) {
    self->ptr[i] = (byte_t)tolower(self->ptr[i]);
  }
}

pub void str_2upper(u_str_t _self) {
  str_ref_t self = nullptr;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  for (int i = 0; i < self->len; i++) {
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

pub void str_trim(u_str_t _self) {
  str_ref_t self = nullptr;
  int l          = 0;
  int r          = 0;

  u_chk_if(_self);

  self = u_container_of(_self, str_t, len);

  for (l = 0; l < self->len && isspace(self->ptr[l]); l++) {
  }

  u_end_if(l == self->len);

  for (r = self->len - 1; r >= 0 && isspace(self->ptr[r]); r--) {
  }

  if (l != 0) {
    memmove(&self->ptr[0], &self->ptr[l], r - l + 1);
  }

  self->len            = r - l + 1;
  self->ptr[self->len] = '\0';

  return;

end:
  self->len            = 0;
  self->ptr[self->len] = '\0';
}

pub void str_ins(u_str_t _self, int idx, any_t str, int type) {
  str_ref_t self = nullptr;
  byte_t* ptr    = nullptr;
  int len        = 0;
  int ret        = 0;

  u_chk_if(_self);
  u_chk_if(idx > _self->len);
  u_chk_if(str);

  str_parse(&ptr, &len, str, type);

  self = u_container_of(_self, str_t, len);

  /* resize */
  if (len > self->cap - self->len) {
    ret = str_resize(self, len);
    u_end_if(ret != 0);
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

pub int str_cmp(u_str_t _self, any_t str, int type) {
  str_ref_t self = nullptr;
  byte_t* ptr    = nullptr;
  int len        = 0;

  u_chk_if(_self, -2);
  u_chk_if(str, -2);

  str_parse(&ptr, &len, str, type);

  self = u_container_of(_self, str_t, len);

  if (self->len == len) {
    return strncmp(self->ptr, ptr, len);
  }

  return self->len > len ? 1 : -1;
}

pub bool str_is_prefix(u_str_t _self, any_t str, int type) {
  str_ref_t self = nullptr;
  byte_t* ptr    = nullptr;
  int len        = 0;

  u_chk_if(_self, 0);
  u_chk_if(str, 0);

  str_parse(&ptr, &len, str, type);
  u_end_if(len == 0 && len > self->len);

  self = u_container_of(_self, str_t, len);

  return memcmp(&self->ptr[0], ptr, len) == 0;

end:
  return false;
}

pub bool str_is_suffix(u_str_t _self, any_t str, int type) {
  str_ref_t self = nullptr;
  byte_t* ptr    = nullptr;
  int len        = 0;

  u_chk_if(_self, 0);
  u_chk_if(str, 0);

  str_parse(&ptr, &len, str, type);
  u_end_if(len == 0 && len > self->len);

  self = u_container_of(_self, str_t, len);

  return memcmp(&self->ptr[self->len - len], ptr, len) == 0;

end:
  return false;
}

pub int str_find(u_str_t _self, any_t str, int type) {
  str_ref_t self = nullptr;
  byte_t* idx    = nullptr;
  byte_t* ptr    = nullptr;
  int len        = 0;

  u_chk_if(_self, -1);
  u_chk_if(str, -1);

  str_parse(&ptr, &len, str, type);
  u_end_if(len == 0 && len > self->len);

  self = u_container_of(_self, str_t, len);

  idx = strstr(self->ptr, ptr);
  u_end_if(idx);

  return (int)(idx - self->ptr);

end:
  return -1;
}

pub u_str_t str_sub(u_str_t _self, int s, int e) {
  str_ref_t new = nullptr;
  int len       = 0;

  /*
   * h e l l o
   * 0 1 2 3 4
   *
   * <1, 3> = el
   * */
  u_chk_if(_self, nullptr);
  u_chk_if(e > _self->len, nullptr);
  u_chk_if(e < s, nullptr);

  new = u_talloc(str_t);
  u_end_if(new);

  new->len = e - s;

  if (new->len > U_STR_BUFF_SIZE) {
    new->cap = u_align_of_2pow(new->len);
    new->ptr = u_zalloc(new->cap + 1);
  } else {
    new->cap = U_STR_BUFF_SIZE;
    new->ptr = new->buff;
  }

  memcpy(new->ptr, &_self->ptr[s], new->len);

  return (u_str_t)(&new->len);

end:
  u_free_if(new);

  return nullptr;
}
