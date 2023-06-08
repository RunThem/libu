/* libs */
#include "src/buf.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

int main(int argc, const char** argv) {
  vec(int) vv = nullptr;

  // vec_init_from(&vv, 100, 203);

  inf("%ld, %ld", vv->len, vv->cap);

  return 0;
}
