/* libs */
// #include "src/buf.h"
// #include "src/fs.h"
// #include "src/list.h"
// #include "src/map.h"
// #include "src/sock.h"
// #include "src/str.h"
#include "src/u.h"
// #include "src/vec.h"

#define _typeof(t) __builtin_classify_type(t)

typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
} vec_t;

#undef vec
#define vec(T)                                                                                     \
  struct {                                                                                         \
    vec_t _;                                                                                       \
    T it;                                                                                          \
  }

void vec_init(any_t _self, size_t itsize, size_t cap) {
  vec_t* self = as(_self, vec_t*);

  self->itsize = itsize;
  self->len    = 0;
  self->cap    = cap;

  self->items = u_calloc(cap, itsize);
};

void vec_push(any_t _self, any_t it) {
  vec_t* self = as(_self, vec_t*);

  memcpy(self->items + self->itsize * self->len, it, self->itsize);
  self->len++;
}

void vec_pop(any_t _self) {
  vec_t* self = as(_self, vec_t*);

  self->len--;
  memcpy(_self + sizeof(vec_t), self->items + self->itsize * self->len, self->itsize);
}

void print(void* ptr) {
  inf("%d", *as(ptr, int*));
}

struct st {
  int a;
  char b;
  long d;
};

fn_eq_def(struct_st, struct st, (x.a == y.a));
fn_cmp_def(struct_st, struct st, (x.a > y.a));

int main(int argc, const char** argv) {

  return 0;
}
