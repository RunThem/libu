#include <u/u.h>

u_lfq_ref_t que = nullptr;
#undef N1K
#define N1K 20

int __read(any_t arg) {
  u_each (i, N1K) {
    int n = u_lfq_pop(que, intptr_t);
    if (n == 0) {
      i--;
      continue;
    }

    mut_e(i + 1, n);
  }

  return 0;
}

int __write(any_t arg) {
  u_each (i, N1K) {
    u_lfq_put(que, (any_t)(intptr_t)(i + 1));
  }

  return 0;
}

test() {
  thrd_t r = {};
  thrd_t w = {};

  que = u_lfq_new();

  u_each (i, N1K) {
    u_lfq_put(que, (any_t)(intptr_t)(i + 1));

    mut_e(i + 1, u_lfq_pop(que, intptr_t));
  }

  thrd_create(&r, __read, nullptr);
  thrd_create(&w, __write, nullptr);

  thrd_join(r, nullptr);
  thrd_join(w, nullptr);
}
