#include "stack.h"
#include "u.h"

#include <stdio.h>
#include <sys/stat.h>

static size_t __stack_cap(size_t size) {
  if (size < 16) {
    return 16;
  } else if (size < 1024) {
    return size * 2;
  }

  return size + 512;
}

ret_t __stack_init(any_t _self, size_t itsize, size_t cap) {
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(itsize == 0, -1);

  self->itsize = itsize;
  self->len    = 0;
  self->cap    = __stack_cap(cap);

  self->items = u_calloc(self->cap, itsize);
  u_alloc_if(self->items);

  return 0;

err:
  bzero(self, sizeof(stack_t));
  return -2;
}

ret_t __stack_resize(any_t _self, size_t cap) {
  any_t ptr     = nullptr;
  stack_t* self = as(_self, stack_t*);

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

ret_t __stack_clear(any_t _self) {
  any_t ptr     = nullptr;
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);

  self->len = 0;
  self->cap = 0;

  return 0;
}

ret_t __stack_cleanup(any_t _self) {
  any_t ptr     = nullptr;
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);

  self->itsize = 0;
  self->len    = 0;
  self->cap    = 0;

  u_free_if(self->items);

  return 0;
}

ret_t __stack_push(any_t _self, any_t it) {
  ret_t code    = 0;
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  if (self->len == self->cap) {
    code = __stack_resize(_self, __stack_cap(self->cap));
    u_goto_if(code != 0);
  }

  memcpy(self->items + self->len * self->itsize, it, self->itsize);
  self->len++;

  return 0;

err:
  return -2;
}

ret_t __stack_pop(any_t _self, any_t it) {
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->len == 0, -1);

  memcpy(it, self->items + (self->len - 1) * self->itsize, self->itsize);
  self->len--;

  return 0;
}

ret_t __stack_peek(any_t _self, any_t it) {
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->len == 0, -1);

  memcpy(it, self->items + (self->len - 1) * self->itsize, self->itsize);

  return 0;
}
