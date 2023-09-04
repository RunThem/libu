#include "stack.h"

/*************************************************************************************************
 * Private function
 *************************************************************************************************/
static size_t __stack_expansion(size_t size) {
  if (size < 16) {
    return 16;
  } else if (size < 1024) {
    return size * 2;
  }

  return size + 512;
}

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __stack_new(size_t itsize, size_t cap) {
  stack_t* self = nullptr;

  u_ret_if(itsize == 0, nullptr);

  self = u_talloc(sizeof(stack_t) + itsize, stack_t*);
  u_mem_if(self);

  self->items = u_zalloc(itsize * cap);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->cap    = cap;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

ret_t __stack_init(any_t* _self, size_t itsize, size_t cap) {
  u_ret_if(_self == nullptr, -1);
  u_ret_if(*_self != nullptr, -1);

  *_self = __stack_new(itsize, cap);
  u_mem_if(*_self);

  return 0;

err:
  return -2;
}

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
ret_t __stack_resize(any_t _self, size_t cap) {
  any_t ptr     = nullptr;
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->cap >= cap, -1);

  ptr = u_realloc(self->items, self->itsize * cap);
  u_mem_if(ptr);

  self->items = ptr;
  self->cap   = cap;

  return 0;

err:
  return -2;
}

ret_t __stack_clear(any_t _self) {
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);

  self->len = 0;
  self->cap = 0;

  return 0;
}

ret_t __stack_cleanup(any_t _self) {
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);

  self->itsize = 0;
  self->len    = 0;
  self->cap    = 0;

  u_free_if(self->items);

  return 0;
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
inline size_t __stack_itsize(any_t _self) {
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);

  return self->itsize;
}

inline size_t __stack_len(any_t _self) {
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, 0);

  return self->len;
}

inline size_t __stack_cap(any_t _self) {
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, 0);

  return self->len;
}

inline bool __stack_empty(any_t _self) {
  return __stack_len(_self) == 0;
}

ret_t __stack_push(any_t _self, any_t it) {
  ret_t code    = 0;
  stack_t* self = as(_self, stack_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  if (self->len == self->cap) {
    code = __stack_resize(_self, __stack_expansion(self->cap));
    u_err_if(code != 0);
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
