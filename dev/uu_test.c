
#define UU_DEBUG

#include "uu.h"

#include <assert.h>
#include <stdio.h>

int cmp_fn_int(const void* x, const void* y) {
  int a = *(int*)x;
  int b = *(int*)y;

  return a - b;
}

void dump_fn_int(const void* _key, const void* _uptr) {
  int key         = *(int*)_key;
  const int* uptr = _uptr;

  printf("[%d] = 0x%x", key, (int)(intptr_t)uptr);
}

int main() {
  {
    uu_dict(int, int*) d = uu_dict_init(d, cmp_fn_int);

    for (int i = 0; i < 100; i++) {
      uu_dict_insert(d, i, (void*)(intptr_t)i);
    }

    for (int i = 70; i < 100; i++) {
      uu_dict_remove(d, i);
    }

    uu_dict_dump(d, dump_fn_int);
  }

  printf("end\n");

  return 0;
}