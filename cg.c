
// #define uvec_Type(T)                                                                               \
//   uvec(T) : (T) {                                                                                  \
//   }
// #define uT(u) _Generic(u, uvec_Type(int))

#define uT(u)                                                                                      \
  _Generic(typeof(u),                                                                              \
      uvec(int): (int){},                                                                          \
      uvec(struct str): (struct str){},                                                            \
      uvec(uvec(int)): (uvec(int)){})

#include "cg.h"

struct str {
  size_t len;
  size_t cap;
  char* buf;
};

int main() {

  /* #[[vec<int, struct str>]] */
  uvec(int) int_vec                  = nullptr;
  uvec(struct str) str_vec           = nullptr;
  uvec(uvec(int)) int_vec_vec        = nullptr;
  uvec(struct { int a; }) struct_vec = nullptr;

  int _uvec_ = 1;

  auto sd = uT(int_vec);

  sd = 2;

  return 0;
}
