#include <u/core/stack.h>

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
typedef struct stack_t stack_t;
struct stack_t {
  size_t itsize;
  size_t len;
  size_t cap;

  any_t items;
};

#undef at
#define at(idx) (self->items + self->itsize * (idx))

#undef selfof
#define selfof(self) (assert((self) != nullptr), as((self) - sizeof(stack_t), stack_t*))

static ret_t __stack_resize(stack_t* self) {
  size_t cap  = 0;
  any_t items = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

  items = u_realloc(self->items, self->itsize * cap);
  u_mem_if(items);

  infln("stack resize(cap(%zu -> %zu))", self->cap, cap);

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

  u_check_ret(itsize == 0, nullptr);

  self = u_zalloc(sizeof(stack_t) + itsize);
  u_mem_if(self);

  self->items = u_zalloc(itsize * cap);
  u_mem_if(self->items);

  infln("stack new(itsize(%zu), cap(%zu))", itsize, cap);

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
  selfof(_self)->len = 0;
}

void __stack_cleanup(any_t _self) {
  stack_t* self = selfof(_self);

  u_free(self->items);
  u_free(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __stack_itsize(any_t _self) {
  return selfof(_self)->itsize;
}

size_t __stack_len(any_t _self) {
  return selfof(_self)->len;
}

size_t __stack_cap(any_t _self) {
  return selfof(_self)->cap;
}

bool __stack_empty(any_t _self) {
  return selfof(_self)->len == 0;
}

void __stack_peek(any_t _self) {
  stack_t* self = selfof(_self);
  any_t item    = _self;

  u_check_nret(self->len == 0);

  memcpy(item, at(self->len - 1), self->itsize);
}

void __stack_pop(any_t _self) {
  stack_t* self = selfof(_self);
  any_t item    = _self;

  u_check_nret(self->len == 0);

  memcpy(item, at(self->len - 1), self->itsize);

  self->len--;
}

ret_t __stack_push(any_t _self) {
  stack_t* self = selfof(_self);
  any_t item    = _self;
  ret_t code    = 0;

  if (self->len == self->cap) {
    code = __stack_resize(self);
    u_err_if(code != 0, "stack push() resize failed.");
  }

  memcpy(at(self->len), item, self->itsize);

  self->len++;

  return 0;

err:
  return -2;
}
