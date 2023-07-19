/* libs */
// #include "src/buf.h"
// #include "src/fs.h"
// #include "src/list.h"
// #include "src/map.h"
// #include "src/sock.h"
// #include "src/str.h"
#include "src/u.h"
#include "vec.h"

#include <stdlib.h>
// #include "vec.h"
// #include "src/vec.h"

#define _typeof(t) __builtin_classify_type(t)

// void print(void* ptr) {
//   inf("%d", *as(ptr, int*));
// }

#include <stddef.h>
#include <stdint.h>

int main(int argc, const char** argv) {

  vec(int) vv = {0};
  int arr[]   = {1, 2,  3, 4, 2, 3, 13, 4,  2,  345, 2, 34, 2,  342, 34, 2,   2, 2, 2, 2,
                 3, 3,  4, 4, 5, 4, 3,  3,  23, 2,   3, 3,  4,  3,   32, 2,   2, 3, 3, 4,
                 3, 23, 2, 3, 3, 4, 3,  23, 2,  3,   3, 3,  32, 2,   21, 232, 3, 3, 3, 3};

  vec_init(&vv, sizeof(int), arr_len(arr));

  arr_for(arr, i) {
    vec_push_b(&vv, arr[i]);
  }

  vec_for(&vv, i) {
    inf("%d", vec_at(&vv, i));
  }

  inf("max %ld", vec_max(&vv, fn_cmp_use(int)));

  vec_sort(&vv, fn_cmp_use(int));

  vec_for(&vv, i) {
    inf("%d", vec_at(&vv, i));
  }

  return 0;
}
