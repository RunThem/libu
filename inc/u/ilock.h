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

#ifndef U_ILOCK_H__
#define U_ILOCK_H__

#include "utils/type.h"
#include "utils/va.h"

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
  _Atomic(bool) locked;
} u_spmtx_t;

typedef struct {
  _Atomic(bool) rwlock;
  _Atomic(int) cnt;
} u_rwmtx_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern void u_spmtx_init(u_spmtx_t*);

extern void u_spmtx_lock(u_spmtx_t*);

extern void u_spmtx_unlock(u_spmtx_t*);

extern void u_rwmtx_init(u_rwmtx_t*);

extern void u_rwmtx_rlock(u_rwmtx_t*);

extern void u_rwmtx_runlock(u_rwmtx_t*);

extern void u_rwmtx_wlock(u_rwmtx_t*);

extern void u_rwmtx_wunlock(u_rwmtx_t*);

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#define u_mtx_if(mtx) for (bool _ = (mtx_lock(mtx), true); _; _ = false, mtx_unlock(mtx))

#define u_smtx_if(mtx) for (bool _ = (u_spmtx_lock(mtx), true); _; _ = false, u_spmtx_unlock(mtx))

#define u_rwmtx_if(mtx, m)                                                                         \
  for (bool _ = (u_rwmtx_##m##lock(mtx), true); _; _ = false, u_rwmtx_##m##unlock(mtx))

#endif /* !U_ILOCK_H__ */
