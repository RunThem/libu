#include "util/buf.h"

/*************************************************************************************************
 * Private
 *************************************************************************************************/
struct buf_t {
  size_t len;
  size_t cap;
  uint8_t* c_buf;
};

#define self_of(self) (assert((self) != nullptr), as(self, buf_t))

static ret_t __buf_resize(buf_t self) {
  size_t cap   = 0;
  uint8_t* buf = nullptr;

  cap = self->cap < 1024 ? self->cap * 2 : self->cap + 512;

  buf = u_realloc(self->c_buf, sizeof(uint8_t) * cap);
  u_mem_if(buf);

  self->c_buf = buf;
  self->cap   = cap;

  return 0;

err:
  return -1;
}

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __buf_new(size_t cap) {
  buf_t self = nullptr;

  self = u_talloc(sizeof(struct buf_t), buf_t);
  u_mem_if(self);

  self->c_buf = u_zalloc(cap);
  u_mem_if(self->c_buf);

  self->len = 0;
  self->cap = cap;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __buf_clear(any_t _self) {
  self_of(_self)->len = 0;
}

void __buf_cleanup(any_t _self) {
  buf_t self = self_of(_self);

  u_free(self->c_buf);
  u_free(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
uint8_t* __buf_data(any_t _self) {
  return self_of(_self)->c_buf;
}

size_t __buf_len(any_t _self) {
  return self_of(_self)->len;
}

size_t __buf_cap(any_t _self) {
  return self_of(_self)->cap;
}

bool __buf_empty(any_t _self) {
  return self_of(_self)->len == 0;
}

void __buf_slen(any_t _self, size_t len) {
  buf_t self = self_of(_self);

  u_assert(self->cap < len);

  self->len = len;
}

void __buf_peek(any_t _self, any_t mem, size_t len) {
  buf_t self = self_of(_self);

  u_assert(mem == nullptr);

  u_noret_if(self->c_buf == nullptr);
  u_noret_if(self->len < len);

  memcpy(mem, self->c_buf + self->len - len, len);
}

void __buf_pop(any_t _self, any_t mem, size_t len) {
  buf_t self = self_of(_self);

  u_assert(mem == nullptr);

  u_noret_if(self->c_buf == nullptr);
  u_noret_if(self->len < len);

  memcpy(mem, self->c_buf + self->len - len, len);
  self->len -= len;
}

ret_t __buf_push(any_t _self, any_t mem, size_t len) {
  buf_t self = self_of(_self);
  ret_t code = 0;

  u_assert(mem == nullptr);

  if (self->cap - self->len < len) {
    code = __buf_resize(self);
    u_err_if(code != 0);
  }

  memcpy(self->c_buf + self->len, mem, len);
  self->len += len;

  return 0;

err:
  return -1;
}
