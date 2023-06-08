/* libs */
#include "src/u.h"

#include <stdio.h>

/*
 * a > b  ->  1
 * a == b ->  0
 * a < b  -> -1
 * */
int comp_int(const int a, const int b) {
  if (a > b)
    return 1;
  return a == b ? 0 : -1;
}

int main(int argc, const char** argv) {
  int arr[] = {-21, -4, 23, 54, 12, -3, 23, 0, -42};

  inf("min idx is %ld", min_idx(arr, arrlen(arr)));
  inf("max idx is %ld", max_idx(arr, arrlen(arr)));

  inf("min idx is %ld", min_idx(arr, arrlen(arr), comp_int));
  inf("max idx is %ld", max_idx(arr, arrlen(arr), comp_int));

  inf("min is %d", min_from(arr, arrlen(arr)));
  inf("max is %d", max_from(arr, arrlen(arr)));

  inf("min is %d", min_from(arr, arrlen(arr), comp_int));
  inf("max is %d", max_from(arr, arrlen(arr), comp_int));

  return 0;
}
