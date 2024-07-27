#include <u/u.h>

u_lfq_ref_t que = nullptr;

int __read(any_t arg) {
  u_each (i, N1K) {
    mut_e(i, (int)(intptr_t)u_lfq_pop(que));
  }

  return 0;
}

int __write(any_t arg) {
  u_each (i, N1K) {
    u_lfq_put(que, (any_t)(intptr_t)i);
  }

  return 0;
}

test() {
  thrd_t r = {};
  thrd_t w = {};

  que = u_lfq_new();

  thrd_create(&r, __read, nullptr);
  thrd_create(&w, __write, nullptr);

  thrd_join(r, nullptr);
  thrd_join(w, nullptr);
}
