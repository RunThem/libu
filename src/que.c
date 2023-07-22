#include "que.h"

static size_t __que_cap(size_t size) {
  if (size < 16) {
    return 16;
  } else if (size < 1024) {
    return size * 2;
  }

  return size + 512;
}

ret_t __que_init(any_t _self, size_t itsize, size_t cap) {
  que_t* self = as(_self, que_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(itsize == 0, -1);

  self->itsize = itsize;
  self->len    = 0;
  self->cap    = __que_cap(cap);

  self->s_idx = 0;
  self->e_idx = 0;

  self->items = u_calloc(cap, itsize);
  u_alloc_if(self->items);

  return 0;

err:
  bzero(self, sizeof(que_t));
  return -2;
}

ret_t __que_resize(any_t _self, size_t cap) {
  any_t ptr   = nullptr;
  que_t* self = as(_self, que_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->cap >= cap, -1);

  ptr = u_realloc(self->items, self->itsize * cap);
  u_alloc_if(ptr);

  self->items = ptr;
  self->cap   = cap;

  return 0;

err:
  return -2;
}

ret_t __que_clear(any_t _self) {
  que_t* self = as(_self, que_t*);

  u_ret_if(_self == nullptr, -1);

  self->len   = 0;
  self->s_idx = 0;
  self->e_idx = 0;

  return 0;
}

ret_t __que_cleanup(any_t _self) {
  que_t* self = as(_self, que_t*);

  u_ret_if(_self == nullptr, -1);

  self->itsize = 0;
  self->len    = 0;
  self->cap    = 0;
  self->s_idx  = 0;
  self->e_idx  = 0;

  u_free_if(self->items);

  return 0;
}

ret_t __que_push(any_t _self, any_t it) {
  ret_t code  = 0;
  que_t* self = as(_self, que_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  if (self->e_idx == self->cap) {
    if (self->s_idx == 0) {
      code = __que_resize(_self, __que_cap(self->cap));
      u_goto_if(code != 0);
    } else {
      memmove(self->items, self->items + self->s_idx * self->itsize, self->len * self->itsize);
      self->e_idx -= self->s_idx;
      self->s_idx = 0;
    }
  }

  memcpy(self->items + self->e_idx * self->itsize, it, self->itsize);
  self->e_idx++;
  self->len++;

  return 0;

err:
  return -2;
}

ret_t __que_pop(any_t _self, any_t it) {
  que_t* self = as(_self, que_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->len == 0, -1);

  if (self->s_idx >= self->cap / 2) {
    memmove(self->items, self->items + self->s_idx * self->itsize, self->len * self->itsize);
    self->e_idx -= self->s_idx;
    self->s_idx = 0;
  }

  memcpy(it, self->items + self->s_idx * self->itsize, self->itsize);
  self->s_idx++;
  self->len--;

  return 0;
}

ret_t __que_peek(any_t _self, any_t it) {
  que_t* self = as(_self, que_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->len == 0, -1);

  memcpy(it, self->items + self->s_idx * self->itsize, self->itsize);

  return 0;
}
