#include <mut.h>
#include <u/core/vec.h>

mut_test(create) {
  u_vec_t(int) v = u_vec_new(int);

  mut_assert(v == nullptr);
}

mut_group(vec) {
  mut_add_test(create, "create vec instance.");
}
