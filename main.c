/* libs */
#include "src/buf.h"
#include "src/fs.h"
#include "src/list.h"
#include "src/map.h"
#include "src/sock.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#define _typeof(t) __builtin_classify_type(t)

#define me(x, ...) (&(struct { typeof(x) _; }){(x)})

void print(void* ptr) {
  inf("%d\n", *(int*)ptr);
}

int main(int argc, const char** argv) {

  print(me(10));

  return 0;
}
