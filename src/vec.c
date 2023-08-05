#include "vec.h"

static size_t __vec_cap(size_t size) {
  if (size < 16) {
    return 16;
  } else if (size < 1024) {
    return size * 2;
  }

  return size + 512;
}

ret_t __vec_init(any_t _self, size_t itsize, size_t cap) {
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(itsize == 0, -1);

  self->itsize = itsize;
  self->len    = 0;
  self->cap    = cap;

  self->items = u_calloc(self->cap, itsize);
  u_alloc_if(self->items);

  return 0;

err:
  bzero(self, sizeof(vec_t));

  return -2;
}

ret_t __vec_resize(any_t _self, size_t cap) {
  any_t ptr   = nullptr;
  vec_t* self = as(_self, vec_t*);

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

vec_t __vec_clone(any_t _self) {
  ret_t code  = 0;
  vec_t vec   = {};
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, vec);

  code = __vec_init(&vec, self->itsize, self->cap);
  u_goto_if(code != 0);

  memcpy(vec.items, self->items, self->itsize * self->len);

  vec.len = self->len;

  return vec;

err:
  vec.itsize = 0;
  vec.len    = 0;
  vec.cap    = 0;

  return vec;
}

ret_t __vec_cleanup(any_t _self) {
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);

  u_free_if(self->items);

  bzero(self, sizeof(vec_t));

  return 0;
}

ret_t __vec_at(any_t _self, size_t idx, any_t it) {
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);
  u_ret_if(self->len <= idx, -1);

  memcpy(it, self->items + self->itsize * idx, self->itsize);

  return 0;
}

ret_t __vec_set(any_t _self, size_t idx, any_t it) {
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);
  u_ret_if(self->len <= idx, -1);

  memcpy(self->items + self->itsize * idx, it, self->itsize);

  return 0;
}

ret_t __vec_push(any_t _self, size_t idx, any_t it) {
  ret_t code  = 0;
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);
  u_ret_if(self->len < idx, -1);

  if (self->len == self->cap) {
    code = __vec_resize(self, __vec_cap(self->cap));
    u_goto_if(code != 0);
  }

  /* [1, 2, 3, 4, 5]
   * push(0, 10)
   * push(2, 10)
   * push(5, 10)
   * */
  memmove(self->items + self->itsize * (idx + 1),
          self->items + self->itsize * idx,
          self->itsize * (self->len - idx));

  memcpy(self->items + self->itsize * idx, it, self->itsize);

  self->len++;

  return 0;

err:
  return -2;
}

ret_t __vec_pop(any_t _self, size_t idx, any_t it) {
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);
  u_ret_if(self->len <= idx, -1);

  memcpy(it, self->items + self->itsize * idx, self->itsize);

  /* [1, 2, 3, 4, 5]
   * pop(0)
   * pop(2)
   * pop(4)
   * */
  memmove(self->items + self->itsize * idx,
          self->items + self->itsize * (idx + 1),
          self->itsize * (self->len - idx - 1));

  self->len--;

  return 0;
}

ret_t __vec_erase(any_t _self, size_t idx) {
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);
  u_ret_if(self->len <= idx, -1);

  memmove(self->items + self->itsize * idx,
          self->items + self->itsize * (idx + 1),
          self->itsize * (self->len - idx - 1));

  self->len--;

  return 0;
}

ssize_t __vec_find(any_t _self, any_t it, eq_fn fn) {
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);
  u_ret_if(fn == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * i, it)) {
      return i;
    }
  }

  return -2;
}

ssize_t __vec_sort(any_t _self, cmp_fn fn) {
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(fn == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);

  qsort(self->items, self->len, self->itsize, fn);

  return 0;
}

ssize_t __vec_min(any_t _self, cmp_fn fn) {
  ssize_t idx = 0;
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(fn == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * idx, self->items + self->itsize * i) == 1) {
      idx = i;
    }
  }

  return idx;
}

ssize_t __vec_max(any_t _self, cmp_fn fn) {
  ssize_t idx = 0;
  vec_t* self = as(_self, vec_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(fn == nullptr, -1);

  u_ret_if(self->items == nullptr, -1);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * idx, self->items + self->itsize * i) == -1) {
      idx = i;
    }
  }

  return idx;
}

bool __vec_cmp(any_t _self, any_t _cmp) {
  vec_t* self = as(_self, vec_t*);
  vec_t* cmp  = as(_cmp, vec_t*);

  u_ret_if(_self == nullptr, false);
  u_ret_if(_cmp == nullptr, false);
  u_ret_if(self->itsize != cmp->itsize, false);
  u_ret_if(self->len != cmp->len, false);

  return !memcmp(self->items, cmp->items, self->len * self->itsize);
}
