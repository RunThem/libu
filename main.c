/* libs */
// #include "src/buf.h"
// #include "src/fs.h"
// #include "src/list.h"
// #include "src/map.h"
// #include "src/sock.h"
// #include "src/str.h"
#include "src/u.h"
// #include "src/vec.h"

#define _typeof(t) __builtin_classify_type(t)

int main(int argc, const char** argv) {

  int arr[] = {1, 3, 4, 12, 5, 2, 43, 523, 5, 23, 52, 34, 2, 5, 23};

  inf_hex(arr, sizeof(arr));

  return 0;
}
