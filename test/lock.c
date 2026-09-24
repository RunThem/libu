/***************************************************************************************************
 * Test: Lock
 *
 * 覆盖: spmtx zero-init / rwmtx manual/scope / spmtx 4 threads / rwmtx 1w+3r
 **************************************************************************************************/

#include "pub.h"

/* ================================== Lock ================================== */

typedef struct {
  u_spmtx_t mtx; /* 故意用非原子的计数器: 锁坏了必然丢更新 */
  int counter;
  int rounds;
} lock_spin_ctx_t;

static tb_int_t lock_spin_worker(tb_cpointer_t priv) {
  lock_spin_ctx_t* ctx = (lock_spin_ctx_t*)priv;

  for (int i = 0; i < ctx->rounds; i++) {
    u_spmtx_if (&ctx->mtx) {
      int v = ctx->counter; /* 读 */

      ctx->counter = v + 1; /* 写: 没有互斥就会丢 */
    }
  }

  return 0;
}

typedef struct {
  u_rwmtx_t mtx;
  int a; /* 不变式: a + b 恒等于 0 */
  int b;
  int wrounds;
  int rrounds;
  int violations; /* 读者看到中间状态的次数 */
  int nonzero;    /* 读者看到写者已经动过的次数 (确认测试不是空跑) */
} lock_rw_ctx_t;

static tb_int_t lock_rw_writer(tb_cpointer_t priv) {
  lock_rw_ctx_t* ctx = (lock_rw_ctx_t*)priv;

  for (int i = 0; i < ctx->wrounds; i++) {
    u_rwmtx_if (&ctx->mtx, w) {
      ctx->a++;
      ctx->b--;
    }
  }

  return 0;
}

static tb_int_t lock_rw_reader(tb_cpointer_t priv) {
  lock_rw_ctx_t* ctx = (lock_rw_ctx_t*)priv;

  for (int i = 0; i < ctx->rrounds; i++) {
    u_rwmtx_if (&ctx->mtx, r) {
      if (ctx->a + ctx->b != 0) {
        ctx->violations++;
      }
      if (ctx->a != 0) {
        ctx->nonzero++;
      }
    }
  }

  return 0;
}

int main(void) {
  tb_init(NULL, NULL); /* tbox 的线程层需要先初始化 */

  printf("================ Lock ================\n");

  /* ---- 1. 全零即未上锁 / init / 手动配对 ---- */
  {
    u_spmtx_t mtx = {};
    int v         = 0;

    u_spmtx_if (&mtx) { /* 没 init 也能用: 全零就是未上锁 */
      v = 1;
    }
    assert(v == 1);

    u_spmtx_lock(&mtx);
    u_spmtx_unlock(&mtx);

    u_spmtx_init(&mtx);
    u_spmtx_if (&mtx) {
      v = 2;
    }
    assert(v == 2);
  }
  printf("  spmtx zero-init     ok\n");

  /* ---- 2. 读写锁: 手动配对 + 作用域宏 ---- */
  {
    u_rwmtx_t rw = {};
    int v        = 0;

    u_rwmtx_rlock(&rw);
    v = 1;
    u_rwmtx_runlock(&rw);

    u_rwmtx_wlock(&rw);
    v = 2;
    u_rwmtx_wunlock(&rw);

    u_rwmtx_init(&rw);
    u_rwmtx_if (&rw, r) {
      v = 3;
    }
    u_rwmtx_if (&rw, w) {
      v = 4;
    }
    assert(v == 4);
  }
  printf("  rwmtx manual/scope  ok\n");

  /* ---- 3. 自旋锁并发: 非原子计数器必须精确等于 线程数 * 轮数 ---- */
  {
    lock_spin_ctx_t ctx        = {.rounds = 25000};
    tb_thread_ref_t threads[4] = {NULL};

    for (int i = 0; i < 4; i++) {
      threads[i] = tb_thread_init(NULL, lock_spin_worker, &ctx, 0);
      assert(threads[i]);
    }
    for (int i = 0; i < 4; i++) {
      assert(tb_thread_wait(threads[i], -1, NULL) > 0);
      tb_thread_exit(threads[i]);
    }

    assert(ctx.counter == 4 * 25000); /* 少一次就是锁没起作用 */
  }
  printf("  spmtx 4 threads     ok\n");

  /* ---- 4. 读写锁并发: 读者不应看到写者的中间状态 ---- */
  {
    lock_rw_ctx_t ctx          = {.wrounds = 20000, .rrounds = 20000};
    tb_thread_ref_t writer     = NULL;
    tb_thread_ref_t readers[3] = {NULL};

    writer = tb_thread_init(NULL, lock_rw_writer, &ctx, 0);
    assert(writer);
    for (int i = 0; i < 3; i++) {
      readers[i] = tb_thread_init(NULL, lock_rw_reader, &ctx, 0);
      assert(readers[i]);
    }

    assert(tb_thread_wait(writer, -1, NULL) > 0);
    tb_thread_exit(writer);
    for (int i = 0; i < 3; i++) {
      assert(tb_thread_wait(readers[i], -1, NULL) > 0);
      tb_thread_exit(readers[i]);
    }

    assert(ctx.violations == 0); /* 读锁里看到 a + b != 0 就是没互斥住 */
    assert(ctx.nonzero > 0);     /* 确认读者确实和写者交替过 */
    assert(ctx.a == 20000);
    assert(ctx.b == -20000);
  }
  printf("  rwmtx 1w+3r         ok\n");

  printf("============ all Lock tests passed ============\n");

  printf("\n");

  return 0;
}
