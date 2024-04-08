#include <mut.h>
#include <u/u.h>

int main(int argc, const char* argv[]) {
  mut_init("libu");

  mut_add_group(vec, "Generic vec container.");
  mut_add_group(map, "Generic map container.");
  mut_add_group(avl, "Generic avl container.");
  mut_add_group(lst, "Generic lst container.");

  mut_results();

  return 0;
}
