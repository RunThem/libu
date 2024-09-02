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
  bool is_alloc;
  bool is_append;
  size_t cap;

  u8_t* rawbuf;
  u8_t* begin;
  u8_t* end;
} buf_t, *buf_ref_t;

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pub u_buf_ref_t buf_new(u8_t* buf, size_t cap) {
  buf_ref_t self = nullptr;

  self = u_talloc(buf_t);
  u_end_if(self);

  if (buf != nullptr) {
    self->rawbuf = buf;
  } else {
    self->is_alloc = true;

    self->rawbuf = u_zalloc(cap);
    u_end_if(self->rawbuf);
  }

  self->begin = self->rawbuf;
  self->end   = self->rawbuf;
  self->cap   = cap;

  return (u_buf_ref_t)self;

end:
  u_free_if(self);

  return nullptr;
}

pub void buf_clear(u_buf_ref_t _self) {
  buf_ref_t self = (buf_ref_t)_self;

  u_chk_if(self);

  self->begin = self->rawbuf;
  self->end   = self->rawbuf;
}

pub void buf_cleanup(u_buf_ref_t _self) {
  buf_ref_t self = (buf_ref_t)_self;

  u_chk_if(self);

  if (self->is_alloc) {
    u_free(self->rawbuf);
  }

  u_free(self);
}

pub size_t buf_len(u_buf_ref_t _self) {
  buf_ref_t self = (buf_ref_t)_self;

  u_chk_if(self, -1);

  return self->end - self->begin;
}

pub void buf_pop(u_buf_ref_t _self, any_t buf, size_t len) {
  buf_ref_t self = (buf_ref_t)_self;

  u_chk_if(self);
  u_chk_if(buf);
  u_chk_if(len > self->end - self->begin);

  if (self->is_append) {
    self->end -= len;
    memcpy(buf, self->end, len);
  } else {
    memcpy(buf, self->begin, len);
    self->begin += len;
  }
}

pub void buf_put(u_buf_ref_t _self, any_t buf, size_t len) {
  size_t diff    = 0;
  size_t size    = 0;
  buf_ref_t self = (buf_ref_t)_self;

  u_chk_if(self);

  u_chk_if(self);
  u_chk_if(buf);

  diff = self->begin - self->rawbuf;
  size = self->end - self->begin;

  /* realloc */
  if (self->cap - (diff + size) < len) {
    if (self->cap - size >= len) {
      memmove(self->rawbuf, self->begin, size);
      self->begin = self->rawbuf;
      self->end   = self->rawbuf + size;
    } else {
      u_end_if(!self->is_alloc);

      self->rawbuf = u_realloc(self->rawbuf, self->cap + len);
      u_end_if(self->rawbuf);

      self->cap += len;
      self->begin = self->rawbuf + diff;
      self->end   = self->begin + size;
    }
  }

  memcpy(self->end, buf, len);

  self->end += len;

  return;

end:
}
