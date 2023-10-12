#include <u/core/avl.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/core/vec.h>
#include <u/u.h>

void bench_vec(size_t N) {
  u_vec_t(int) v = u_vec_new(int);

  /* push */
  each(i, N) {
    u_vec_push_back(v, i);
  }

  each(i, N) {
    u_vec_push_front(v, i);
  }

  each(i, N) {
    u_vec_push(v, N, i);
  }

  /* at */
  each(i, 3 * N) {
    u_vec_at_back(v);
  }

  each(i, 3 * N) {
    u_vec_at_front(v);
  }

  each(i, 3 * N) {
    u_vec_at(v, i);
  }

  /* pop */
  each(i, N) {
    u_vec_pop_back(v);
  }

  each(i, N) {
    u_vec_pop_front(v);
  }

  each(i, N) {
    u_vec_pop(v, 0);
  }

  u_vec_cleanup(v);
}

int main(int argc, const char** argv) {

  bench_vec(N1W);

  return 0;
}
