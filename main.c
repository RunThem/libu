/* libs */
#include "src/buf.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#include <stdio.h>

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
