/* libs */
// #include "buf.h"
#include "buf.h"
#include "fs.h"
#include "list.h"
#include "map.h"
#include "que.h"
#include "sock.h"
#include "stack.h"
#include "str.h"
#include "u.h"
#include "vec.h"

#define _typeof(t) __builtin_classify_type(t)

int main(int argc, const char** argv) {

  list(int) lst = {0};

  list_init(&lst);

  for (size_t i = 0; i < 10; i++) {
    list_push_b(&lst, i);
  }

  inf("%ld", list_len(&lst));

  list_for(&lst, it) {
    inf("%d", it->it);
  }

  // Generic map data structure

  return 0;
}
