#include <mut.h>

int main(int argc, const char** argv) {
  mut_init("libu");

  mut_add_group(vec, "Generic vector container.");
  mut_add_group(map, "Generic map container.");

  mut_results();

  return 0;
}
