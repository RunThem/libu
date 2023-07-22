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

  str_t str = str_new('c');

  inf("'%s'", str->c_str);

  auto c = 'c';

  inf("sizeof(%lu)", sizeof(c));
  inf_hex((uint8_t*)__str_char_start(&c), 4l);

  return 0;
}
