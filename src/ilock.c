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

#include "u/ilock.h"

/***************************************************************************************************
 * Function
 **************************************************************************************************/
pub void u_spmtx_init(u_spmtx_ref_t self) {
  u_chk_if(self);

  atomic_init(&self->locked, false);
}

/* 自旋互斥锁: TAS (test-and-set), exchange 永远写 true,
   旧值是 true 说明别人持锁 -> 继续空转; 旧值是 false 说明本轮自己抢到了 */
pub void u_spmtx_lock(u_spmtx_ref_t self) {
  while (atomic_exchange(&self->locked, true))
    ;
}

pub void u_spmtx_unlock(u_spmtx_ref_t self) {
  atomic_exchange(&self->locked, false);
}

pub void u_rwmtx_init(u_rwmtx_ref_t self) {
  u_chk_if(self);

  atomic_init(&self->cnt, 0);
  atomic_init(&self->rwlock, false);
}

/* 读锁: 先等写标志落下, 再登记读者 (cnt++),
   然后复查写标志: 若这中间写者插了进来, 就退回登记并重试 */
pub void u_rwmtx_rlock(u_rwmtx_ref_t self) {
  while (true) {
    while (atomic_load(&self->rwlock))
      ;

    atomic_fetch_add(&self->cnt, 1);
    u_brk_if(atomic_load(&self->rwlock) == false);

    atomic_fetch_sub(&self->cnt, 1);
  }
}

pub void u_rwmtx_runlock(u_rwmtx_ref_t self) {
  atomic_fetch_sub(&self->cnt, 1);
}

/* 写锁: 先占住写标志 (后来者不论是读是写都会被挡住), 再等已登记的读者退完 */
pub void u_rwmtx_wlock(u_rwmtx_ref_t self) {
  while (atomic_exchange(&self->rwlock, true))
    ;

  while (atomic_load(&self->cnt))
    ;
}

pub void u_rwmtx_wunlock(u_rwmtx_ref_t self) {
  atomic_exchange(&self->rwlock, false);
}
