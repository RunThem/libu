#include <u/core/avl.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/core/vec.h>
#include <u/u.h>

#define N 1000

int main(int argc, const char** argv) {

  u_vec_t(int) v = u_vec_new(int);

  for (size_t i = 0; i < N; i++) {
    u_vec_push_back(v, i);
  }

  for (size_t i = 0; i < N; i++) {
    u_vec_push_front(v, i);
  }

  for (size_t i = 0; i < N; i++) {
    u_vec_push(v, N, i);
  }

  u_vec_cleanup(v);

  return 0;
}
