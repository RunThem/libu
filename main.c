/* libs */
#include "src/buf.h"
#include "src/map.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#include <stdio.h>

int main(int argc, const char** argv) {
  map(int, str_t) m;

  map_init(&m);

  map_cleanup(&m);

  return 0;
}
