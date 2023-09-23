#include <u/core/avl.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/core/vec.h>
#include <u/u.h>

int main(int argc, const char** argv) {

  u_vec_t(int) v = u_vec_new(int);

  each(i, N10W) {
    u_vec_push_back(v, i);
  }

  for (size_t i = 0; i < 10'0000; i++) {
    u_vec_push_front(v, i);
  }

  for (size_t i = 0; i < 10'0000; i++) {
    u_vec_push(v, 10'0000, i);
  }

  return 0;
}
