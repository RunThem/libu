#include <stdarg.h>
#include <stdatomic.h>
#include <threads.h>

/* libs */
#include "core/avl.h"
#include "core/str.h"

typedef struct {
  size_t cap;
  size_t len;
  char str[];
} str_t;

#define self_of(self) (assert((self) != nullptr), as(container_of(self, str_t, str), str_t*))

static u_avl_t(u_str_t) stbl  = nullptr;
static atomic_flag flag       = ATOMIC_FLAG_INIT;
static once_flag call_init    = ONCE_FLAG_INIT;
static once_flag call_cleanup = ONCE_FLAG_INIT;

#define stbl_lock()                                                                                \
  while (atomic_flag_test_and_set(&flag)) { }
#define stbl_unlock() atomic_flag_clear(&flag)

#define stbl_exist(str) u_avl_exist(stbl, str)
#define stbl_add(str)   u_avl_push(stbl, str);
#define stbl_del(str)   u_avl_pop(stbl, str);

fn_eq_def(stbl_str, u_str_t, x == y);
fn_cmp_def(stbl_str, u_str_t, x > y);

static void __stbl_init(void) {
  stbl = u_avl_new(u_str_t, fn_cmp_use(stbl_str));
}

static void __stbl_cleanup(void) {
  u_avl_cleanup(stbl);
}

void stbl_init(void) {
  call_once(&call_init, __stbl_init);
}

void stbl_cleanup(void) {
  call_once(&call_cleanup, __stbl_cleanup);
}

static str_t* str_new(size_t cap) {
  str_t* self = nullptr;

  cap  = cap < 16 ? 16 : cap < 10240 ? cap * 2 : cap + 1024;
  self = u_zalloc(sizeof(str_t) + cap + 1);
  u_mem_if(self);

  self->len = 0;
  self->cap = cap;

  stbl_lock();
  stbl_add(self->str);
  stbl_unlock();

  return self;

err:
  return nullptr;
}

u_str_t str_from(u_str_t str) {
  str_t* self = nullptr;
  size_t len  = 0;

  u_assert(str == nullptr);

  stbl_lock();
  len = stbl_exist(str) ? self_of(str)->len : strlen(str); /* long time */
  stbl_unlock();

  self = str_new(len);
  u_mem_if(self);

  self->len = len;
  strncpy(self->str, str, len);

  return self->str;

err:
  return nullptr;
}

u_str_t str_fromf(u_str_t fmt, ...) {
  str_t* self    = nullptr;
  char buf[4096] = {0};
  va_list ap     = {0};
  size_t len     = 0;
  ret_t code     = 0;

  u_assert(fmt == nullptr);

  va_start(ap, format);
  len = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  self = str_new(len);
  u_mem_if(self);

  self->len = len;
  strncpy(self->str, buf, len);

  return self->str;

err:
  return nullptr;
}

void str_clear(u_str_t str) {
  u_assert(str == nullptr);
}

void str_cleanup(u_str_t str) {
  u_assert(str == nullptr);

  stbl_lock();

  if (stbl_exist(str)) {
    stbl_del(str);

    u_free_if(self_of(str));
  }

  stbl_unlock();
}

u_str_t str_append(u_str_t self, u_str_t str) {
  str_t* nstr = nullptr;
  size_t cap  = 0;
  size_t len  = 0;
  ret_t code  = 0;

  cap = self_of(self)->cap;

  stbl_lock();
  len = stbl_exist(str) ? self_of(str)->len : strlen(str); /* long time */
  stbl_unlock();

  if (cap - self_of(self)->len > len) {
    strncpy((u_mut_str_t)self + self_of(self)->len, str, len);
  } else {
    nstr = str_new(self_of(self)->len + len);
  }

  return 0;
}

#if 0
struct str_t {
  size_t cap;
  size_t len;
  c_str c_str;
};

/* clang-format off */
size_t __str_char_size(char c)     { return sizeof(char); }
size_t __str_c_str_size(c_str s)   { return strlen(s); }
size_t __str_string_size(str_t s)  { return s.len; }

c_str  __str_char_start(char* s)    { return s; }
c_str  __str_c_str_start(c_str * s) { return *s; }
c_str  __str_string_start(str_t* s) { return (*s).c_str; }
/* clang-format on */

static size_t __str_cap(size_t size) {
  if (size < 16) {
    return 16;
  } else if (size < 1024) {
    return size * 2;
  }

  return size + 1024;
}

str_t __str_from(c_str cstr, size_t len) {
  struct str_t* self = nullptr;

  u_assert(cstr == nullptr);

  self = u_talloc(sizeof(struct str_t), struct str_t*);
  u_mem_if(self);

  __str_push(&self, 0, cstr, len);

  return as(&self->len, str_t);

err:
  return nullptr;
}

str_t __str_fromf(c_str fmt, ...) {
  ret_t code     = 0;
  str_t self     = {0};
  char buf[4096] = {0};
  size_t len     = 0;
  va_list ap     = {0};

  u_assert(fmt == nullptr);

  code = __str_init(&self, 0);
  u_err_if(code != 0);

  va_start(ap, fmt);
  len = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  __str_push(&self, 0, buf, len);

  return self;

err:
  bzero(&self, sizeof(str_t));

  return self;
}

ret_t __str_resize(any_t _self, size_t cap) {
  c_str ptr   = nullptr;
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->cap >= cap, -1);

  ptr = u_realloc(self->c_str, sizeof(char) * cap);
  u_mem_if(ptr);

  self->c_str = ptr;
  self->cap   = cap;

  return 0;

err:
  return -2;
}

str_t __str_clone(any_t _self) {
  ret_t code  = 0;
  str_t str   = {0};
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, str);

  code = __str_init(&str, self->len);
  u_err_if(code != 0);

  memcpy(str.c_str, self->c_str, self->len);

  str.len = self->len;

  return str;

err:
  str.len = 0;
  str.cap = 0;

  return str;
}

ret_t __str_cleanup(any_t _self) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);

  u_free_if(self->c_str);

  bzero(self, sizeof(str_t));

  return 0;
}

char __str_at(any_t _self, size_t idx) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, '\0');
  u_ret_if(self->len <= idx, '\0');

  u_ret_if(self->c_str == nullptr, '\0');

  return self->c_str[idx];
}

ret_t __str_push(any_t _self, size_t idx, c_str cstr, size_t len) {
  ret_t code  = 0;
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx > self->len, -1);
  u_ret_if(cstr == nullptr, -1);
  u_ret_if(len == 0, -1);

  if (self->cap - self->len < len) {
    code = __str_resize(self, __str_cap(self->cap + len));
    u_err_if(code != 0);
  }

  if (idx < self->len) {
    memmove(self->c_str + idx + len, self->c_str + idx, self->len - idx);
  }

  memcpy(self->c_str + idx, cstr, len);
  self->len += len;

  return 0;

err:
  return -2;
}

ret_t __str_pushf(any_t _self, size_t idx, c_str fmt, ...) {
  ret_t code     = 0;
  char buf[4096] = {0};
  size_t len     = 0;
  str_t* self    = as(_self, str_t*);
  va_list ap     = {0};

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx > self->len, -1);
  u_ret_if(fmt == nullptr, -1);

  va_start(ap, fmt);
  len = vsnprintf(buf, sizeof(buf), fmt, ap);
  va_end(ap);

  if (self->cap - self->len < len) {
    code = __str_resize(self, __str_cap(self->cap + len));
    u_err_if(code != 0);
  }

  if (idx < self->len) {
    memmove(self->c_str + idx + len, self->c_str + idx, self->len - idx);
  }

  memcpy(self->c_str + idx, buf, len);
  self->len += len;

  return 0;

err:
  return -2;
}

ret_t __str_erase(any_t _self, size_t idx, size_t len) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx >= self->len, -1);
  u_ret_if(len + idx > self->len, -1);

  memcpy(self->c_str + idx, self->c_str + idx + len, self->len - idx - len);

  self->c_str[self->len - len] = '\0';
  self->len -= idx + len;

  return 0;
}

bool __str_cmp(any_t _self, c_str cstr, size_t len) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, false);
  u_ret_if(cstr == nullptr, false);
  u_ret_if(len != self->len, false);

  return !strncmp(self->c_str, cstr, len);
}

ssize_t __str_find(any_t _self, c_str cstr, size_t len) {
  any_t pos   = nullptr;
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(cstr == nullptr, -1);
  u_ret_if(len > self->len, -1);

  pos = strstr(self->c_str, cstr);
  u_err_if(pos == nullptr);

  return pos - any(self->c_str);

err:
  return -1;
}

bool __str_prefix(any_t _self, c_str cstr, size_t len) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, false);
  u_ret_if(cstr == nullptr, false);
  u_ret_if(len == 0, false);

  return !strncmp(self->c_str, cstr, len);
}

bool __str_subfix(any_t _self, c_str cstr, size_t len) {
  str_t* self = as(_self, str_t*);

  u_ret_if(_self == nullptr, false);
  u_ret_if(cstr == nullptr, false);
  u_ret_if(len == 0, false);

  return !strncmp(self->c_str + self->len - len, cstr, len);
}

size_t __str_count(any_t _self, c_str cstr, size_t len) {
  size_t count = 0;
  c_str pos    = nullptr;
  c_str tmp    = nullptr;
  str_t* self  = as(_self, str_t*);

  u_ret_if(_self == nullptr, 0);
  u_ret_if(cstr == nullptr, 0);
  u_ret_if(len == 0, 0);
  u_ret_if(len > self->len, 0);

  pos = self->c_str;

  while (true) {
    tmp = strstr(pos, cstr);
    if (tmp == nullptr) {
      break;
    }

    pos = tmp + 1;
    count++;
  }

  return count;
}

#  if 0

bool __str_contain(str_t* str, c_str c_string, size_t len) {
  return __str_find(str, c_string, len, 0) != -1;
}

str_t __str_cut(str_t* str, size_t idx, size_t len) {
  str_t sub_str = nullptr;

  u_ret_if(str == nullptr, nullptr);
  u_ret_if(_str == nullptr, nullptr);
  u_ret_if(idx >= _str->len, nullptr);
  u_ret_if(idx + len > _str->len, nullptr);

  if (len == 0) {
    sub_str = __str_new(&_str->c_str[idx], _str->len - idx);
    u_err_if(sub_str == nullptr);

    __str_erase(str, idx, -2);
  } else {
    sub_str = __str_new(&_str->c_str[idx], len);
    u_err_if(sub_str == nullptr);

    __str_erase(str, idx, (ssize_t)len);
  }

  return sub_str;

err:
  return nullptr;
}

int __str_repeat(str_t* str, size_t count) {
  int ret = 0;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(count <= 1, -1);

  ret = __str_resize(str, str_cap(_str->len * count));
  u_err_if(ret != 0);

  for (size_t i = 0; i < count; i++) {
    strncpy(&_str->c_str[i * _str->len], _str->c_str, _str->len);
  }

  _str->len *= count;

  return 0;

err:
  return -1;
}

int __str_replace(str_t* str,
                  c_str src_string,
                  size_t src_len,
                  c_str dst_string,
                  size_t dst_len,
                  size_t count) {
  c_str p       = nullptr;
  size_t idx    = 0;
  size_t _count = 0;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(src_string == nullptr, -1);
  u_ret_if(src_len <= 0, -1);
  u_ret_if(dst_string == nullptr, -1, );
  u_ret_if(dst_len <= 0, -1);
  u_ret_if(count < 0, -1);

  while (count == 0 || _count < count) {
    p = strstr(&_str->c_str[idx], src_string);
    if (p == nullptr) {
      break;
    }

    idx = p - _str->c_str;

    __str_erase(str, idx, (ssize_t)src_len);
    __str_insert(str, idx, dst_string, dst_len);

    _count++;
    idx += dst_len;
  }

  return _count == count;
}

int __str_2lower(str_t* str) {
  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);

  for (size_t i = 0; i < _str->len; i++) {
    _str->c_str[i] = chr(tolower(_str->c_str[i]));
  }

  return 0;
}

int __str_2upper(str_t* str) {
  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);

  for (size_t i = 0; i < _str->len; i++) {
    _str->c_str[i] = chr(toupper(_str->c_str[i]));
  }

  return 0;
}

int __str_trim(str_t* str, c_str c_string, size_t len) {
  c_str ptr = nullptr;

  u_ret_if(str == nullptr, -1);
  u_ret_if(_str == nullptr, -1);
  u_ret_if(c_string == nullptr, -1);
  u_ret_if(len <= 0, -1);

  while (true) {
    ptr = strpbrk(_str->c_str, c_string);
    if (ptr == nullptr) {
      break;
    }

    __str_erase(str, ptr - _str->c_str, 1);
  }

  return 0;
}

#  endif

#endif
