/* libs */
#include "src/str.h"
#include "src/u.h"

#include <assert.h>
#include <string.h>

int main(int argc, const char** argv) {

  str_t str_1 = str_new("hello");

  assert(5 == str_1->len);

  str_t str_2 = str_new("hello world", 5);

  assert(5 == str_2->len);
  assert(!strncmp(str_1->c_str, str_2->c_str, 5));

  str_t str_3 = str_new_f("hell%c", 'o');

  assert(5 == str_3->len);
  assert(!strncmp(str_1->c_str, str_3->c_str, 5));

  return 0;
}
