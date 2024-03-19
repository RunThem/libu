#include "cg.h"

int main() {
  kvec(int) int_vec = nullptr;

  struct str {
    size_t len;
    size_t cap;
    char* buf;
  };

  kvec(struct str) str_vec = nullptr;

  kvec(kvec(int)) int_vec_vec = nullptr;

  return 0;
}
