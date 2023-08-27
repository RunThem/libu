/* libs */
// #include "buf.h"
// #include "fs.h"
// #include "list.h"
// #include "map.h"
// #include "que.h"
// #include "sock.h"
#include "stack.h"
// #include "str.h"
#include "u.h"
// #include "vec.h"

#define _typeof(t) __builtin_classify_type(t)

int main(int argc, const char** argv) {

  stack(int) st = nullptr;

  st = stack_new(int, 32);

  stack_push(st, 32);

  stack_push(st, 10);

  stack_pop(st);
  inf("%d", stack_peek(st));

  return 0;
}
