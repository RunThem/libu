/* libs */
#include "src/u.h"

#include <stdio.h>

int main(int argc, const char** argv) {

  u_if(1 != 1);
  u_if(1 == 1);

  u_if(1 == 1, "1 == 1");

  u_if(1 == 1, "1 == %d", 1);

  u_goto_if(1 == 1);

err:

  u_goto_if(1 == 1, loop);

loop:

  u_goto_if(1 == 1, loop_1, "goto loop_1");

loop_1:

  return 0;
}
