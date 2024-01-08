#include <u/core/ivec.h>

/***************************************************************************************************
 * Private
 **************************************************************************************************/
typedef struct vec_t vec_t;
struct vec_t {
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
};

#undef at
#define at(idx) (self->items + self->itsize * (idx))

#undef getidx
#define getidx(idx) (((idx) < 0) ? ((idx) + as(self->len, ssize_t)) : (idx))

static ret_t vec_resize(vec_t* self) {
  size_t cap  = 0;
  any_t items = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

  items = u_realloc(self->items, self->itsize * cap);
  u_mem_if(items);

  infln("vec resize(cap(%zu -> %zu))", self->cap, cap);

  self->items = items;
  self->cap   = cap;

  return 0;

err:
  return -1;
}

/***************************************************************************************************
 * Create & Clone
 **************************************************************************************************/
u_vec_t vec_new(size_t itsize, size_t cap) {
  vec_t* self = nullptr;

  u_check_ret(itsize == 0, nullptr);

  cap = (cap < U_VEC_CAP) ? U_VEC_CAP : cap;

  self = u_zalloc(sizeof(vec_t));
  u_mem_if(self);

  self->items = u_zalloc(itsize * cap);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->cap    = cap;

  infln("itsize(%zu), cap(%zu)", itsize, cap);

  return as(self, u_vec_t);

err:
  u_free_if(self);

  return nullptr;
}

/***************************************************************************************************
 * Destruction
 **************************************************************************************************/
void vec_clear(u_vec_t _self) {
  vec_t* self = as(_self, vec_t*);

  self->len = 0;
}

void vec_cleanup(u_vec_t _self) {
  vec_t* self = as(_self, vec_t*);

  u_free_if(self->items);
  u_free_if(self);
}

/***************************************************************************************************
 * Interface
 **************************************************************************************************/
size_t vec_len(u_vec_t _self) {
  vec_t* self = as(_self, vec_t*);

  return self->len;
}

size_t vec_cap(u_vec_t _self) {
  vec_t* self = as(_self, vec_t*);

  return self->cap;
}

bool vec_isexist(u_vec_t _self, size_t idx) {
  vec_t* self = as(_self, vec_t*);

  return self->len > idx;
}

void vec_at(u_vec_t _self, size_t idx, any_t item) {
  vec_t* self = as(_self, vec_t*);

  u_check_nret(self == nullptr);
  u_check_nret(idx > self->len && idx < -self->len, "idx(%ld), len(%zu)", idx, self->len);

  idx = getidx(idx);
  memcpy(item, at(idx), self->itsize);
}

void vec_re(u_vec_t _self, size_t idx, any_t item) {
  vec_t* self = as(_self, vec_t*);

  u_check_nret(self == nullptr);
  u_check_nret(idx > self->len && idx < -self->len, "idx(%ld), len(%zu)", idx, self->len);

  idx = getidx(idx);
  memcpy(at(idx), item, self->itsize);
}

void vec_pop(u_vec_t _self, size_t idx, any_t item) {
  vec_t* self = as(_self, vec_t*);

  u_check_nret(self == nullptr);
  u_check_nret(idx > self->len && idx < -self->len, "idx(%ld), len(%zu)", idx, self->len);

  idx = getidx(idx);
  memcpy(item, at(idx), self->itsize);
  if (idx != self->len - 1) {
    memmove(at(idx), at(idx + 1), (self->len - idx - 1) * self->itsize);
  }

  self->len--;
}

void vec_put(u_vec_t _self, ssize_t idx, any_t item) {
  vec_t* self = as(_self, vec_t*);
  ret_t code  = 0;

  u_check_nret(self == nullptr);
  u_check_nret(idx > self->len && idx < -self->len, "idx(%ld), len(%zu)", idx, self->len);

  idx = getidx(idx);
  if (self->len == self->cap) {
    code = vec_resize(self);
    u_err_if(code != 0, "resize failed.");
  }

  if (idx != self->len) {
    memmove(at(idx + 1), at(idx), (self->len - idx) * self->itsize);
  }

  memcpy(at(idx), item, self->itsize);

  self->len++;

err:
}

/***************************************************************************************************
 * Iterator
 **************************************************************************************************/
bool vec_range(u_vec_t _self, ssize_t* idx, bool flag, any_t item) {
  vec_t* self = as(_self, vec_t*);

  u_check_ret(*idx == -1 || *idx == self->len, false);

  /* initialize */
  if (*idx == -2) {
    *idx = flag ? 0 : as(self->len - 1, ssize_t);
  }

  memcpy(item, at(*idx), self->itsize);

  return true;
}
