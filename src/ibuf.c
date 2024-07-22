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
 * Function
 **************************************************************************************************/
void buf_init(u_buf_ref_t self, u8_t* buf, size_t cap) {
  u_chk_if(self);

  if (buf != nullptr) {
    self->__rawbuf = buf;
  } else {
    self->__rawbuf = u_zalloc(cap);
    u_end_if(self->__rawbuf);

    self->alloc_flag = true;
  }

  self->len = 0;
  self->cap = cap;
  self->buf = self->__rawbuf;

  return;

end:
}

void buf_clear(u_buf_ref_t self) {
  u_chk_if(self);

  self->len = 0;
  self->buf = self->__rawbuf;
}

void buf_cleanup(u_buf_ref_t self) {
  u_chk_if(self);

  if (self->alloc_flag) {
    u_free_if(self->__rawbuf);
  }
}

size_t buf_len(u_buf_ref_t self) {
  u_chk_if(self, 0);

  return self->len;
}

void buf_skip(u_buf_ref_t self, size_t len) {
  u_chk_if(self);
  u_chk_if(len > self->len);

  self->buf += len;
}

void buf_pop(u_buf_ref_t self, any_t buf, size_t len) {
  u_chk_if(self);
  u_chk_if(buf);
  u_chk_if(len > self->len);

  memcpy(buf, self->buf, len);

  self->len -= len;
  self->buf += len;
}

void buf_put(u_buf_ref_t self, any_t buf, size_t len) {
  size_t diff = 0;

  u_chk_if(self);
  u_chk_if(buf);

  diff = self->buf - self->__rawbuf;

  /* realloc */
  if (self->cap - (diff + self->len) < len) {
    if (self->cap - self->len >= len) {
      memmove(self->__rawbuf, self->buf, self->len);
      self->buf = self->__rawbuf;
    } else {
      u_end_if(!self->alloc_flag);

      self->__rawbuf = u_realloc(self->__rawbuf, self->cap + len);
      u_end_if(self->__rawbuf);

      self->cap += len;
      self->buf = self->__rawbuf + diff;
    }
  }

  memcpy(self->buf + self->len, buf, len);

  self->len += len;

  return;

end:
}
