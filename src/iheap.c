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
#undef left
#define left(i) (i * 2 + 1)

#undef right
#define right(i) (i * 2 + 2)

#undef parent
#define parent(i) ((i - 1) / 2)

#undef at
#define at(idx) (self->items + self->itsize * (idx))

/***************************************************************************************************
 * Type
 **************************************************************************************************/
u_struct_def(heap, [[gnu::packed]]) {
  typeof_unqual(*(u_vec_t(heap_mut_t)){}) m;

  u_order_e order; /* { true: min, false: max } */
  i32_t itsize;

  u_cmp_fn cmp_fn;

  any_t items;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri int heap_resize(heap_mut_t self) {
  i32_t cap  = 0;
  any_t root = nullptr;

  cap = (self->m.cap < 1024) ? self->m.cap * 2 : self->m.cap + 512;

  root = u_realloc(self->items, self->itsize * cap);
  u_end_if(root);

  self->items = root;
  self->m.cap = cap;

  return 0;

end:
  return -1;
}

pub any_t $heap_new(i32_t itsize, u_order_e order, u_cmp_fn fn) {
  heap_mut_t self = nullptr;

  u_chk_if(itsize == 0, nullptr);

  self = u_talloc(heap_t);
  u_end_if(self);

  self->items = u_calloc(32, itsize);
  u_end_if(self->items);

  self->itsize = itsize;
  self->cmp_fn = fn;
  self->order  = order;
  self->m.cap  = 32;
  self->m.len  = 0;
  self->m.ref  = any(self);

  return self;

end:
  u_free_if(self);

  return nullptr;
}

pub void $heap_clear(any_t _self) {
  heap_mut_t self = (heap_mut_t)_self;

  u_chk_if(self);

  self->m.len = 0;
}

pub void $heap_cleanup(any_t _self) {
  heap_mut_t self = (heap_mut_t)_self;

  u_chk_if(self);

  u_free_if(self->items);
  u_free_if(self);
}

pub any_t $heap_at(any_t _self) {
  heap_mut_t self = (heap_mut_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(self->m.len == 0, nullptr);

  return self->items;
}

pub void $heap_pop(any_t _self, any_t item) {
  heap_mut_t self = (heap_mut_t)_self;
  int len         = self->m.len;
  int idx         = 0;
  int lidx        = 0;
  int ridx        = 0;
  int pidx        = 0;

  u_chk_if(self);
  u_chk_if(self->m.len == 0);

  memcpy(item, self->items, self->itsize);

  while (true) {
    lidx = left(pidx);
    ridx = right(pidx);

    /* no child node */
    u_brk_if(lidx >= len);

    idx = lidx;

    /* if there are two child nodes, select the node that meets the rules(order) */
    if (ridx < len) {
      if (self->cmp_fn(at(lidx), at(ridx)) == self->order) {
        idx = ridx;
      }
    }

    /* compare the child node with the current node to see if they meet the rules(order) */
    u_brk_if(self->cmp_fn(at(idx), at(len - 1)) == self->order);

    /* exchange child node with current node  */
    memcpy(at(pidx), at(idx), self->itsize);

    /* iteration */
    pidx = idx;
  }

  memcpy(at(idx), at(len - 1), self->itsize);

  self->m.len--;
}

pub void $heap_put(any_t _self, any_t item) {
  heap_mut_t self = (heap_mut_t)_self;
  int result      = 0;
  int idx         = 0;
  int pidx        = 0;

  u_chk_if(self);

  if (self->m.len == self->m.cap) {
    result = heap_resize(self);
    u_end_if(result != 0);
  }

  for (idx = self->m.len; idx > 0; idx = pidx) {
    pidx = parent(idx);

    u_brk_if(self->cmp_fn(item, at(pidx)) == self->order);

    memcpy(at(idx), at(pidx), self->itsize);
  }

  memcpy(at(idx), item, self->itsize);

  self->m.len++;

  return;

end:
}
