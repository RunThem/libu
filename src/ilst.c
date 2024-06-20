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

#define u_vec_defs u_defs(vec, any_t)

#include <u/u.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct lst_t lst_t;
struct lst_t {
  u8_t flags[4];
  u_vec_t(any_t) items;

  size_t iter;
};

/***************************************************************************************************
 * Macro
 **************************************************************************************************/

/***************************************************************************************************
 * Function
 **************************************************************************************************/
any_t lst_new() {

  lst_t* self = nullptr;

  self = u_zalloc(sizeof(lst_t));
  u_nil_if(self);

  u_vec_init(self->items);
  u_nil_if(self->items);

  self->iter = 0;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

void lst_cleanup(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_vec_cleanup(self->items);

  u_free_if(self);
}

size_t lst_len(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, 0);

  return u_vec_len(self->items);
}

bool lst_exist(any_t _self, any_t ptr) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, false);
  u_chk_if(ptr == nullptr, false);
  u_chk_if(u_vec_is_empty(self->items), false);

  u_vec_for (self->items, i, node) {
    if (node == ptr) {
      return true;
    }
  }

  return false;
}

any_t lst_first(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(u_vec_is_empty(self->items), nullptr);

  return u_vec_at(self->items, 0ul);
}

any_t lst_last(any_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(u_vec_is_empty(self->items), nullptr);

  return u_vec_at(self->items, -1);
}

any_t lst_next(any_t _self, any_t idx) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(idx == nullptr, nullptr);

  u_vec_for (self->items, i, node) {
    if (node == idx) {
      return i == u_vec_len(self->items) - 1 ? nullptr : u_vec_at(self->items, i + 1);
    }
  }

  return nullptr;
}

any_t lst_prev(any_t _self, any_t idx) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(idx == nullptr, nullptr);

  u_vec_for (self->items, i, node) {
    if (node == idx) {
      return (i == 0) ? nullptr : u_vec_at(self->items, i - 1);
    }
  };

  return nullptr;
}

void lst_pop(any_t _self, any_t ptr) {
  lst_t* self = (lst_t*)_self;

  u_nchk_if(self == nullptr);
  u_nchk_if(ptr == nullptr);
  u_nchk_if(u_vec_is_empty(self->items));

  u_vec_for (self->items, i, node) {
    if (node == ptr) {
      u_vec_pop(self->items, i);
      break;
    }
  }
}

void lst_put(any_t _self, any_t idx, any_t ptr) {
  lst_t* self = (lst_t*)_self;

  u_nchk_if(self == nullptr);
  u_nchk_if(ptr == nullptr);

  if (idx == nullptr) {
    u_vec_put(self->items, 0, ptr);
  } else {
    u_vec_for (self->items, i, node) {
      if (node == idx) {
        u_vec_put(self->items, i + 1, ptr);
        break;
      }
    }
  }
}

bool lst_for_init(any_t _self, bool flag) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, false);

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
  u_chk_if(u_vec_is_empty(self->items), nullptr);
  u_chk_if(self->flags[3], nullptr);

  /* 初始化 */
  if (self->flags[2]) {
    self->iter     = self->flags[1] ? 0 : u_vec_len(self->items) - 1;
    self->flags[2] = !self->flags[2];
  } else { /* 迭代 */
    self->iter += (self->flags[1] ? +1 : -1);

    /* 判断是否继续迭代 */
    if (self->iter == 0 || self->iter == u_vec_len(self->items) - 1) {
      self->flags[3] = true;
    }
  }

  return u_vec_at(self->items, self->iter);
}
