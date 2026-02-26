
#include "../../uu/ubench.h"
#include "u/u.h"

#include <assert.h>
#include <stdio.h>

void shuffle(int* array, int len) {
  srand(time(NULL));

  for (int i = len - 1; i > 0; i--) {
    int j = rand() % (i + 1);

    int temp = array[i];
    array[i] = array[j];
    array[j] = temp;
  }
}

int* make_array(int n) {
  int* array = (int*)malloc(n * sizeof(int));

  for (int i = 0; i < n; i++) {
    array[i] = i;
  }

  shuffle(array, n);

  return array;
}

UBENCH_EX(Dict, at) {
  u_dict_t(int, int) d = u_dict_new(d);

  size_t i   = 0;
  int* array = make_array(COUNT);

  for (i = 0; i < COUNT; i++) {
    u_dict_insert(d, array[i], array[i]);
  }

  UBENCH_DO_BENCHMARK() {
    UBENCH_DO_BENCHMARK_BLOCK({
      for (i = 0; i < COUNT; i++) {
        u_dict_at(d, array[i]);
      }
    });
  }

  UBENCH_DO_NOTHING(&i);

  free(array);

  u_dict_cleanup(d);
}

UBENCH_EX(Dict, insert) {
  u_dict_t(int, int) d = u_dict_new(d);

  size_t i   = 0;
  int* array = make_array(COUNT);

  UBENCH_DO_BENCHMARK() {
    UBENCH_DO_BENCHMARK_BLOCK({
      for (i = 0; i < COUNT; i++) {
        u_dict_insert(d, array[i], array[i]);
      }
    });

    for (i = 0; i < COUNT; i++) {
      u_dict_remove(d, array[i]);
    }
  }

  UBENCH_DO_NOTHING(&i);

  free(array);

  u_dict_cleanup(d);
}

UBENCH_EX(Dict, remove) {
  u_dict_t(int, int) d = u_dict_new(d);

  size_t i   = 0;
  int* array = make_array(COUNT);

  UBENCH_DO_BENCHMARK() {
    for (i = 0; i < COUNT; i++) {
      u_dict_insert(d, i, i);
    }

    shuffle(array, COUNT);

    UBENCH_DO_BENCHMARK_BLOCK({
      for (i = 0; i < COUNT; i++) {
        u_dict_remove(d, array[i]);
      }
    });
  }

  UBENCH_DO_NOTHING(&i);

  free(array);

  u_dict_cleanup(d);
}

UBENCH_MAIN();