/* libs */
#include "src/buf.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#include <stdio.h>

#define vec_dis(v, fn)                                                                             \
  do {                                                                                             \
    __inf("len %ld, cap %ld: {", _(v)->len, _(v)->cap);                                            \
    for (size_t i = 0; i < _(v)->len; i++) {                                                       \
      fn(_(v)->data[i]);                                                                           \
      __prt(", ");                                                                                 \
    }                                                                                              \
    __prt("\b\b}\n");                                                                              \
  } while (0)

void pr(int a) {
  fprintf(stderr, "%d", a);
}

int main(int argc, const char** argv) {
  vec(int) vv = nullptr;

  vec_init_from(&vv, 100, 203);

  vec_push_b(&vv, 100);

  vec_push_f(&vv, 1000);

  vec_insert(&vv, 0, 500);

  vec_dis(&vv, pr);

  return 0;
}
