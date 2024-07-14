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

typedef struct lst_t lst_t;
struct lst_t {
  u8_t flags[4];

  size_t offset;
  size_t len;

  u_node_t* head;
  u_node_t* tail;
  u_node_t* iter;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
any_t lst_new(size_t offset) {
  lst_t* self = nullptr;

  self = u_talloc(lst_t);
  u_nil_if(self);

  self->head   = nullptr;
  self->tail   = nullptr;
  self->len    = 0;
  self->offset = offset;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

bool lst_clear(any_t _self, any_t ptr) {
  lst_t* self    = (lst_t*)_self;
  u_node_t* node = nullptr;

  u_chk_if(self == nullptr, false);
  u_chk_if(ptr == nullptr, false);

  node = ptr + self->offset;

  self->head = self->head->next;
  if (self->head == nullptr) {
    self->len  = 0;
    self->tail = nullptr;
    self->iter = nullptr;
  }

  node->prev = nullptr;
  node->next = nullptr;
  node->ptr  = nullptr;

  return true;
}

void lst_cleanup(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_nchk_if(self == nullptr);

  u_free_if(self);
}

size_t lst_len(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, 0);

  return self->len;
}

bool lst_exist(any_t _self, any_t ptr) {
  lst_t* self    = (lst_t*)_self;
  u_node_t* node = nullptr;

  u_chk_if(self == nullptr, false);
  u_chk_if(ptr == nullptr, false);
  u_chk_if(self->len == 0, false);

  node = ptr + self->offset;

  return node->ptr == self;
}

any_t lst_head(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(self->len == 0, nullptr);

  return any(self->head) - self->offset;
}

any_t lst_tail(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(self->len == 0, nullptr);

  return any(self->tail) - self->offset;
}

any_t lst_prev(any_t _self, any_t ptr) {
  lst_t* self    = (lst_t*)_self;
  u_node_t* node = nullptr;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(ptr == nullptr, nullptr);

  node = ptr + self->offset;
  u_err_if(node->ptr != self);

  return node->prev == nullptr ? nullptr : any(node->prev) - self->offset;

err:
  return nullptr;
}

any_t lst_next(any_t _self, any_t ptr) {
  lst_t* self    = (lst_t*)_self;
  u_node_t* node = nullptr;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(ptr == nullptr, nullptr);

  node = ptr + self->offset;
  u_err_if(node->ptr != self);

  return node->next == nullptr ? nullptr : any(node->next) - self->offset;

err:
  return nullptr;
}

void lst_pop(any_t _self, any_t ptr) {
  lst_t* self    = (lst_t*)_self;
  u_node_t* node = nullptr;

  u_nchk_if(self == nullptr);
  u_nchk_if(ptr == nullptr);

  node = ptr + self->offset;
  u_err_if(node->ptr != self);

  if (node->prev != nullptr) {
    node->prev->next = node->next;
  } else {
    self->head = node->next;
  }

  if (node->next != nullptr) {
    node->next->prev = node->prev;
  } else {
    self->tail = node->prev;
  }

  self->len--;

  node->prev = nullptr;
  node->next = nullptr;
  node->ptr  = nullptr;

  return;

err:
}

void lst_put(any_t _self, any_t idx, any_t ptr) {
  lst_t* self    = (lst_t*)_self;
  u_node_t* node = nullptr;
  u_node_t* prev = nullptr;
  u_node_t* next = nullptr;
  u_node_t* _idx = nullptr;

  u_nchk_if(self == nullptr);
  u_nchk_if(ptr == nullptr);

  node       = ptr + self->offset;
  node->prev = nullptr;
  node->next = nullptr;
  node->ptr  = self;

  if (idx == nullptr) {
    node->next = self->head;
    self->head = node;
  } else {
    _idx = idx + self->offset;
    u_err_if(_idx->ptr != self);

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

err:
}

bool lst_for_init(any_t _self, bool flag) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, false);

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

void lst_for_end(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_nchk_if(self == nullptr);

  self->flags[0] = 2;
}

any_t lst_for(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(self->len == 0, nullptr);
  u_chk_if(self->flags[3], nullptr);

  /* 初始化 */
  if (self->flags[2]) {
    self->iter     = self->flags[1] ? self->head : self->tail;
    self->flags[2] = !self->flags[2];
  } else { /* 迭代 */
    self->iter = self->flags[1] ? self->iter->next : self->iter->prev;

    /* 判断是否继续迭代 */
    if ((self->flags[1] && self->iter->next == nullptr) ||
        (!self->flags[1] && self->iter->prev == nullptr)) {
      self->flags[3] = true;
    }
  }

  return any(self->iter) - self->offset;
}
