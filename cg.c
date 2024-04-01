#include "cg.h"

int main() {

  /* #[[vec<int>]] */
  uvec(int) int_vec = nullptr;

  struct str {
    size_t len;
    size_t cap;
    char* buf;
  };

  uvec(struct str) str_vec = nullptr;

  uvec(uvec(int)) int_vec_vec = nullptr;

  {
    uvec(int) int_vec = nullptr;

    uvec(struct str) str_vec = nullptr;

    uvec(uvec(int)) int_vec_vec = nullptr;

    uvec(bool) bool_vec = nullptr;
  }

  uavl(int, char) int_char_avl = nullptr;

  int _uvec_ = 1;

  auto long_vec = uv_new(long);

  /* clang-format off */
uvec(char) char_vec = nullptr;

  uvec  (bool) bool_vec = nullptr;

  uvec  ( char) vec1 = nullptr;

  uvec  ( struct   str ) vec2 = nullptr;


  uvec(uvec(int)) vec3 = nullptr;

  return 0;
}
