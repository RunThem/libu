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
  i64_t itsize;
  i64_t len;
  i64_t cap;
  any_t items;
} vec_t, *vec_ref_t;

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#undef at
#define at(idx) (self->items + self->itsize * (idx))

/***************************************************************************************************
 * Let
 **************************************************************************************************/
thread_local u_cmp_fn th_cmp_fn     = {};
thread_local u_order_e th_cmp_order = {};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri inline int vec_cmp_fn(cany_t a, cany_t b) {
  int res = th_cmp_fn(a, b);

  return th_cmp_order == U_ORDER_ASCEND ? res : -res;
}

pub any_t vec_new(i64_t itsize) {
  vec_ref_t self = nullptr;

  assert(itsize);

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

pub bool vec_is_exist(any_t _self, i64_t idx) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, false);

  return self->len > idx;
}

pub i64_t vec_len(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, -1);

  return self->len;
}

pub i64_t vec_cap(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, 0);

  return self->cap;
}

pub int vec_resize(any_t _self, i64_t cap) {
  vec_ref_t self = (vec_ref_t)_self;
  any_t items    = nullptr;

  u_chk_if(self, -1);
  u_chk_if(cap < self->cap, -1);

  items = u_realloc(self->items, self->itsize * cap);
  u_end_if(items);

  self->items = items;
  self->cap   = cap;

  return 0;

end:
  return -1;
}

pub any_t vec_at(any_t _self, i64_t idx) {
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
pub void vec_pop(any_t _self, i64_t idx, any_t item) {
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
pub void vec_put(any_t _self, i64_t idx, any_t item) {
  vec_ref_t self = (vec_ref_t)_self;
  i64_t cap      = 0;
  int ret        = 0;

  u_chk_if(self);
  u_chk_if(idx > self->len || idx < -(self->len + 1));

  idx += (idx < 0) ? self->len + 1 : 0;
  if (self->len == self->cap) {
    cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;
    ret = vec_resize(_self, cap);
    u_end_if(ret != 0);
  }

  if (idx != self->len) {
    memmove(at(idx + 1), at(idx), (self->len - idx) * self->itsize);
  }

  memcpy(at(idx), item, self->itsize);

  self->len++;

  return;

end:
}

pub void vec_sort(any_t _self, u_cmp_fn cmp_fn, u_order_e order) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);
  u_chk_if(cmp_fn);
  u_chk_if(self->len < 2);

  th_cmp_fn    = cmp_fn;
  th_cmp_order = order;
  qsort(self->items, self->len, self->itsize, vec_cmp_fn);
}

pub bool vec_is_sort(any_t _self, u_cmp_fn cmp_fn, u_order_e order) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, false);
  u_chk_if(cmp_fn, false);

  for (int i = 1; i < self->len; i++) {
    if (order == cmp_fn(at(i - 1), at(i))) {
      return false;
    }
  }

  return true;
}

pub i64_t vec_pole(any_t _self, u_cmp_fn cmp_fn, u_order_e order) {
  vec_ref_t self = (vec_ref_t)_self;
  int result     = 0;

  u_chk_if(self, -1);
  u_chk_if(cmp_fn, -1);

  result = 0;
  for (int i = 1; i < self->len; i++) {
    if (cmp_fn(at(result), at(i)) == order) {
      result = i;
    }
  }

  return result;
}

pub bool vec_for(any_t _self, bool* init, i64_t* idx, any_t item, u_order_e order) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, false);
  u_chk_if(self->len == 0, false);

  if (!*init) {
    *init = true;
    *idx  = order == U_ORDER_ASCEND ? 0 : self->len - 1;
  } else { /* range */
    *idx += order;
  }

  u_end_if(*idx == -1 || *idx == self->len);

  memcpy(item, at(*idx), self->itsize);

  return true;

end:
  return false;
}
