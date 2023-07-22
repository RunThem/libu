/* libs */
#include "src/buf.h"
#include "src/fs.h"
#include "src/list.h"
#include "src/map.h"
#include "src/que.h"
#include "src/sock.h"
#include "src/stack.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#define _typeof(t) __builtin_classify_type(t)

int main(int argc, const char** argv) {

  stack(int) st = {0};
  stack_init(&st, sizeof(int), 10);

  for (int i = 0; i < 110; i++) {
    stack_push(&st, i);
  }

  for (int i = 0; i < stack_len(&st); i++) {
    inf("%d, ", i);
  }

  return 0;
}
