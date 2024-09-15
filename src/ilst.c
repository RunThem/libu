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
  i64_t offset;
  i64_t len;

  u_node_ref_t head;
  u_node_ref_t tail;
} lst_t, *lst_ref_t;

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pub any_t lst_new(i64_t offset) {
  lst_ref_t self = nullptr;

  self = u_talloc(lst_t);
  u_end_if(self);

  self->head   = nullptr;
  self->tail   = nullptr;
  self->len    = 0;
  self->offset = offset;

  return self;

end:
  u_free_if(self);

  return nullptr;
}

pub bool lst_clear(any_t _self, any_t ptr) {
  lst_ref_t self    = (lst_ref_t)_self;
  u_node_ref_t node = nullptr;

  u_chk_if(self, false);
  u_chk_if(ptr, false);

  node = ptr + self->offset;

  self->head = self->head->next;
  if (!self->head) {
    self->len  = 0;
    self->tail = nullptr;
  }

  node->prev = nullptr;
  node->next = nullptr;
  node->ptr  = nullptr;

  return true;
}

pub void lst_cleanup(any_t _self) {
  lst_ref_t self = (lst_ref_t)_self;

  u_chk_if(self);

  u_free_if(self);
}

pub i64_t lst_len(any_t _self) {
  lst_ref_t self = (lst_ref_t)_self;

  u_chk_if(self, -1);

  return self->len;
}

pub bool lst_is_exist(any_t _self, any_t ptr) {
  lst_ref_t self    = (lst_ref_t)_self;
  u_node_ref_t node = nullptr;

  u_chk_if(self, false);
  u_chk_if(ptr, false);
  u_chk_if(self->len == 0, false);

  node = ptr + self->offset;

  return node->ptr == self;
}

pub any_t lst_head(any_t _self) {
  lst_ref_t self = (lst_ref_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(self->len == 0, nullptr);

  return any(self->head) - self->offset;
}

pub any_t lst_tail(any_t _self) {
  lst_ref_t self = (lst_ref_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(self->len == 0, nullptr);

  return any(self->tail) - self->offset;
}

pub any_t lst_prev(any_t _self, any_t ptr) {
  lst_ref_t self    = (lst_ref_t)_self;
  u_node_ref_t node = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(ptr, nullptr);

  node = ptr + self->offset;
  u_end_if(node->ptr != self);
  u_end_if(node->prev);

  return any(node->prev) - self->offset;

end:
  return nullptr;
}

pub any_t lst_next(any_t _self, any_t ptr) {
  lst_ref_t self    = (lst_ref_t)_self;
  u_node_ref_t node = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(ptr, nullptr);

  node = ptr + self->offset;
  u_end_if(node->ptr != self);
  u_end_if(node->next);

  return any(node->next) - self->offset;

end:
  return nullptr;
}

pub void lst_pop(any_t _self, any_t ptr) {
  lst_ref_t self    = (lst_ref_t)_self;
  u_node_ref_t node = nullptr;

  u_chk_if(self);
  u_chk_if(ptr);

  node = ptr + self->offset;
  u_end_if(node->ptr != self);

  if (node->prev) {
    node->prev->next = node->next;
  } else {
    self->head = node->next;
  }

  if (node->next) {
    node->next->prev = node->prev;
  } else {
    self->tail = node->prev;
  }

  self->len--;

  node->prev = nullptr;
  node->next = nullptr;
  node->ptr  = nullptr;

  return;

end:
}

pub void lst_put(any_t _self, any_t idx_ptr, any_t ptr) {
  lst_ref_t self    = (lst_ref_t)_self;
  u_node_ref_t node = nullptr;
  u_node_ref_t prev = nullptr;
  u_node_ref_t next = nullptr;
  u_node_ref_t _idx = nullptr;

  u_chk_if(self);
  u_chk_if(ptr);

  node       = ptr + self->offset;
  node->prev = nullptr;
  node->next = nullptr;
  node->ptr  = self;

  if (!idx_ptr) {
    node->next = self->head;
    self->head = node;
  } else {
    _idx = idx_ptr + self->offset;
    u_end_if(_idx->ptr != self);

    node->next = _idx->next;
    node->prev = _idx;
    _idx->next = node;
  }

  if (node->next == nullptr) {
    self->tail = node;
  } else {
    node->next->prev = node;
  }

  self->len++;

  return;

end:
}

pub bool lst_for(any_t _self, any_t* it, u_order_e order, any_t init) {
  lst_ref_t self    = (lst_ref_t)_self;
  u_node_ref_t iter = nullptr;

  u_chk_if(self, false);
  u_chk_if(self->len == 0, false);

  /* init */
  if (init) {
    iter = order == U_ORDER_ASCEND ? self->head : self->tail;
  } else { /* range */
    iter = any(*it) + self->offset;
    iter = order == U_ORDER_ASCEND ? iter->next : iter->prev;
  }

  u_end_if(iter);

  *it = any(iter) - self->offset;

  return true;

end:
  return false;
}
