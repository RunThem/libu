#include "chibicc.h"

pub char* format(char* fmt, ...) {
  char* buf     = {};
  size_t buflen = 0;
  FILE* out     = open_memstream(&buf, &buflen);

  va_list ap;
  va_start(ap);
  vfprintf(out, fmt, ap);
  va_end(ap);

  fclose(out);

  return buf;
}