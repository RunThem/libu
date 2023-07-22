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
  map(int, char) mm;

  map_init(&mm, sizeof(int), sizeof(char), map_int_hash);

  map_push(&mm, 273478210, 'a');
  map_push(&mm, 27478211, 'b');
  map_push(&mm, 2778212, 'c');
  map_push(&mm, 27848213, 'd');
  map_push(&mm, 278348214, 'e');
  map_push(&mm, 27815, 'f');
  map_push(&mm, 2783216, 'j');

  map_for(&mm, it) {
    inf("%d -> '%c'", it->key, it->val);
  }

  map_pop(&mm, 27848213);
  map_pop(&mm, 278348214);
  map_pop(&mm, 27815);

  inf("map change: ");

  map_for(&mm, it) {
    inf("%d -> '%c'", it->key, it->val);
  }

  return 0;
}
