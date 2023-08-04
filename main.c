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

  map_push(&mm, 12, 'a');
  map_push(&mm, 2, '4');
  map_push(&mm, 14, 'v');
  map_push(&mm, 132, 'm');

  printf("len is %ld\n", map_len(&mm));
  printf("'%c'\n", map_at(&mm, 12));
  printf("'%c'\n", map_at(&mm, 2));
  printf("'%c'\n", map_at(&mm, 14));
  printf("'%c'\n", map_at(&mm, 132));

  map_for(&mm, i) {
    printf("%d, %c\n", mm.key, mm.val);
  }

  return 0;
}
