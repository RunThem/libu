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

  any_t item;
};

typedef struct list_t list_t, *list_ref_t;
struct [[gnu::packed]] list_t {
  typeof_unqual(*(u_vec_t(list_ref_t)){}) m;

  i32_t len;

  lnode_ref_t head;
  lnode_ref_t tail;
  lnode_ref_t free;
  lnode_ref_t iter;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pri lnode_ref_t list_find_node(list_ref_t self, any_t item) {
  lnode_ref_t node = nullptr;

  for (node = self->head; node->item != item; node = node->next)
    ;

  return node;
}

pub any_t $list_new() {
  list_ref_t self = nullptr;

  self = u_talloc(list_t);
  u_end_if(self);

  self->head  = nullptr;
  self->tail  = nullptr;
  self->m.ref = self;

  return self;

end:
  u_free_if(self);

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
  u_chk_if(self->len == 0, nullptr);

  return self->head->item;
}

pub any_t $list_tail(any_t _self) {
  list_ref_t self = (list_ref_t)_self;

  u_chk_if(self, nullptr);
  u_chk_if(self->len == 0, nullptr);

  return self->tail->item;
}

pub any_t $list_prev(any_t _self, any_t item) {
  list_ref_t self  = (list_ref_t)_self;
  lnode_ref_t node = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(item, nullptr);

  node = list_find_node(self, item);
  u_end_if(node);
  u_end_if(node->prev);

  return node->prev->item;

end:
  return nullptr;
}

pub any_t $list_next(any_t _self, any_t item) {
  list_ref_t self  = (list_ref_t)_self;
  lnode_ref_t node = nullptr;

  u_chk_if(self, nullptr);
  u_chk_if(item, nullptr);

  node = list_find_node(self, item);
  u_end_if(node);
  u_end_if(node->next);

  return node->next->item;

end:
  return nullptr;
}

pub void list_pop(any_t _self, any_t item) {
  list_ref_t self  = (list_ref_t)_self;
  lnode_ref_t node = nullptr;

  u_chk_if(self);
  u_chk_if(item);

  node = list_find_node(self, item);
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

  self->len--;
  self->m.len--;

  node->next = self->free;
  self->free = node;

  return;

end:
}

pub void $list_put(any_t _self, any_t pos, any_t item) {
  list_ref_t self      = (list_ref_t)_self;
  lnode_ref_t node     = nullptr;
  lnode_ref_t pos_node = nullptr;

  u_inf("pos{%p}, item{%p}", pos, item);

  u_chk_if(self);
  u_chk_if(item);

  node = u_talloc(lnode_t);
  u_end_if(node);

  node->prev = nullptr;
  node->next = nullptr;
  node->item = item;

  if (!pos) {
    node->next = self->head;
    self->head = node;
  } else {
    pos_node = list_find_node(self, pos);
    u_end_if(pos_node);

    node->next     = pos_node->next;
    node->prev     = pos_node;
    pos_node->next = node;
  }

  if (node->next == nullptr) {
    self->tail = node;
  } else {
    node->next->prev = node;
  }

  self->len++;
  self->m.len++;

  return;

end:
  u_free_if(node);
}

pub bool $list_each(any_t _self, any_t* item) {
  list_ref_t self = (list_ref_t)_self;

  u_chk_if(self, false);
  u_chk_if(self->len == 0, false);

  /* init */
  if (item == nullptr) {
    self->iter = self->head;
  } else {
    u_end_if(self->iter);

    *item      = self->iter->item;
    self->iter = self->iter->next;
  }

  return true;

end:
  return false;
}
