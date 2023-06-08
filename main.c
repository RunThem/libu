/* libs */
#include "src/buf.h"
#include "src/list.h"
#include "src/map.h"
#include "src/str.h"
#include "src/u.h"
#include "src/vec.h"

#include <stdio.h>

int main(int argc, const char** argv) {

  list(int) lst = nullptr;

  list_cleanup(&lst);

  return 0;
}
