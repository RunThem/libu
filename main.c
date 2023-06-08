/* libs */
#include "src/u.h"

#include <stdio.h>

int main(int argc, const char** argv) {

  inf("min is %d", min_from(1, 3));
  inf("max is %d", max_from(1, 3));

  inf("min is %d", min_from(342, 3, 1, 34, 2, 35));
  inf("max is %d", max_from(342, 3, 1, 34, 2, 35));

  return 0;
}
