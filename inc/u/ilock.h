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

#pragma once

#ifndef U_ILOCK_H__
#  define U_ILOCK_H__

#  ifdef __cplusplus
extern "C" {
#  endif

#  include "u/utils.h"

#  include <stdatomic.h>

/* clang-format off */

/**
 * ::Class Lock
 *
 * 两类自旋锁, 都基于 _Atomic; 临界区建议用 u_*_if 作用域宏管理
 *
 *   u_spmtx_t mtx = {};           // 全零 == 未上锁, 也可以 u_spmtx_init
 *
 *   u_spmtx_if(&mtx) {
 *     ...                         // 进作用域加锁, 出作用域解锁
 *   }
 *
 *   u_rwmtx_t rw = {};
 *
 *   u_rwmtx_if(&rw, r) { ... }     // 读锁: 读者之间不互斥
 *   u_rwmtx_if(&rw, w) { ... }     // 写锁: 独占
 *
 * <threads.h> 的 mtx_t 可以配 u_mtx_if 用 (需要平台提供 <threads.h>; macOS 没有)
 *
 * 约定:
 *   - 全零初始化的锁就是"未上锁": = {} / 静态存储期 / u_*_init 三种都行
 *   - 自旋锁: 抢不到就空转, 没有 yield / backoff, 只适合很短的临界区
 *   - 不可重入: 同一线程重复加同一把锁 = 自死锁
 *   - 加解锁必须配对, 且只能由加锁者解锁 (没有 owner 记录)
 *   - 作用域宏的临界区里不要 break / return / goto: 会跳过解锁 -> 死锁
 *     (continue 反而会正常解锁并结束临界区)
 *   - rwmtx 写者优先: 写者先占住写标志再等读者退出, 所以持续写流量下读者可能饿死
 *   - 原子操作都用默认 memory order (seq_cst): 正确, 但不最快
 */

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
  _Atomic(bool) locked;
} u_spmtx_t, *u_spmtx_ref_t;

typedef struct {
  _Atomic(bool) rwlock;
  _Atomic(int) cnt;
} u_rwmtx_t, *u_rwmtx_ref_t;

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern void u_spmtx_init    (u_spmtx_ref_t);
extern void u_spmtx_lock    (u_spmtx_ref_t);
extern void u_spmtx_unlock  (u_spmtx_ref_t);
extern void u_rwmtx_init    (u_rwmtx_ref_t);
extern void u_rwmtx_rlock   (u_rwmtx_ref_t);
extern void u_rwmtx_runlock (u_rwmtx_ref_t);
extern void u_rwmtx_wlock   (u_rwmtx_ref_t);
extern void u_rwmtx_wunlock (u_rwmtx_ref_t);

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
/**
 * u_mtx_if(mtx) -> <block>
 *
 * C11 <threads.h> 的 mtx_t 作用域锁: 需要自己 include <threads.h>
 * 并 mtx_init (macOS 的 SDK 不提供 <threads.h>)
 */
#  define u_mtx_if(mtx)                                                                            \
    for (bool _ = ({ mtx_lock(mtx), true; }); _; _ = false, mtx_unlock(mtx))


/**
 * u_spmtx_if(mtx) -> <block>
 *
 * mtx 是 u_spmtx_t*: 进入加锁, 离开 (正常离开循环) 解锁
 */
#  define u_spmtx_if(mtx)                                                                          \
    for (bool _ = ({ u_spmtx_lock(mtx), true; }); _; _ = false, u_spmtx_unlock(mtx))


/**
 * u_rwmtx_if(mtx, m) -> <block>
 *
 * m 取 r 或 w: u_rwmtx_if(&rw, r) 读锁, u_rwmtx_if(&rw, w) 写锁
 */
#  define u_rwmtx_if(mtx, m)                                                                       \
    for (bool _ = ({ u_rwmtx_##m##lock(mtx), true; }); _; _ = false, u_rwmtx_##m##unlock(mtx))

/* clang-format on */

#  ifdef __cplusplus
} /* extern "C" */
#  endif

#endif /* !U_ILOCK_H__ */
