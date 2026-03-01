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

#include "u/ivec.h"

#include <string.h>

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#undef at
#define at(idx) (self->items + self->itsize * (idx))

/***************************************************************************************************
 * Type
 **************************************************************************************************/
u_struct_def(u_vec, [[gnu::packed]]) {
  any_t ref;
  int len;
  int cap;

  i32_t itsize;
  i32_t idx; /* iter */
  any_t items;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pub any_t __u_vec_new(i32_t itsize, i32_t cap) {
  u_vec_mut_t self = NULL;

  self = u_talloc(u_vec_t);
  u_end_if(self);

  self->items = u_calloc(cap, itsize);
  u_end_if(self->items);

  self->idx    = 0;
  self->itsize = itsize;
  self->cap    = cap;
  self->len    = 0;
  self->ref    = any(self);

  return self;

end:
  u_free_if(self);

  return NULL;
}

pub void __u_vec_clear(any_t _self) {
  u_vec_mut_t self = (u_vec_mut_t)_self;

  u_chk_if(self);

  self->len = 0;
}

pub void __u_vec_cleanup(any_t _self) {
  u_vec_mut_t self = (u_vec_mut_t)_self;

  u_chk_if(self);

  u_free_if(self->items);
  u_free_if(self);
}

pub bool __u_vec_resize(any_t _self, i32_t cap) {
  u_vec_mut_t self = (u_vec_mut_t)_self;
  any_t items      = NULL;

  u_chk_if(self, -1);

  items = u_realloc(self->items, self->itsize * cap);
  u_end_if(items);

  self->items = items;
  self->cap   = cap;

  return 0;

end:
  return -1;
}

pub any_t __u_vec_at(any_t _self, i32_t idx) {
  u_vec_mut_t self = (u_vec_mut_t)_self;

  u_chk_if(self, NULL);

  return at(idx);
}

pub void __u_vec_del(any_t _self, i32_t idx) {
  u_vec_mut_t self = (u_vec_mut_t)_self;

  u_chk_if(self);

  if (idx != self->len - 1) {
    memmove(at(idx), at(idx + 1), (self->len - idx - 1) * self->itsize);
  }

  self->len--;
}

pub any_t __u_vec_add(any_t _self, i32_t idx) {
  u_vec_mut_t self = (u_vec_mut_t)_self;
  i32_t cap        = 0;
  int result       = 0;

  u_chk_if(self, NULL);

  if (self->len == self->cap) {
    result = __u_vec_resize(_self, (i32_t)(self->cap * 1.5));
    u_end_if(result != 0);
  }

  if (idx != self->len) {
    memmove(at(idx + 1), at(idx), (self->len - idx) * self->itsize);
  }

  self->len++;

  return at(idx);

end:
  return NULL;
}

pub any_t __u_vec_each(any_t _self, bool init) {
  u_vec_mut_t self = (u_vec_mut_t)_self;

  u_chk_if(self, NULL);
  u_chk_if(self->len == 0, NULL);

  if (!init) {
    return (self->idx == self->len) ? NULL : at(self->idx++);
  }

  self->idx = 0;

  return NULL;
}

pub any_t __u_vec_reach(any_t _self, bool init) {
  u_vec_mut_t self = (u_vec_mut_t)_self;

  u_chk_if(self, NULL);
  u_chk_if(self->len == 0, NULL);

  if (!init) {
    return (self->idx == -1) ? NULL : at(self->idx--);
  }

  self->idx = self->len - 1;

  return NULL;
}
