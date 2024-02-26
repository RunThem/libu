#include <mut.h>

int main(int argc, const char** argv) {
  mut_init("libu");

  mut_add_group(vec, "Generic vec container.");
  mut_add_group(tbl, "Generic tbl container.");
  mut_add_group(avl, "Generic avl container.");

  mut_results();

  return 0;
}
