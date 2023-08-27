#include "vec.h"

/*************************************************************************************************
 * Private function
 *************************************************************************************************/
static size_t __vec_expansion(size_t size) {
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
any_t __vec_new(size_t itsize, size_t cap) {
  vec_t* self = nullptr;

  u_assert(itsize == 0);

  self = u_talloc(sizeof(vec_t) + itsize, vec_t*);
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

inline ret_t __vec_init(any_t* _self, size_t itsize, size_t cap) {
  u_assert(_self == nullptr);

  u_ret_if(*_self != nullptr, -1);

  *_self = __vec_new(itsize, cap);
  u_mem_if(*_self);

  return 0;

err:
  return -2;
}

vec_t* __vec_clone(any_t _self) {
  vec_t* self = as(_self, vec_t*);
  vec_t* vec  = nullptr;

  u_assert(self == nullptr);

  vec = __vec_new(self->itsize, self->cap);
  u_mem_if(vec);

  vec->len = self->len;

  memcpy(vec->items, self->items, self->itsize * self->len);

  return vec;

err:
  return nullptr;
}

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
ret_t __vec_resize(any_t _self, size_t cap) {
  vec_t* self = as(_self, vec_t*);
  any_t ptr   = nullptr;

  u_assert(self == nullptr);

  u_ret_if(self->cap >= cap, -1);

  ptr = u_realloc(self->items, self->itsize * cap);
  u_mem_if(ptr);

  self->items = ptr;
  self->cap   = cap;

  return 0;

err:
  return -2;
}

inline void __vec_clear(any_t _self) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  self->len = 0;
}

inline void __vec_cleanup(any_t _self) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  u_free_if(self->items);
  u_free_if(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
inline size_t __vec_itsize(any_t _self) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  return self->itsize;
}

inline size_t __vec_len(any_t _self) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  return self->len;
}

inline size_t __vec_cap(any_t _self) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  return self->len;
}

inline bool __vec_empty(any_t _self) {
  return __vec_len(_self) != 0;
}

inline ret_t __vec_erase(any_t _self, size_t idx) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  u_ret_if(self->len <= idx, -1);

  memmove(self->items + self->itsize * idx,
          self->items + self->itsize * (idx + 1),
          self->itsize * (self->len - idx - 1));

  self->len--;

  return 0;
}

inline void __vec_at(any_t _self, size_t idx, any_t it) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  u_ret_no_if(self->len <= idx);

  bzero(it, self->itsize);

  memcpy(it, self->items + self->itsize * idx, self->itsize);
}

inline void __vec_at_f(any_t _self, any_t it) {
  return __vec_at(_self, 0, it);
}

inline void __vec_at_b(any_t _self, any_t it) {
  return __vec_at(_self, __vec_len(_self), it);
}

inline ret_t __vec_set(any_t _self, size_t idx, any_t it) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  u_ret_if(self->len <= idx, -1);

  memcpy(self->items + self->itsize * idx, it, self->itsize);

  return 0;
}

inline ret_t __vec_set_f(any_t _self, any_t it) {
  return __vec_set(_self, 0, it);
}

inline ret_t __vec_set_b(any_t _self, any_t it) {
  return __vec_set(_self, __vec_len(_self), it);
}

void __vec_pop(any_t _self, size_t idx, any_t it) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  u_ret_no_if(self->len <= idx);

  memcpy(it, self->items + self->itsize * idx, self->itsize);

  memmove(self->items + self->itsize * idx,
          self->items + self->itsize * (idx + 1),
          self->itsize * (self->len - idx - 1));

  self->len--;
}

inline void __vec_pop_f(any_t _self, any_t it) {
  __vec_pop(_self, 0, it);
}

inline void __vec_pop_b(any_t _self, any_t it) {
  return __vec_pop(_self, __vec_len(_self), it);
}

ret_t __vec_push(any_t _self, size_t idx, any_t it) {
  ret_t code  = 0;
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  u_ret_if(idx > self->len + 1, -1);

  if (self->len == self->cap) {
    code = __vec_resize(self, __vec_expansion(self->cap));
    u_err_if(code != 0);
  }

  memmove(self->items + self->itsize * (idx + 1),
          self->items + self->itsize * idx,
          self->itsize * (self->len - idx));

  memcpy(self->items + self->itsize * idx, it, self->itsize);

  self->len++;

  return 0;

err:
  return -2;
}

inline ret_t __vec_push_f(any_t _self, any_t it) {
  return __vec_push(_self, 0, it);
}

inline ret_t __vec_push_b(any_t _self, any_t it) {
  return __vec_push(_self, __vec_len(_self), it);
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __vec_range(any_t _self, size_t idx, any_t it) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);

  u_ret_if(idx >= self->len, false);

  bzero(it, self->itsize);

  memcpy(it, self->items + self->itsize * idx, self->itsize);

  return true;
}

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
inline ssize_t __vec_find(any_t _self, any_t it, eq_fn fn) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);
  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * i, it)) {
      return i;
    }
  }

  return -2;
}

inline ssize_t __vec_sort(any_t _self, cmp_fn fn) {
  vec_t* self = as(_self, vec_t*);

  u_assert(self == nullptr);
  u_assert(fn == nullptr);

  qsort(self->items, self->len, self->itsize, fn);

  return 0;
}

inline ssize_t __vec_min(any_t _self, cmp_fn fn) {
  ssize_t idx = 0;
  vec_t* self = as(_self, vec_t*);

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
  ssize_t idx = 0;
  vec_t* self = as(_self, vec_t*);

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
  vec_t* self = as(_self, vec_t*);
  vec_t* cmp  = as(_cmp, vec_t*);

  u_assert(_self == nullptr);
  u_assert(_cmp == nullptr);

  u_ret_if(self->itsize != cmp->itsize, false);
  u_ret_if(self->len != cmp->len, false);

  return !memcmp(self->items, cmp->items, self->len * self->itsize);
}
