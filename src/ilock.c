/* MIT License
 *
 * Copyright (c) 2024 RunThem <iccy.fun@outlook.com>
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

pub void u_spmtx_init(u_spmtx_ref_t self) {
  u_chk_if(self);

  u_atomic_init(&self->locked, false);
}

pub void u_spmtx_lock(u_spmtx_ref_t self) {
  u_chk_if(self);

  while (u_atomic_swap(&self->locked, true))
    ;
}

pub void u_spmtx_unlock(u_spmtx_ref_t self) {
  u_chk_if(self);

  u_atomic_swap(&self->locked, false);
}

pub void u_rwmtx_init(u_rwmtx_ref_t self) {
  u_chk_if(self);

  u_atomic_init(&self->cnt, 0);
  u_atomic_init(&self->rwlock, false);
}

pub void u_rwmtx_rlock(u_rwmtx_ref_t self) {
  u_chk_if(self);

  while (true) {
    while (u_atomic_pop(&self->rwlock))
      ;

    u_atomic_add(&self->cnt, 1);
    u_brk_if(u_atomic_pop(&self->rwlock) == false);

    u_atomic_sub(&self->cnt, 1);
  }
}

pub void u_rwmtx_runlock(u_rwmtx_ref_t self) {
  u_chk_if(self);

  u_atomic_sub(&self->cnt, 1);
}

pub void u_rwmtx_wlock(u_rwmtx_ref_t self) {
  u_chk_if(self);

  while (u_atomic_swap(&self->rwlock, true))
    ;

  while (u_atomic_pop(&self->cnt))
    ;
}

pub void u_rwmtx_wunlock(u_rwmtx_ref_t self) {
  u_chk_if(self);

  u_atomic_swap(&self->rwlock, false);
}
