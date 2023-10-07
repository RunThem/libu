#include <u/core/vec.h>

/*
 * 0        8        16       24       32
 * +--------+--------+--------+--------+--------------+
 * | itsize |  len   |  cap   | items  |     ....     |
 * +--------+--------+--------+--------+--------------+
 *                                |    ^   sizeof(T)
 *                                |    |
 *                                |    +
 *                                |  return point
 *                                |
 *                                |
 *                                |
 *                                |      +-----+-----+-----+
 *                                +----->+  T  |  T  |  T  |...
 *                                       +-----+-----+-----+
 *
 * @param
 * itsize
 *      memory size of the element.
 * len
 *      number of elements in the vector.
 * cap
 *      number of elements the vector can hold.
 * items
 *      pointer to the underlying array.
 *
 * */

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct vec_t vec_t;
struct vec_t {
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
};

#undef at
#define at(idx) (self->items + self->itsize * (idx))

#undef selfof
#define selfof(self) (assert((self) != nullptr), as((self) - sizeof(vec_t), vec_t*))

static ret_t __vec_resize(vec_t* self) {
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
 * Create & Clone
 *************************************************************************************************/
any_t __vec_new(size_t itsize, size_t cap) {
  vec_t* self = nullptr;

  u_assert(itsize == 0);

  self = u_zalloc(sizeof(vec_t) + itsize);
  u_mem_if(self);

  cap = (cap < U_VEC_CAP) ? U_VEC_CAP : cap;

  self->items = u_zalloc(itsize * cap);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->cap    = cap;

  return self + 1;

err:
  u_free_if(self);

  return nullptr;
}

any_t __vec_clone(any_t _self) {
  vec_t* self = selfof(_self);
  vec_t* vec  = nullptr;

  vec = __vec_new(self->itsize, self->cap);
  u_mem_if(vec);

  vec = as((vec) - sizeof(vec_t), vec_t*);

  memcpy(vec->items, self->items, self->itsize * self->len);
  vec->len = self->len;

  return vec + 1;

err:
  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __vec_clear(any_t _self) {
  selfof(_self)->len = 0;
}

void __vec_cleanup(any_t _self) {
  vec_t* self = selfof(_self);

  u_free_if(self->items);
  u_free_if(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __vec_itsize(any_t _self) {
  return selfof(_self)->itsize;
}

size_t __vec_len(any_t _self) {
  return selfof(_self)->len;
}

size_t __vec_cap(any_t _self) {
  return selfof(_self)->cap;
}

bool __vec_empty(any_t _self) {
  return selfof(_self)->len == 0;
}

/*
 * re
 * */
void __vec_re(any_t _self, size_t idx) {
  vec_t* self = selfof(_self);
  any_t item  = _self;

  u_assert(idx >= self->len);

  memcpy(at(idx), item, self->itsize);
}

void __vec_re_front(any_t _self) {
  __vec_re(_self, 0);
}

void __vec_re_back(any_t _self) {
  __vec_re(_self, selfof(_self)->len - 1);
}

/*
 * at
 * */
void __vec_at(any_t _self, size_t idx) {
  vec_t* self = selfof(_self);
  any_t item  = _self;

  u_assert(idx >= self->len);

  memcpy(item, at(idx), self->itsize);
}

void __vec_at_front(any_t _self) {
  __vec_at(_self, 0);
}

void __vec_at_back(any_t _self) {
  __vec_at(_self, selfof(_self)->len - 1);
}

/*
 * pop
 * */
void __vec_pop(any_t _self, size_t idx) {
  vec_t* self = selfof(_self);
  any_t item  = _self;

  u_assert(idx >= self->len);

  memcpy(item, at(idx), self->itsize);
  if (idx != self->len - 1) {
    memmove(at(idx), at(idx + 1), (self->len - idx - 1) * self->itsize);
  }

  self->len--;
}

void __vec_pop_front(any_t _self) {
  __vec_pop(_self, 0);
}

void __vec_pop_back(any_t _self) {
  __vec_pop(_self, selfof(_self)->len - 1);
}

/*
 * push
 * */
ret_t __vec_push(any_t _self, size_t idx) {
  vec_t* self = selfof(_self);
  any_t item  = _self;
  ret_t code  = 0;

  u_ret_if(idx > self->len, -1);

  if (self->len == self->cap) {
    code = __vec_resize(self);
    u_err_if(code != 0);
  }

  if (idx != self->len) {
    memmove(at(idx + 1), at(idx), (self->len - idx) * self->itsize);
  }

  memcpy(at(idx), item, self->itsize);

  self->len++;

  return 0;

err:
  return -2;
}

ret_t __vec_push_front(any_t _self) {
  return __vec_push(_self, 0);
}

ret_t __vec_push_back(any_t _self) {
  return __vec_push(_self, selfof(_self)->len);
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __vec_range(any_t _self, ssize_t* idx, bool flag) {
  vec_t* self = selfof(_self);
  any_t item  = _self;

  if (*idx == -1 || *idx == self->len) {
    return false;
  }

  /* initialize */
  if (*idx == -2) {
    *idx = flag ? 0 : as(self->len - 1, ssize_t);
  }

  memcpy(item, at(*idx), self->itsize);

  return true;
}

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
void __vec_sort(any_t _self, cmp_fn fn) {
  vec_t* self = selfof(_self);

  u_assert(fn == nullptr);

  qsort(self->items, self->len, self->itsize, fn);
}

ssize_t __vec_find(any_t _self, eq_fn fn) {
  vec_t* self = selfof(_self);
  any_t item  = _self;

  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(at(i), item)) {
      return i;
    }
  }

  return -2;
}

ssize_t __vec_min(any_t _self, cmp_fn fn) {
  vec_t* self = selfof(_self);
  ssize_t idx = 0;

  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(at(idx), at(i)) == 1) {
      idx = i;
    }
  }

  return idx;
}

ssize_t __vec_max(any_t _self, cmp_fn fn) {
  vec_t* self = selfof(_self);
  ssize_t idx = 0;

  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(at(idx), at(i)) == -1) {
      idx = i;
    }
  }

  return idx;
}
