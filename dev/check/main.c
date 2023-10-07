#include <mut.h>

mut_extern(vec);

int main(int argc, const char** argv) {
  mut_init("libu");

  mut_add_group(vec, "generic vec unit test.");

  mut_results();

  return 0;
}
