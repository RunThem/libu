/* libs */
#include "src/str.h"
#include "src/u.h"
#include "src/buf.h"

int main(int argc, const char** argv) {

  buf_t buf = nullptr;

  int a[] = {0x01, 0x05, 0x03};

  buf = buf_new(p(a), 10);

  inf_hex(buf->c_buf, buf->len);

  return 0;
}
