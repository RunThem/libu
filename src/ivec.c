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

#include <u/u.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
  u8_t flags[4];
  size_t itsize;
  int len;
  int cap;
  any_t items;
} vec_t, *vec_ref_t;

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#undef at
#define at(idx) (self->items + self->itsize * (idx))

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri ret_t vec_resize(vec_ref_t self) {
  int cap     = 0;
  any_t items = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

  items = u_realloc(self->items, self->itsize * cap);
  u_end_if(items);

  self->items = items;
  self->cap   = cap;

  return 0;

end:
  return -1;
}

pub any_t vec_new(size_t itsize) {
  vec_ref_t self = nullptr;

  u_chk_if(itsize == 0, nullptr);

  self = u_talloc(vec_t);
  u_end_if(self);

  self->items = u_calloc(16, itsize);
  u_end_if(self->items);

  self->itsize = itsize;
  self->cap    = 16;

  return self;

end:
  u_free_if(self);

  return nullptr;
}

pub void vec_clear(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);

  self->len = 0;
}

pub void vec_cleanup(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);

  u_free_if(self->items);
  u_free_if(self);
}

pub bool vec_exist(any_t _self, int idx) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, false);

  return self->len > idx;
}

pub size_t vec_len(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, 0);

  return self->len;
}

pub size_t vec_cap(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, 0);

  return self->cap;
}

pub any_t vec_at(any_t _self, int idx) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(idx >= self->len && idx < -self->len, nullptr);

  idx += (idx < 0) ? self->len : 0;

  return at(idx);
}

/*
 * len = 7
 *
 * idx = (idx < 0) ? idx + len : idx
 * {  0,  1,  2,  3,  4,  5,  6 }
 * { -7, -6, -5, -4, -3, -2  -1 }
 * */
pub void vec_pop(any_t _self, int idx, any_t item) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);
  u_chk_if(self->len == 0);
  u_chk_if(idx > self->len && idx < -self->len);

  idx += (idx < 0) ? self->len : 0;
  memcpy(item, at(idx), self->itsize);
  if (idx != self->len - 1) {
    memmove(at(idx), at(idx + 1), (self->len - idx - 1) * self->itsize);
  }

  self->len--;

  bzero(at(self->len), self->itsize);
}

/*
 * len = 7
 *
 * idx = (0 < idx) ? idx + 1 + len : idx
 *
 * {  0,  1,  2,  3,  4,  5,  6 }  7
 * { -8, -7, -6, -5, -4, -3, -2 } -1
 * */
pub void vec_put(any_t _self, int idx, any_t item) {
  vec_ref_t self = (vec_ref_t)_self;
  ret_t result   = 0;

  u_chk_if(self);
  u_chk_if(idx > (ssize_t)self->len || idx < -((ssize_t)self->len + 1));

  idx += (idx < 0) ? self->len + 1 : 0;
  if (self->len == self->cap) {
    result = vec_resize(self);
    u_end_if(result != 0);
  }

  if (idx != self->len) {
    memmove(at(idx + 1), at(idx), (self->len - idx) * self->itsize);
  }

  memcpy(at(idx), item, self->itsize);

  self->len++;

  return;

end:
}

pub void vec_sort(any_t _self, u_cmp_fn cmp_fn) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);
  u_chk_if(cmp_fn);
  u_chk_if(self->len < 2);

  qsort(self->items, self->len, self->itsize, cmp_fn);
}

pub bool vec_for_init(any_t _self, bool flag) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, false);

  if (self->flags[0] == 0) {
    self->flags[0] = 1;
  } else if (self->flags[0] == 2) {
    self->flags[0] = 0;
  }

  self->flags[1] = flag;
  self->flags[2] = true;
  self->flags[3] = false;

  return self->flags[0];
}

pub void vec_for_end(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);

  self->flags[0] = 2;
}

pub bool vec_for(any_t _self, int* idx, any_t item) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, false);
  u_chk_if(self->len == 0, false);
  u_chk_if(self->flags[3], false);

  /* 初始化 */
  if (self->flags[2]) {
    *idx           = self->flags[1] ? 0 : self->len - 1;
    self->flags[2] = !self->flags[2];
  } else { /* 迭代 */
    *idx += self->flags[1] ? 1 : -1;

    /* 判断是否继续迭代 */
    if (*idx == self->len - 1 || *idx == 0) {
      self->flags[3] = true;
    }
  }

  memcpy(item, at(*idx), self->itsize);

  return true;
}
