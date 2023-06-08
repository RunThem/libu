/* libs */
#include "u.h"

int main(int argc, const char** argv) {

  va_slice(3);
  va_slice(3, 1);
  va_slice(3, 1, 2);
  va_slice(3, 1, 2, 3);
  va_slice(3, 1, 2, 3, 4);
  va_slice(3, 1, 2, 3, 4, 5);
  va_slice(3, 1, 2, 3, 4, 5, 6);
  va_slice(3, 1, 2, 3, 4, 5, 6, 7);

  return 0;
}
