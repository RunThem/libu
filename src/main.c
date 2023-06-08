/* libs */
#include "u.h"

int main(int argc, const char** argv) {

  va_cut(0);
  va_cut(0, 1);
  va_cut(0, 1, 2);

  va_cut(1);
  va_cut(1, 1);
  va_cut(1, 1, 2);
  va_cut(1, 1, 2, 3);

  va_cut(2);
  va_cut(2, 1);
  va_cut(2, 1, 2);
  va_cut(2, 1, 2, 3);
  va_cut(2, 1, 2, 3, 4);
  va_cut(2, 1, 2, 3, 4, 5);

  va_cut(3, );
  va_cut(3, 1);
  va_cut(3, 1, 2);
  va_cut(3, 1, 2, 3);
  va_cut(3, 1, 2, 3, 4);
  va_cut(3, 1, 2, 3, 4, 5);

  va_cut(4, );
  va_cut(4, 1);
  va_cut(4, 1, 2);
  va_cut(4, 1, 2, 3);
  va_cut(4, 1, 2, 3, 4);
  va_cut(4, 1, 2, 3, 4, 5);

  va_cut(5, );
  va_cut(5, 1);
  va_cut(5, 1, 2);
  va_cut(5, 1, 2, 3);
  va_cut(5, 1, 2, 3, 4);
  va_cut(5, 1, 2, 3, 4, 5);
  va_cut(5, 1, 2, 3, 4, 5, 6);
  va_cut(5, 1, 2, 3, 4, 5, 6, 7);

  va_at(0);
  va_at(0, 1);
  va_at(0, 1, 2);
  va_at(0, 1, 2, 3);
  va_at(0, 1, 2, 3, 4);
  va_at(0, 1, 2, 3, 4, 5);

  va_at(1);
  va_at(1, 1);
  va_at(1, 1, 2);
  va_at(1, 1, 2, 3);
  va_at(1, 1, 2, 3, 4);
  va_at(1, 1, 2, 3, 4, 5);

  va_at(2);
  va_at(2, 1);
  va_at(2, 1, 2);
  va_at(2, 1, 2, 3);
  va_at(2, 1, 2, 3, 4);
  va_at(2, 1, 2, 3, 4, 5);

  va_at(3, );
  va_at(3, 1);
  va_at(3, 1, 2);
  va_at(3, 1, 2, 3);
  va_at(3, 1, 2, 3, 4);
  va_at(3, 1, 2, 3, 4, 5);

  va_at(4);
  va_at(4, 1);
  va_at(4, 1, 2);
  va_at(4, 1, 2, 3);
  va_at(4, 1, 2, 3, 4);
  va_at(4, 1, 2, 3, 4, 5);

  va_at(5);
  va_at(5, 1);
  va_at(5, 1, 2);
  va_at(5, 1, 2, 3);
  va_at(5, 1, 2, 3, 4);
  va_at(5, 1, 2, 3, 4, 5);
  va_at(5, 1, 2, 3, 4, 5, 6);

  va_size();
  va_size(0);
  va_size(0, 1);
  va_size(0, 1, 2);
  va_size(0, 1, 2, 3);
  va_size(0, 1, 2, 3, 4);
  va_size(0, 1, 2, 3, 4, 5);

  va_empty();
  va_empty(0);
  va_empty(0, 1);

  return 0;
}
