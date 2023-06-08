/* libs */
#include "src/buf.h"
#include "src/map.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#include <stdio.h>

void fn(int k, c_str v) {
  fprintf(stderr, "{ %d: '%s' },\n", k, v);
}

void fn_1(int k) {
  fprintf(stderr, "%d", k);
}

void fn_2(c_str v) {
  fprintf(stderr, "%s", v);
}

int main(int argc, const char** argv) {
  map(int, c_str) m;

  map_init(&m);

  map_push(&m, 1, "one");
  map_push(&m, 2, "two");

  auto v = map_keys(&m);

  vec_dis(&v, fn_1);

  auto k = map_values(&m);

  vec_dis(&k, fn_2);

  map_dis(&m, fn);

  map_cleanup(&m);

  return 0;
}
