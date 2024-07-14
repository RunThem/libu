#include <u/u.h>

test() {
  /* #[[heap<int>]] */
  u_heap_t(int) h = nullptr;

  h = u_heap_new(int, true, fn_cmp(int));

  u_each (i, N1W) {
    u_heap_put(h, N1W - i - 1);
  }

  mut_e(N1W, u_heap_len(h));

  u_each (i, N1W) {
    mut_e(i, u_heap_pop(h));
  }

  mut_e(0, u_heap_len(h));
  mut_e(0, u_heap_pop(h));
  mut_e(0, u_heap_pop(h));

  u_heap_cleanup(h);
}
