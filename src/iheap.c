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
#define left(i)   (i * 2 + 1)
#define right(i)  (i * 2 + 2)
#define parent(i) ((i - 1) / 2)

#define at(idx) (self->root + self->itsize * (idx))

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
  bool attr; /* { true: min, false: max } */
  u8_t flags[4];
  size_t itsize;
  size_t len;
  size_t cap;

  u_cmp_fn cmp_fn;

  any_t root;
} heap_t, *heap_ref_t;

/***************************************************************************************************
 * Function
 **************************************************************************************************/
static ret_t heap_resize(heap_ref_t self) {
  size_t cap = 0;
  any_t root = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

  root = u_realloc(self->root, self->itsize * cap);
  u_check_expr_null_goto(root);

  self->root = root;
  self->cap  = cap;

  return 0;

end:
  return -1;
}

any_t heap_new(size_t itsize, bool attr, u_cmp_fn fn) {
  heap_ref_t self = nullptr;

  u_check_args_ret(itsize == 0, nullptr);

  self = u_talloc(heap_t);
  u_check_expr_null_goto(self);

  self->root = u_calloc(32, itsize);
  u_check_expr_null_goto(self->root);

  self->itsize = itsize;
  self->cmp_fn = fn;
  self->len    = 0;
  self->cap    = 32;
  self->attr   = attr;

  return self;

end:
  u_free_if(self);

  return nullptr;
}

void heap_clear(any_t _self) {
  heap_ref_t self = (heap_ref_t)_self;

  u_check_args_null_ret(self);

  self->len = 0;
}

void heap_cleanup(any_t _self) {
  heap_ref_t self = (heap_ref_t)_self;

  u_check_args_null_ret(self);

  u_free_if(self->root);
  u_free_if(self);
}

size_t heap_len(any_t _self) {
  heap_ref_t self = (heap_ref_t)_self;

  u_check_args_null_ret(self, 0);

  return self->len;
}

void heap_at(any_t _self, any_t item) {
  heap_ref_t self = (heap_ref_t)_self;

  u_check_args_null_ret(self);

  u_check_args_ret(self->len == 0);

  memcpy(item, self->root, self->itsize);
}

void heap_pop(any_t _self, any_t item) {
  heap_ref_t self = (heap_ref_t)_self;
  size_t idx      = 0;
  size_t lidx     = 0;
  size_t ridx     = 0;
  size_t pidx     = 0;
  int flag        = 0;

  u_check_args_null_ret(self);

  u_check_args_ret(self->len == 0);

  memcpy(item, self->root, self->itsize);

  flag = self->attr ? 1 : -1;
  while (true) {
    lidx = left(pidx);
    ridx = right(pidx);

    /* no child node */
    if (lidx >= self->len) {
      break;
    }

    idx = lidx;

    /* if there are two child nodes, select the node that meets the rules(flag) */
    if (ridx < self->len) {
      if (self->cmp_fn(at(lidx), at(ridx)) == flag) {
        idx = ridx;
      }
    }

    /* compare the child node with the current node to see if they meet the rules(flag) */
    if (self->cmp_fn(at(idx), at(self->len - 1)) == flag) {
      break;
    }

    /* exchange child node with current node  */
    memcpy(at(pidx), at(idx), self->itsize);

    /* iteration */
    pidx = idx;
  }

  memcpy(at(idx), at(self->len - 1), self->itsize);

  self->len--;
}

void heap_put(any_t _self, any_t item) {
  heap_ref_t self = (heap_ref_t)_self;
  ret_t code      = 0;
  size_t idx      = 0;
  size_t pidx     = 0;
  int flag        = 0;

  u_check_args_null_ret(self);

  if (self->len == self->cap) {
    code = heap_resize(self);
    u_check_expr_goto(code != 0);
  }

  flag = self->attr ? 1 : -1;
  for (idx = self->len; idx > 0; idx = pidx) {
    pidx = parent(idx);

    if (self->cmp_fn(item, at(pidx)) == flag) {
      break;
    }

    memcpy(at(idx), at(pidx), self->itsize);
  }

  memcpy(at(idx), item, self->itsize);

  self->len++;

  return;

end:
}
