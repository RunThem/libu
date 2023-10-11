#include <stdarg.h>
#include <u/core/str.h>

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct str_t str_t;
struct str_t {
  size_t cap;
  size_t len;
  char data[];
};

#define selfof(self) (assert((self) != nullptr), as(container_of(self, str_t, data), str_t*))

static ret_t __str_resize(u_str_t* _self, size_t cap) {
  str_t* self = selfof(*_self);
  str_t* str  = nullptr;

  cap += (cap < 102400) ? cap : 10240;
  str = u_realloc(self, cap);
  u_mem_if(str);

  str->cap = cap;

  *_self = str->data;

  return 0;
err:
  return -1;
}

/*************************************************************************************************
 * Create
 *************************************************************************************************/
u_str_t __str_new(size_t cap, u_str_t fmt, ...) {
  str_t* self    = nullptr;
  va_list ap     = {0};
  char buf[4096] = {0};
  size_t len     = 0;

  u_assert(cap == 0 && fmt == nullptr);
  u_assert(cap != 0 && fmt != nullptr);

  if (cap == 0) {
    va_start(ap, fmt);

    len = vsnprintf(buf, arr_len(buf), fmt, ap);
    cap = len;

    va_end(ap);
  }

  self = u_zalloc(sizeof(str_t) + cap + 1);
  u_mem_if(self);

  strncpy(self->data, buf, len);

  self->len = len;
  self->cap = cap;

  return self->data;

err:
  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __str_clear(u_str_t* _self) {
  str_t* self = selfof(*_self);

  /* static string */
  u_assert(self->cap == 0);

  self->len     = 0;
  self->data[0] = '\0';
}

void __str_cleanup(u_str_t* _self) {
  str_t* self = selfof(*_self);

  /* static string */
  u_assert(self->cap == 0);

  u_free_if(self);
  *_self = nullptr;
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __str_len(u_str_t* _self) {
  return selfof(*_self)->len;
}

size_t __str_cap(u_str_t* _self) {
  return selfof(*_self)->cap;
}

bool __str_empty(u_str_t* _self) {
  return selfof(*_self)->len == 0;
}

ret_t __str_append(u_str_t* _self, u_str_t fmt, ...) {
  str_t* self    = selfof(*_self);
  va_list ap     = {0};
  char buf[4096] = {0};
  size_t len     = 0;
  ret_t code     = 0;

  /* static string */
  u_assert(self->cap == 0);
  u_assert(fmt == nullptr);

  va_start(ap, fmt);
  len = vsnprintf(buf, arr_len(buf), fmt, ap);
  va_end(ap);

  if (self->cap - self->len <= len) {
    /* resize */
    code = __str_resize(_self, self->len + self->len);
    u_err_if(code != 0);

    self = selfof(*_self);
  }

  strncpy(&self->data[self->len], buf, len);

  self->len += len;
  self->data[self->len] = '\0';

  return 0;

err:
  return -1;
}

ret_t __str_erase(u_str_t* _self, size_t idx, size_t len) {
  str_t* self = selfof(*_self);

  u_assert(idx + len > self->len);
  u_assert(len == 0);

  /*
   * "hello world!"
   *       ^     ^
   * idx = 5
   * len = 6
   * */

  memmove(&self->data[idx], &self->data[idx + len], self->len - idx - len);

  self->len -= len;
  self->data[self->len] = '\0';

  return 0;
}
