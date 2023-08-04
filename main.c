/* libs */
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

  map(int, char) mm = {};

  map_init(&mm);

  for (size_t i = 'a'; i <= 'z'; i++) {
    map_push(&mm, i, i);
  }

  inf("len is %ld", map_len(&mm));

  map_for(&mm, i) {
    inf("key(%d), val('%c')", mm.key, mm.val);
  }

  for (size_t i = 'f'; i <= 'k'; i++) {
    map_pop(&mm, i);
    inf("pop(%d, '%c')", mm.key, mm.val);
  }

  inf("len is %ld", map_len(&mm));

  map_for(&mm, i) {
    inf("key(%d), val('%c')", mm.key, mm.val);
  }

  return 0;
}
