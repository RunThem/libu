#include "core/queue.h"

/*
 * 0        8        16       24       32       40       48
 * +--------+--------+--------+--------+--------+--------+------------+
 * | itsize |  len   |  cap   | s_idx  | e_idx  | items  |    ...     |
 * +--------+--------+--------+--------+--------+---------------------+
 *                                                       |  sizeof(T)
 *                                                       v
 *                                                  return point
 *
 * */

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;

  size_t s_idx;
  size_t e_idx;

  any_t items;
} queue_t;

#undef self_of
#define self_of(self) (assert((self) != nullptr), as((self) - sizeof(queue_t), queue_t*))

static ret_t __queue_resize(queue_t* self) {
  size_t cap  = 0;
  any_t items = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

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
any_t __queue_new(size_t itsize, size_t cap) {
  queue_t* self = nullptr;

  u_assert(itsize == 0);

  self = u_zalloc(sizeof(queue_t) + itsize);
  u_mem_if(self);

  self->items = u_zalloc(itsize * cap);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->len    = 0;
  self->cap    = cap;

  self->s_idx = 0;
  self->e_idx = 0;

  return self + 1;

err:
  u_free_if(self);

  return nullptr;
}

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
void __queue_clear(any_t _self) {
  queue_t* self = self_of(_self);

  self->len   = 0;
  self->s_idx = 0;
  self->e_idx = 0;
}

void __queue_cleanup(any_t _self) {
  queue_t* self = self_of(_self);

  u_free_if(self->items);
  u_free_if(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __queue_itsize(any_t _self) {
  return self_of(_self)->itsize;
}

size_t __queue_len(any_t _self) {
  return self_of(_self)->len;
}

size_t __queue_cap(any_t _self) {
  return self_of(_self)->cap;
}

bool __queue_empty(any_t _self) {
  return self_of(_self)->len == 0;
}

void __queue_peek(any_t _self) {
  queue_t* self = self_of(_self);
  any_t item    = _self;

  u_nonret_if(self->len == 0);

  memcpy(item, self->items + self->s_idx * self->itsize, self->itsize);
}

void __queue_pop(any_t _self) {
  queue_t* self = self_of(_self);
  any_t item    = _self;

  u_nonret_if(self->len == 0);

  if (self->s_idx >= self->cap / 2) {
    memmove(self->items, self->items + self->s_idx * self->itsize, self->len * self->itsize);
    self->e_idx -= self->s_idx;
    self->s_idx = 0;
  }

  memcpy(item, self->items + self->s_idx * self->itsize, self->itsize);
  self->s_idx++;
  self->len--;
}

ret_t __queue_push(any_t _self) {
  queue_t* self = self_of(_self);
  any_t item    = _self;
  ret_t code    = 0;

  if (self->e_idx == self->cap) {
    if (self->s_idx == 0) {
      code = __queue_resize(self);
      u_err_if(code != 0);
    } else {
      memmove(self->items, self->items + self->s_idx * self->itsize, self->len * self->itsize);
      self->e_idx -= self->s_idx;
      self->s_idx = 0;
    }
  }

  memcpy(self->items + self->e_idx * self->itsize, item, self->itsize);
  self->e_idx++;
  self->len++;

  return 0;

err:
  return -2;
}
