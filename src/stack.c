#include "stack.h"

/*
 * 0        8        16       24       32
 * +--------+--------+--------+--------+------------+
 * | itsize |  len   |  cap   | items  |    ...     |
 * +--------+--------+--------+---------------------+
 *                                     |  sizeof(T)
 *                                     v
 *                                return point
 *
 */

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
} stack_t;

#define stack_of(self) (assert((self) != nullptr), as((self) - sizeof(stack_t), stack_t*))

static ret_t __stack_resize(stack_t* self) {
  size_t cap  = 0;
  any_t items = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

  inf("resize %zu", cap);

  items = u_realloc(self->items, self->itsize * cap);
  u_mem_if(items);

  self->items = items;
  self->cap   = cap;

  return 0;

err:
  return -1;
}

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __stack_new(size_t itsize, size_t cap) {
  stack_t* self = nullptr;

  u_assert(itsize == 0);

  self = u_talloc(sizeof(stack_t) + itsize, stack_t*);
  u_mem_if(self);

  self->items = u_zalloc(itsize * cap);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->cap    = cap;

  return self + 1;

err:
  u_free_if(self);

  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __stack_clear(any_t _self) {
  stack_of(_self)->len = 0;
}

void __stack_cleanup(any_t _self) {
  stack_t* self = stack_of(_self);

  u_free_if(self->items);
  u_free_if(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __stack_itsize(any_t _self) {
  return stack_of(_self)->itsize;
}

size_t __stack_len(any_t _self) {
  return stack_of(_self)->len;
}

size_t __stack_cap(any_t _self) {
  return stack_of(_self)->cap;
}

bool __stack_empty(any_t _self) {
  return stack_of(_self)->len == 0;
}

void __stack_peek(any_t _self) {
  stack_t* self = stack_of(_self);
  any_t item    = _self;

  u_noret_if(self->len == 0);

  memcpy(item, self->items + (self->len - 1) * self->itsize, self->itsize);
}

void __stack_pop(any_t _self) {
  stack_t* self = stack_of(_self);
  any_t item    = _self;

  u_noret_if(self->len == 0);

  memcpy(item, self->items + (self->len - 1) * self->itsize, self->itsize);
  self->len--;
}

ret_t __stack_push(any_t _self) {
  stack_t* self = stack_of(_self);
  any_t item    = _self;
  ret_t code    = 0;

  if (self->len == self->cap) {
    code = __stack_resize(self);
    u_err_if(code != 0);
  }

  memcpy(self->items + self->len * self->itsize, item, self->itsize);
  self->len++;

  return 0;

err:
  return -2;
}
