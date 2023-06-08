/* libs */
#include "src/str.h"
#include "src/u.h"

#include <assert.h>
#include <string.h>

int main(int argc, const char** argv) {

  str_t str = str_new("hello world");

  assert(4 == str_find(&str, "o"));
  assert(7 == str_find(&str, "o", 5));

  return 0;
}
