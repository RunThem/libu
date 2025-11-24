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
 * Macro
 **************************************************************************************************/
#undef at
#define at(idx) (self->items + self->itsize * (idx))

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct vec_t vec_t, *vec_ref_t;
struct [[gnu::packed]] vec_t {
  typeof_unqual(*(u_vec_t(vec_ref_t)){}) m;

  i32_t itsize;
  i32_t idx; /* iter */
  any_t items;
};

/***************************************************************************************************
 * Let
 **************************************************************************************************/
thread_local u_cmp_fn th_cmp_fn     = {};
thread_local u_order_e th_cmp_order = {};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
// pri inline int vec_cmp_fn(cany_t a, cany_t b) {
//   int res = th_cmp_fn(a, b);
//
//   return th_cmp_order == U_ORDER_ASCEND ? res : -res;
// }

pub any_t $vec_new(i32_t itsize, i32_t cap) {
  vec_ref_t self = nullptr;

  self = u_talloc(vec_t);
  u_end_if(self);

  self->items = u_calloc(cap, itsize);
  u_end_if(self->items);

  self->idx    = 0;
  self->itsize = itsize;
  self->m.cap  = cap;
  self->m.len  = 0;
  self->m.ref  = any(self);

  return self;

end:
  u_free_if(self);

  return nullptr;
}

pub void $vec_clear(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);

  self->m.len = 0;
}

pub void $vec_cleanup(any_t _self) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);

  u_free_if(self->items);
  u_free_if(self);
}

pub bool $vec_resize(any_t _self, i32_t cap) {
  vec_ref_t self = (vec_ref_t)_self;
  any_t items    = nullptr;

  u_chk_if(self, -1);

  items = u_realloc(self->items, self->itsize * cap);
  u_end_if(items);

  self->items = items;
  self->m.cap = cap;

  return 0;

end:
  return -1;
}

pub any_t $vec_at(any_t _self, i32_t idx) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, nullptr);

  return at(idx);
}

pub void $vec_del(any_t _self, i32_t idx) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self);

  if (idx != self->m.len - 1) {
    memmove(at(idx), at(idx + 1), (self->m.len - idx - 1) * self->itsize);
  }

  self->m.len--;
}

pub any_t $vec_add(any_t _self, i32_t idx) {
  vec_ref_t self = (vec_ref_t)_self;
  i32_t cap      = 0;
  int result     = 0;

  u_chk_if(self, nullptr);

  if (self->m.len == self->m.cap) {
    result = $vec_resize(_self, (i32_t)(self->m.cap * 1.5));
    u_end_if(result != 0);
  }

  if (idx != self->m.len) {
    memmove(at(idx + 1), at(idx), (self->m.len - idx) * self->itsize);
  }

  self->m.len++;

  return at(idx);

end:
  return nullptr;
}

pub any_t $vec_each(any_t _self, bool init) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(self->m.len == 0, nullptr);

  if (!init) {
    return (self->idx == self->m.len) ? nullptr : at(self->idx++);
  }

  self->idx = 0;

  return nullptr;
}

pub any_t $vec_reach(any_t _self, bool init) {
  vec_ref_t self = (vec_ref_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(self->m.len == 0, nullptr);

  if (!init) {
    return (self->idx == -1) ? nullptr : at(self->idx--);
  }

  self->idx = self->m.len - 1;

  return nullptr;
}

// pub void vec_sort(any_t _self, int (^cmp_fn)(void*, void*), u_order_e order) {
//   vec_ref_t self = (vec_ref_t)_self;
//
//   u_chk_if(self);
//   u_chk_if(cmp_fn);
//   u_chk_if(self->len < 2);
//
//   // th_cmp_fn    = cmp_fn;
//   // th_cmp_order = order;
//   qsort(self->items, self->len, self->itsize, cmp_fn);
// }

#if 0

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

#endif
