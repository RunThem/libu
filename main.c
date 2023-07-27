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
  buf_t buf = {0};

  buf_init(&buf, 10);

  int a = 124823;
  buf_push(&buf, 10);

  buf_push(&buf, &a, 4);

  inf_hex(buf_data(&buf), buf_len(&buf));
  buf_hex(&buf);

  int b = 0;
  buf_pop(&buf, &b, 2);

  inf("%d", b);

  buf_hex(&buf);

  buf_peek(&buf, &b);

  inf("%d", b);

  buf_hex(&buf);

  buf_cleanup(&buf);

  return 0;
}
