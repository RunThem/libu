#include "buf.h"

#define _buf (*buf)

static size_t buf_cap(size_t len) {
  if (len < 32) {
    return 32;
  } else if (len < 1024) {
    return len * 2;
  }

  return len + 1024;
}

buf_t buf_new(c_buf c_buffer, size_t len) {
  size_t cap = 0;
  buf_t buf  = nullptr;

  u_ret_if(c_buffer == nullptr, nullptr);
  u_ret_if(len == 0, nullptr);

  cap = buf_cap(len);

  buf = u_talloc(sizeof(struct __buf) + 1 + cap, buf_t);
  u_alloc_if(buf);

  buf->len = len;
  buf->cap = cap;
  memcpy(buf->c_buf, c_buffer, len);

  return buf;

err:
  return nullptr;
}

int buf_init(buf_t* buf, size_t len) {
  size_t cap = 0;

  u_ret_if(buf == nullptr, -1);

  cap = buf_cap(len);

  _buf = u_talloc(sizeof(struct __buf) + 1 + cap, buf_t);
  u_alloc_if(_buf);

  _buf->cap = cap;

  return 0;

err:
  return -1;
}
