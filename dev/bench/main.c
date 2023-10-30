#include <u/core/avl.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/u.h>

extern void vec(size_t);
extern void map(size_t);
extern void avl(size_t);

int main(int argc, const char** argv) {

  println("benchmark vec: {");
  vec(N10W);
  println("}\n");

  /* map */
  println("benchmark map: {");
  map(N1W);
  println();

  map(N10W);
  println();

  map(N100W);
  println();

  map(N1000W);
  println("}\n");

  /* avl tree */
  println("benchmark avl: {");
  avl(N1W);
  println();

  avl(N10W);
  println();

  avl(N100W);
  println();

  avl(N1000W);
  println("}");

  return 0;
}
