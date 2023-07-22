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

  str_t str = str_new('c');

  inf("'%s'", str->c_str);

  auto c = 'c';

  inf("sizeof(%lu)", sizeof(c));
  inf_hex((uint8_t*)__str_char_start(&c), 4l);

  return 0;
}
