/* libs */
#include "src/buf.h"
#include "src/map.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#include <stdio.h>

#define map_dis(map, fn)                                                                           \
  do {                                                                                             \
    inf("len %ld, _cap %ld: {", _(map)->len, _(map)->_cap);                                        \
                                                                                                   \
    for (size_t __i = 0; __i < _(map)->_cap; __i++) {                                              \
      if (_(map)->nodes[__i].hash == 0) {                                                          \
        continue;                                                                                  \
      }                                                                                            \
                                                                                                   \
      __prt("  [%ld] len %ld { \n", __i, _(map)->nodes[__i].hash);                                 \
                                                                                                   \
      map_T(map)* __node = _(map)->nodes[__i].next;                                                \
      for (; __node != nullptr; __node = __node->next) {                                           \
        __prt("    ");                                                                             \
        fn(__node->key, __node->value);                                                            \
      }                                                                                            \
                                                                                                   \
      __prt("  }\n");                                                                              \
    }                                                                                              \
                                                                                                   \
    __prt("}\n");                                                                                  \
  } while (0)

void fn(int k, c_str v) {
  fprintf(stderr, "{ %d: '%s' },\n", k, v);
}

int main(int argc, const char** argv) {
  map(int, c_str) m;

  map_init(&m);

  map_push(&m, 1, "one");
  map_push(&m, 2, "two");

  auto v = map_pop(&m, 2);

  inf("%d", v.key);
  inf("%s", v.value);

  map_dis(&m, fn);

  map_cleanup(&m);

  return 0;
}
