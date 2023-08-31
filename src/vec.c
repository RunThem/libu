#include "vec.h"

/*************************************************************************************************
 * Private function
 *************************************************************************************************/
/*
 *
 * 0        8        16       32       40       48
 * +--------+--------+--------+--------+--------+--------------+
 * | itsize |  len   |  cap   | items  |  item  |     ....     |
 * +--------+--------+--------+-----------------+--------------+
 *                                     ^   T*       sizeof(T)
 *                                     |
 *                                     +
 *                                   return
 *
 * */
typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
  any_t item;
} vec_t;

#define vec_of(self) as(container_of(self, vec_t, item), vec_t*)

static ret_t __vec_resize(vec_t* self) {
  size_t cap = 0;
  any_t ptr  = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

  ptr = u_realloc(self->items, self->itsize * cap);
  u_mem_if(ptr);

  self->items = ptr;
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

  self = u_talloc(sizeof(vec_t) + itsize, vec_t*);
  u_mem_if(self);

  cap = (cap < U_VEC_CAP) ? U_VEC_CAP : cap;

  self->items = u_zalloc(itsize * cap);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->cap    = cap;

  return &self->item;

err:
  u_free_if(self);

  return nullptr;
}

any_t __vec_clone(any_t _self) {
  vec_t* self = vec_of(_self);
  vec_t* vec  = nullptr;

  u_assert(self == nullptr);

  vec = __vec_new(self->itsize, self->cap);
  u_mem_if(vec);

  memcpy(vec->items, self->items, self->itsize * self->len);
  vec->len = self->len;

  return &self->item;

err:
  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __vec_clear(any_t _self) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);

  self->len = 0;
}

void __vec_cleanup(any_t _self) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);

  u_free_if(self->items);
  u_free_if(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __vec_itsize(any_t _self) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);

  return self->itsize;
}

size_t __vec_len(any_t _self) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);

  return self->len;
}

size_t __vec_cap(any_t _self) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);

  return self->len;
}

bool __vec_empty(any_t _self) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);

  return self->itsize != 0;
}

any_t __vec_at(any_t _self, size_t idx) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);
  u_assert(idx >= self->len);

  return self->items + idx * self->itsize;
}

void __vec_pop(any_t _self, size_t idx) {
  vec_t* self = vec_of(_self);
  any_t point = nullptr;

  u_assert(self == nullptr);
  u_noret_if(idx >= self->len);

  if (idx != self->len - 1) {
    point = self->items + self->itsize * idx;
    memmove(point, point + self->itsize, self->itsize * (self->len - idx - 1));
  }

  self->len--;
}

ret_t __vec_push(any_t _self, size_t idx) {
  vec_t* self = vec_of(_self);
  any_t item  = self + 1;
  any_t point = nullptr;
  ret_t code  = 0;

  u_assert(self == nullptr);
  u_ret_if(idx > self->len, -1);

  if (self->len == self->cap) {
    code = __vec_resize(self);
    u_err_if(code != 0);
  }

  point = self->items + self->itsize * idx;
  if (idx != self->len) {
    memmove(point + self->itsize, point, self->itsize * (self->len - idx));
  }

  memcpy(point, item, self->itsize);
  self->len++;

  return 0;

err:
  return -2;
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __vec_range(any_t _self, bool flag) {
  vec_t* self = vec_of(_self);
  any_t item  = _self;

  u_assert(self == nullptr);

  /* initialize */
  if (self->item == nullptr) {
    self->item = flag ? self->items : self->items + (self->len - 1) * self->itsize;
  } else { /* iteration */
    self->item += flag ? self->itsize : -self->itsize;
  }

  /* whether it is out of range */
  if ((self->item == self->items + self->len * self->itsize) ||
      (self->item == self->items - self->itsize)) {
    self->item = self + 1;

    return false;
  }

  return true;
}

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
void __vec_sort(any_t _self, cmp_fn fn) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);
  u_assert(fn == nullptr);

  qsort(self->items, self->len, self->itsize, fn);
}

#if 0

inline ssize_t __vec_find(any_t _self, any_t it, eq_fn fn) {
  vec_t* self = vec_of(_self);

  u_assert(self == nullptr);
  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * i, it)) {
      return i;
    }
  }

  return -2;
}

inline ssize_t __vec_min(any_t _self, cmp_fn fn) {
  vec_t* self = vec_of(_self);
  ssize_t idx = 0;

  u_assert(_self == nullptr);
  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * idx, self->items + self->itsize * i) == 1) {
      idx = i;
    }
  }

  return idx;
}

inline ssize_t __vec_max(any_t _self, cmp_fn fn) {
  vec_t* self = vec_of(_self);
  ssize_t idx = 0;

  u_assert(_self == nullptr);
  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * idx, self->items + self->itsize * i) == -1) {
      idx = i;
    }
  }

  return idx;
}

inline bool __vec_cmp(any_t _self, any_t _cmp) {
  vec_t* self = vec_of(_self);
  vec_t* cmp  = vec_of(_cmp);

  u_assert(_self == nullptr);
  u_assert(_cmp == nullptr);

  u_ret_if(self->itsize != cmp->itsize, false);
  u_ret_if(self->len != cmp->len, false);

  return !memcmp(self->items, cmp->items, self->len * self->itsize);
}

#endif
