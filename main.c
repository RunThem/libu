/* libs */
#include "src/buf.h"
#include "src/fs.h"
#include "src/list.h"
#include "src/map.h"
#include "src/sock.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#include <stdio.h>

int main(int argc, const char** argv) {

  vec(int) vv = nullptr;

  vec_init_from(&vv, 1, 2, 3, 4, 5, 6);

  vec_for(&vv, it) {
    inf("%d", *it);
  }

  vec_pop(&vv, 1);

  vec_for(&vv, it) {
    inf("%d", *it);
  }

  return 0;
}
