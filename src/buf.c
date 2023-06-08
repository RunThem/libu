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

bool buf_empty(buf_t* buf) {
  u_ret_if(buf == nullptr, -1);
  u_ret_if(_buf == nullptr, -1);

  return _buf->len == 0;
}

int buf_resize(buf_t* buf, size_t len) {
  u_ret_if(buf == nullptr, -1);
  u_ret_if(_buf == nullptr, -1);
  u_ret_if(len <= _buf->cap, -1);

  _buf = (buf_t)u_realloc(_buf, len);
  u_alloc_if(_buf);

  _buf->cap = len;

  return 0;

err:
  return -1;
}

void buf_clear(buf_t* buf) {
  u_ret_no_if(buf == nullptr);
  u_ret_no_if(_buf == nullptr);

  _buf->len      = 0;
  _buf->c_buf[0] = '\0';
}

void buf_cleanup(buf_t* buf) {
  u_ret_no_if(buf == nullptr);
  u_ret_no_if(_buf == nullptr);

  u_free(_buf);
  _buf = nullptr;
}

int __buf_push(buf_t* buf, c_buf c_buffer, size_t size) {
  int ret = 0;

  u_ret_if(buf == nullptr, -1);
  u_ret_if(_buf == nullptr, -1);
  u_ret_if(c_buffer == nullptr, -1);

  if (size > _buf->cap - _buf->len) {
    ret = buf_resize(buf, buf_cap(_buf->len) + size);
    u_goto_if(ret != 0);
  }

  memcpy(&_buf->c_buf[_buf->len], c_buffer, size);
  _buf->len += size;

  return 0;

err:
  return -1;
}

int __buf_pop(buf_t* buf, c_buf c_buffer, size_t size) {
  u_ret_if(buf == nullptr, -1);
  u_ret_if(_buf == nullptr, -1);
  u_ret_if(c_buffer == nullptr, -1);
  u_ret_if(size == 0 || size > _buf->len, -1);

  memcpy(c_buffer, &_buf->c_buf[_buf->len - size], size);
  _buf->len -= size;

  return 0;
}

int __buf_peek(buf_t* buf, c_buf c_buffer, size_t size) {
  u_ret_if(buf == nullptr, -1);
  u_ret_if(_buf == nullptr, -1);
  u_ret_if(c_buffer == nullptr, -1);
  u_ret_if(size == 0 || size > _buf->len, -1);

  memcpy(c_buffer, &_buf->c_buf[_buf->len - size], size);

  return 0;
}
