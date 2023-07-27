#include "buf.h"

static size_t __buf_cap(size_t size) {
  if (size < 32) {
    return 32;
  } else if (size < 1024) {
    return size * 2;
  }

  return size + 512;
}

ret_t __buf_init(any_t _self, size_t cap) {
  buf_t* self = as(_self, buf_t*);

  u_ret_if(_self == nullptr, -1);

  self->len = 0;
  self->cap = __buf_cap(cap);

  self->c_buf = u_calloc(self->cap, sizeof(uint8_t));
  u_alloc_if(self->c_buf);

  return 0;

err:
  bzero(self, sizeof(buf_t));
  return -2;
}

ret_t __buf_resize(any_t _self, size_t cap) {
  uint8_t* ptr = nullptr;
  buf_t* self  = as(_self, buf_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->cap >= cap, -2);

  ptr = u_realloc(self->c_buf, sizeof(uint8_t) * cap);
  u_alloc_if(ptr);

  self->c_buf = ptr;
  self->cap   = cap;

  return 0;

err:
  bzero(self, sizeof(buf_t));
  return -2;
}

ret_t __buf_cleanup(any_t _self) {
  buf_t* self = as(_self, buf_t*);

  u_ret_if(_self == nullptr, -1);

  u_free(self->c_buf);

  bzero(self, sizeof(buf_t));

  return 0;
}

ret_t __buf_push(any_t _self, any_t mem, size_t len) {
  ret_t code  = 0;
  buf_t* self = as(_self, buf_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(mem == nullptr, -1);

  u_ret_if(self->c_buf == nullptr, -1);

  if (self->cap - self->len < len) {
    code = __buf_resize(self, __buf_cap(self->cap));
    u_goto_if(code != 0);
  }

  memcpy(self->c_buf + self->len, mem, len);

  self->len += len;

  return 0;

err:
  return -2;
}

ret_t __buf_pop(any_t _self, any_t mem, size_t len) {
  buf_t* self = as(_self, buf_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(mem == nullptr, -1);

  u_ret_if(self->c_buf == nullptr, -1);
  u_ret_if(self->len < len, -1);

  memcpy(mem, self->c_buf + self->len - len, len);

  self->len -= len;

  return 0;
}

ret_t __buf_peek(any_t _self, any_t mem, size_t len) {
  buf_t* self = as(_self, buf_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(mem == nullptr, -1);

  u_ret_if(self->c_buf == nullptr, -1);
  u_ret_if(self->len < len, -1);

  memcpy(mem, self->c_buf + self->len - len, len);

  return 0;
}
