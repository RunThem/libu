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

typedef struct lnode_t lnode_t, *lnode_ref_t;
struct lnode_t {
  lnode_ref_t prev;
  lnode_ref_t next;

  any_t uptr;
};

typedef struct list_t list_t, *list_ref_t;
struct [[gnu::packed]] list_t {
  typeof_unqual(*(u_list_t(list_ref_t)){}) m;

  lnode_ref_t head;
  lnode_ref_t tail;
  lnode_ref_t free;
  lnode_ref_t iter;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri lnode_ref_t list_find_node(list_ref_t self, any_t uptr) {
  lnode_ref_t node = nullptr;

  for (node = self->head; node->uptr != uptr; node = node->next)
    ;

  return node;
}

pub any_t $list_new() {
  list_ref_t self = nullptr;

  self = u_talloc(list_t);
  u_end_if(self);

  self->head  = nullptr;
  self->tail  = nullptr;
  self->m.ref = any(self);
  self->m.len = 0;

  return self;

end:
  return nullptr;
}

pub void list_cleanup(any_t _self) {
  list_ref_t self = (list_ref_t)_self;

  u_chk_if(self);

  u_free_if(self);
}

pub any_t $list_head(any_t _self) {
  list_ref_t self = (list_ref_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(self->m.len == 0, nullptr);

  return self->head->uptr;
}

pub any_t $list_tail(any_t _self) {
  list_ref_t self = (list_ref_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(self->m.len == 0, nullptr);

  return self->tail->uptr;
}

pub any_t $list_prev(any_t _self, any_t uptr) {
  list_ref_t self  = (list_ref_t)_self;
  lnode_ref_t node = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(uptr, nullptr);

  node = list_find_node(self, uptr);
  u_end_if(node);
  u_end_if(node->prev);

  return node->prev->uptr;

end:
  return nullptr;
}

pub any_t $list_next(any_t _self, any_t uptr) {
  list_ref_t self  = (list_ref_t)_self;
  lnode_ref_t node = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(uptr, nullptr);

  node = list_find_node(self, uptr);
  u_end_if(node);
  u_end_if(node->next);

  return node->next->uptr;

end:
  return nullptr;
}

pub void list_del(any_t _self, any_t uptr) {
  list_ref_t self  = (list_ref_t)_self;
  lnode_ref_t node = nullptr;

  u_chk_if(self);
  u_chk_if(uptr);

  node = list_find_node(self, uptr);
  u_end_if(node);

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

  self->m.len--;

  node->next = self->free;
  self->free = node;

  return;

end:
}

pub any_t $list_add(any_t _self, any_t uidxptr) {
  list_ref_t self     = (list_ref_t)_self;
  lnode_ref_t node    = nullptr;
  lnode_ref_t idxnode = nullptr;

  u_chk_if(self, nullptr);

  node = u_talloc(lnode_t);
  u_end_if(node);

  node->prev = nullptr;
  node->next = nullptr;

  if (!uidxptr) {
    node->next = self->head;
    self->head = node;
  } else {
    idxnode = list_find_node(self, uidxptr);
    u_end_if(idxnode);

    node->next    = idxnode->next;
    node->prev    = idxnode;
    idxnode->next = node;
  }

  if (node->next == nullptr) {
    self->tail = node;
  } else {
    node->next->prev = node;
  }

  self->m.len++;

  return &node->uptr;

end:
  u_free_if(node);

  return nullptr;
}

pub any_t $list_each(any_t _self, bool init) {
  list_ref_t self  = (list_ref_t)_self;
  lnode_ref_t iter = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(self->m.len == 0, nullptr);

  /* init */
  if (init) {
    iter = self->iter = self->head;
  } else {
    u_end_if(self->iter);

    iter       = self->iter;
    self->iter = self->iter->next;
  }

  return iter->uptr;

end:
  return nullptr;
}

pub any_t $list_reach(any_t _self, bool init) {
  list_ref_t self  = (list_ref_t)_self;
  lnode_ref_t iter = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(self->m.len == 0, nullptr);

  /* init */
  if (init) {
    iter = self->iter = self->tail;
  } else {
    u_end_if(self->iter);

    iter       = self->iter;
    self->iter = self->iter->prev;
  }

  return iter->uptr;

end:
  return nullptr;
}
