/* libs */
#include "src/u.h"

#include <stdio.h>

int main(int argc, const char** argv) {

  int arr[] = {1, 2, 3, 4, 5, 6};

  inf("%ld", array_len(arr));

  int a = 0;
  int b = 100;

  swap(a, b);

  inf("%d, %d", a, b);

  inf("%d", align_of(9, 8));
  inf("%d", align_of(9, 4));

  return 0;
}
