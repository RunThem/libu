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

#include <u/core.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct lst_t lst_t;
struct lst_t {
  bool flags[4];
  uvec(any_t) items;

  size_t iter;
};

/***************************************************************************************************
 * Macro
 **************************************************************************************************/

/***************************************************************************************************
 * Function
 **************************************************************************************************/
u_lst_t lst_new() {

  lst_t* self = nullptr;

  self = u_zalloc(sizeof(lst_t));
  u_nil_if(self);

  uv_init(self->items);
  u_nil_if(self->items);

  self->iter = 0;

  return (u_lst_t)self;

err:
  u_free_if(self);

  return nullptr;
}

void lst_cleanup(u_lst_t _self) {
  lst_t* self = (lst_t*)_self;

  uv_cleanup(self->items);

  u_free_if(self);
}

size_t lst_len(u_lst_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, 0);

  return uv_len(self->items);
}

bool lst_exist(u_lst_t _self, any_t ptr) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, false);
  u_chk_if(ptr == nullptr, false);
  u_chk_if(uv_empty(self->items), false);

  uv_for_all(self->items, i, node) {
    if (node == ptr) {
      return true;
    }
  }

  return false;
}

any_t lst_first(u_lst_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(uv_empty(self->items), nullptr);

  return uv_at(self->items, 0ul);
}

any_t lst_last(u_lst_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(uv_empty(self->items), nullptr);

  return uv_at(self->items, -1ul);
}

any_t lst_next(u_lst_t _self, any_t idx) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(idx == nullptr, nullptr);

  uv_for_all(self->items, i, node) {
    if (node == idx) {
      return uv_at(self->items, i + 1);
    }
  }

  return nullptr;
}

any_t lst_prev(u_lst_t _self, any_t idx) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(idx == nullptr, nullptr);

  uv_for_all(self->items, i, node) {
    if (node == idx) {
      return (i == 0) ? nullptr : uv_at(self->items, i - 1);
    }
  };

  return nullptr;
}

void lst_pop(u_lst_t _self, any_t ptr) {
  lst_t* self = (lst_t*)_self;

  u_nchk_if(self == nullptr);
  u_nchk_if(ptr == nullptr);
  u_nchk_if(uv_empty(self->items));

  uv_for_all(self->items, i, node) {
    if (node == ptr) {
      uv_pop(self->items, i);
      break;
    }
  }
}

void lst_put(u_lst_t _self, any_t idx, any_t ptr) {
  lst_t* self = (lst_t*)_self;

  u_nchk_if(self == nullptr);
  u_nchk_if(ptr == nullptr);

  if (idx == nullptr) {
    uv_put(self->items, 0, ptr);
  } else {
    uv_for_all(self->items, i, node) {
      if (node == idx) {
        uv_put(self->items, i + 1, ptr);
        break;
      }
    }
  }
}

bool lst_each_init(u_lst_t _self, bool flag) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, false);
  u_chk_if(self->flags[0], false);

  self->flags[0] = true;
  self->flags[1] = flag;
  self->flags[2] = true;
  self->flags[3] = false;

  return true;
}

any_t lst_each(u_lst_t _self) {
  lst_t* self = (lst_t*)_self;

  u_chk_if(self == nullptr, nullptr);
  u_chk_if(uv_empty(self->items), nullptr);
  u_chk_if(self->flags[3], nullptr);

  /* 初始化 */
  if (self->flags[2]) {
    self->iter     = self->flags[1] ? 0 : uv_len(self->items) - 1;
    self->flags[2] = !self->flags[2];
  } else { /* 迭代 */
    self->iter += (self->flags[1] ? +1 : -1);

    /* 判断是否继续迭代 */
    if (self->iter == 0 || self->iter == uv_len(self->items) - 1) {
      self->flags[3] = true;
    }
  }

  return uv_at(self->items, self->iter);
}
