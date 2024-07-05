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
typedef struct node_t node_t;
struct node_t {
  node_t* prev;
  node_t* next;
  any_t item;
};

typedef struct lst_t lst_t;
struct lst_t {
  u8_t flags[4];

  node_t* head;
  node_t* tail;

  node_t* free;
  node_t* iter;

  size_t len;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
static node_t* lst_alloc(lst_t* self, any_t ptr) {
  node_t* node = nullptr;

  if (self->free != nullptr) {
    node       = self->free;
    self->free = self->free->next;
  } else {
    node = u_talloc(node_t);
  }

  node->item = ptr;

  return node;
}

static void lst_free(lst_t* self, node_t* node) {
  u_nchk_if(node == nullptr);

  bzero(node, sizeof(node_t));

  node->next = self->free;
  self->free = node;
}

static node_t* lst_ptr_to_node(lst_t* self, any_t ptr) {
  node_t* _idx = nullptr;

  for (_idx = self->head; _idx != nullptr && _idx->item != ptr; _idx = _idx->next)
    ;

  return _idx;
}

any_t lst_new() {
  lst_t* self = nullptr;

  self = u_talloc(lst_t);
  u_nil_if(self);

  self->head = nullptr;
  self->tail = nullptr;
  self->len  = 0;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

void lst_cleanup(any_t _self) {
  lst_t* self  = (lst_t*)_self;
  node_t* node = nullptr;

  u_nchk_if(self == nullptr);

  while (self->free != nullptr) {
    node       = self->free;
    self->free = node->next;

    u_free(node);
  }

  u_free_if(self);
}

size_t lst_len(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, 0);

  return self->len;
}

bool lst_exist(any_t _self, any_t ptr) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, false);
  u_chk_if(ptr == nullptr, false);
  u_chk_if(self->len == 0, false);

  return lst_ptr_to_node(self, ptr) != nullptr;
}

any_t lst_head(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(self->len == 0, nullptr);

  return self->head->item;
}

any_t lst_tail(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(self->len == 0, nullptr);

  return self->tail->item;
}

any_t lst_prev(any_t _self, any_t ptr) {
  lst_t* self  = (lst_t*)_self;
  node_t* node = nullptr;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(ptr == nullptr, nullptr);

  node = lst_ptr_to_node(self, ptr);
  u_nil_if(node);
  u_nil_if(node->prev);

  return node->prev->item;

err:
  return nullptr;
}

any_t lst_next(any_t _self, any_t ptr) {
  lst_t* self  = (lst_t*)_self;
  node_t* node = nullptr;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(ptr == nullptr, nullptr);

  node = lst_ptr_to_node(self, ptr);
  u_nil_if(node);
  u_nil_if(node->next);

  return node->next->item;

err:
  return nullptr;
}

void lst_pop(any_t _self, any_t ptr) {
  lst_t* self  = (lst_t*)_self;
  node_t* node = nullptr;
  node_t* prev = nullptr;
  node_t* next = nullptr;
  node_t* _idx = nullptr;

  u_nchk_if(self == nullptr);
  u_nchk_if(ptr == nullptr);

  _idx = lst_ptr_to_node(self, ptr);
  u_nil_if(_idx);

  if (_idx->prev != nullptr) {
    _idx->prev->next = _idx->next;
  } else {
    self->head = _idx->next;
  }

  if (_idx->next != nullptr) {
    _idx->next->prev = _idx->prev;
  } else {
    self->tail = _idx->prev;
  }

  self->len--;

  lst_free(self, _idx);

  return;

err:
}

void lst_put(any_t _self, any_t idx, any_t ptr) {
  lst_t* self  = (lst_t*)_self;
  node_t* node = nullptr;
  node_t* prev = nullptr;
  node_t* next = nullptr;
  node_t* _idx = nullptr;

  u_nchk_if(self == nullptr);
  u_nchk_if(ptr == nullptr);

  node = lst_alloc(self, ptr);
  u_nil_if(node);

  if (idx == nullptr) {
    if (self->len == 0) {
      self->head = self->tail = node;
    } else {
      node->next       = self->head;
      node->next->prev = node;
      self->head       = node;
    }
  } else {
    _idx = lst_ptr_to_node(self, idx);
    u_nil_if(_idx);

    node->next = _idx->next;
    node->prev = _idx;
    _idx->next = node;

    if (node->next != nullptr) {
      node->next->prev = node;
    } else {
      self->tail = node;
    }
  }

  self->len++;

  return;

err:
  lst_free(self, node);
}

bool lst_for_init(any_t _self, bool flag) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, false);

  if (self->flags[0] == 0) {
    self->flags[0] = 1;
  } else if (self->flags[0] == 2) {
    self->flags[0] = 0;
  }

  self->flags[0] = !self->flags[0];
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

  return self->iter->item;
}
