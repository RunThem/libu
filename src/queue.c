#include "queue.h"

/*************************************************************************************************
 * Private function
 *************************************************************************************************/
static size_t __queue_expansion(size_t size) {
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
any_t __queue_new(size_t itsize, size_t cap) {
  queue_t* self = nullptr;

  u_ret_if(itsize == 0, nullptr);

  self = u_talloc(sizeof(queue_t) + itsize, queue_t*);
  u_alloc_if(self);

  self->items = u_zalloc(itsize * cap);
  u_alloc_if(self->items);

  self->itsize = itsize;
  self->len    = 0;
  self->cap    = cap;

  self->s_idx = 0;
  self->e_idx = 0;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

ret_t __queue_init(any_t* _self, size_t itsize, size_t cap) {
  u_ret_if(_self == nullptr, -1);
  u_ret_if(*_self != nullptr, -1);

  *_self = __queue_new(itsize, cap);
  u_alloc_if(*_self);

  return 0;

err:
  return -2;
}

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
ret_t __queue_resize(any_t _self, size_t cap) {
  any_t ptr     = nullptr;
  queue_t* self = as(_self, queue_t*);

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

ret_t __queue_clear(any_t _self) {
  queue_t* self = as(_self, queue_t*);

  u_ret_if(_self == nullptr, -1);

  self->len   = 0;
  self->s_idx = 0;
  self->e_idx = 0;

  return 0;
}

ret_t __queue_cleanup(any_t _self) {
  queue_t* self = as(_self, queue_t*);

  u_ret_if(_self == nullptr, -1);

  self->itsize = 0;
  self->len    = 0;
  self->cap    = 0;
  self->s_idx  = 0;
  self->e_idx  = 0;

  u_free_if(self->items);

  return 0;
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
inline size_t __queue_itsize(any_t _self) {
  queue_t* self = as(_self, queue_t*);

  u_ret_if(_self == nullptr, -1);

  return self->itsize;
}

inline size_t __queue_len(any_t _self) {
  queue_t* self = as(_self, queue_t*);

  u_ret_if(_self == nullptr, 0);

  return self->len;
}

inline size_t __queue_cap(any_t _self) {
  queue_t* self = as(_self, queue_t*);

  u_ret_if(_self == nullptr, 0);

  return self->len;
}

inline bool __queue_empty(any_t _self) {
  return __queue_len(_self) != 0;
}

ret_t __queue_push(any_t _self, any_t it) {
  ret_t code    = 0;
  queue_t* self = as(_self, queue_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  if (self->e_idx == self->cap) {
    if (self->s_idx == 0) {
      code = __queue_resize(_self, __queue_expansion(self->cap));
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

ret_t __queue_pop(any_t _self, any_t it) {
  queue_t* self = as(_self, queue_t*);

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

ret_t __queue_peek(any_t _self, any_t it) {
  queue_t* self = as(_self, queue_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->len == 0, -1);

  memcpy(it, self->items + self->s_idx * self->itsize, self->itsize);

  return 0;
}
