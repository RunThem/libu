/* libs */
#include "src/u.h"

int main(int argc, const char** argv) {

  void* buf = u_malloc(100);

  u_free(buf);

  return 0;
}
