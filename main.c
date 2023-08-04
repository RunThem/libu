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

  println("Hello libu!");

  return 0;
}
