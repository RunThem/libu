/* libs */
#include "src/str.h"
#include "src/u.h"

#include <assert.h>
#include <string.h>

int main(int argc, const char** argv) {

  str_t str = str_new("hello");

  str_cleanup(&str);

  assert(nullptr == str);

  return 0;
}
