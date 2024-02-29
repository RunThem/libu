#include <u/u.h>

extern void vec(size_t);
extern void tbl(size_t);
extern void avl(size_t);

int main(int argc, const char** argv) {

  println("benchmark vec: {");
  vec(N10W);
  println("}\n");

  /* tbl */
  println("benchmark tbl: {");
  tbl(N1W);
  println();

  tbl(N10W);
  println();

  tbl(N100W);
  println();

  tbl(N1000W);
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
  println();

  avl(N1000W * 10);
  println("}");

  return 0;
}
