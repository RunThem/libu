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

#include <u/istr.h>
#include <u/utils/alloc.h>
#include <u/utils/debug.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct str_t str_t;
struct str_t {
  size_t len;
  size_t cap;
  u_cstr_t data;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
u_str_t str_new() {
  str_t* self = nullptr;

  self = u_zalloc(sizeof(str_t));
  u_nil_if(self);

  self->data = u_zalloc(16);
  u_nil_if(self->data);

  self->len = 0;
  self->cap = 16;

  return (u_str_t)self;

err:
  u_free_if(self);

  return nullptr;
}

size_t str_len(u_str_t _self) {
  str_t* self = (str_t*)_self;

  u_chk_if(self == nullptr, 0);

  return self->len;
}

u_cstr_t str_tocstr(u_str_t _self) {
  str_t* self = (str_t*)_self;

  u_chk_if(self == nullptr, nullptr);

  return self->data;
}
