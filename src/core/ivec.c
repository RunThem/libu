#include <u/core.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct vec_t vec_t;
struct vec_t {
  bool flags[4];
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
};

/***************************************************************************************************
 * Macro
 **************************************************************************************************/
#undef at
#define at(idx) (self->items + self->itsize * (idx))

#undef getidx
#define getidx(idx) (((idx) < 0) ? ((idx) + as(self->len + 1, ssize_t)) : (idx))

/***************************************************************************************************
 * Function
 **************************************************************************************************/
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

u_vec_t vec_new(size_t itsize) {
  vec_t* self = nullptr;

  u_check_ret(itsize == 0, nullptr);

  self = u_zalloc(sizeof(vec_t));
  u_mem_if(self);

  self->items = u_zalloc(itsize * 16);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->cap    = 16;

  infln("itsize(%zu)", itsize);

  return as(self, u_vec_t);

err:
  u_free_if(self);

  return nullptr;
}

void vec_clear(u_vec_t _self) {
  vec_t* self = as(_self, vec_t*);

  self->len = 0;
}

void vec_cleanup(u_vec_t _self) {
  vec_t* self = as(_self, vec_t*);

  u_free_if(self->items);
  u_free_if(self);
}

bool vec_isexist(u_vec_t _self, size_t idx) {
  vec_t* self = as(_self, vec_t*);

  return self->len > idx;
}

size_t vec_len(u_vec_t _self) {
  vec_t* self = as(_self, vec_t*);

  return self->len;
}

size_t vec_cap(u_vec_t _self) {
  vec_t* self = as(_self, vec_t*);

  return self->cap;
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
  u_check_nret(idx > as(self->len, ssize_t) || idx < -as(self->len + 1, ssize_t),
               "idx(%ld), len(%zu)",
               idx,
               self->len);

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

bool vec_for_init(u_vec_t _self, bool flag) {
  vec_t* self = as(_self, vec_t*);

  u_check_ret(self == nullptr, false);
  u_check_ret(self->len == 0, false);

  self->flags[0] = !self->flags[0];
  self->flags[1] = flag;
  self->flags[2] = true;
  self->flags[3] = false;

  return self->flags[0];
}

bool vec_for(u_vec_t _self, size_t* idx, any_t item) {
  vec_t* self = as(_self, vec_t*);

  u_check_ret(self == nullptr, false);
  u_check_ret(self->flags[3], false);

  /* 初始化 */
  if (self->flags[2]) {
    *idx = self->flags[1] ? 0 : self->len - 1;
    self->flags[2] = !self->flags[2];
  } else { /* 迭代 */
    *idx += self->flags[1] ? 1 : -1;
  }

  /* 判断是否迭代 */
  if ((self->flags[1] && *idx == self->len - 1) || (!self->flags[1] && *idx == 0)) {
    self->flags[3] = true;
  }

  memcpy(item, at(*idx), self->itsize);

  return true;
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