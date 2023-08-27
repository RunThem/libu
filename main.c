/* libs */
// #include "buf.h"
// #include "fs.h"
// #include "list.h"
#include "map.h"
// #include "que.h"
// #include "sock.h"
// #include "stack.h"
// #include "str.h"
#include "u.h"
// #include "vec.h"

#define _typeof(t) __builtin_classify_type(t)

int main(int argc, const char** argv) {

  map(int, char) mm = nullptr;

  mm = map_new(int, char, fn_eq_use(int32));

  map_push(mm, 12, 'c');
  map_push(mm, 12, 'b');
  map_push(mm, 13, '1');
  map_push(mm, 14, 'i');
  map_push(mm, 15, 'a');
  map_push(mm, 16, 'b');
  map_push(mm, 17, '[');
  map_push(mm, 18, '.');

  map_for(mm) {
    inf("key(%d), val('%c')", mm->key, mm->val);
  }

  inf("%c", map_at(mm, 18));
  inf("%c", map_pop(mm, 18));

  map_for(mm) {
    inf("key(%d), val('%c')", mm->key, mm->val);
  }

  return 0;
}
