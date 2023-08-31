#include "buf.h"

/*************************************************************************************************
 * Private function
 *************************************************************************************************/
static size_t __buf_expansion(size_t size) {
  if (size < 32) {
    return 32;
  } else if (size < 1024) {
    return size * 2;
  }

  return size + 512;
}

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __buf_new(size_t cap) {
  buf_t self = nullptr;

  self = u_talloc(sizeof(struct buf_t), buf_t);
  u_mem_if(self);

  self->c_buf = u_zalloc(cap);
  u_mem_if(self->c_buf);

  self->cap = cap;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

ret_t __buf_init(any_t* _self, size_t cap) {
  buf_t self = as(_self, buf_t);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(*_self != nullptr, -1);

  *_self = __buf_new(cap);
  u_mem_if(*_self);

  return 0;

err:
  return -2;
}

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
ret_t __buf_resize(any_t _self, size_t cap) {
  uint8_t* ptr = nullptr;
  buf_t self   = as(_self, buf_t);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->cap >= cap, -2);

  ptr = u_realloc(self->c_buf, sizeof(uint8_t) * cap);
  u_mem_if(ptr);

  self->c_buf = ptr;
  self->cap   = cap;

  return 0;

err:
  return -2;
}

inline void __buf_clear(any_t _self) {
  buf_t self = as(_self, buf_t);

  u_noret_if(_self == nullptr);

  self->len = 0;
}

ret_t __buf_cleanup(any_t _self) {
  buf_t self = as(_self, buf_t);

  u_ret_if(_self == nullptr, -1);

  u_free(self->c_buf);
  u_free(self);

  return 0;
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
inline size_t __buf_len(any_t _self) {
  buf_t self = as(_self, buf_t);

  u_ret_if(_self == nullptr, 0);

  return self->len;
}

inline size_t __buf_cap(any_t _self) {
  buf_t self = as(_self, buf_t);

  u_ret_if(_self == nullptr, 0);

  return self->len;
}

inline bool __buf_empty(any_t _self) {
  return __buf_len(_self) != 0;
}

ret_t __buf_push(any_t _self, any_t mem, size_t len) {
  ret_t code = 0;
  buf_t self = as(_self, buf_t);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(mem == nullptr, -1);

  u_ret_if(self->c_buf == nullptr, -1);

  if (self->cap - self->len < len) {
    code = __buf_resize(self, __buf_expansion(self->cap));
    u_err_if(code != 0);
  }

  memcpy(self->c_buf + self->len, mem, len);

  self->len += len;

  return 0;

err:
  return -2;
}

ret_t __buf_pop(any_t _self, any_t mem, size_t len) {
  buf_t self = as(_self, buf_t);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(mem == nullptr, -1);

  u_ret_if(self->c_buf == nullptr, -1);
  u_ret_if(self->len < len, -1);

  memcpy(mem, self->c_buf + self->len - len, len);

  self->len -= len;

  return 0;
}

ret_t __buf_peek(any_t _self, any_t mem, size_t len) {
  buf_t self = as(_self, buf_t);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(mem == nullptr, -1);

  u_ret_if(self->c_buf == nullptr, -1);
  u_ret_if(self->len < len, -1);

  memcpy(mem, self->c_buf + self->len - len, len);

  return 0;
}
