#include <u/core/heap.h>

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct heap_t heap_t;
struct heap_t {
  enum u_heap_attr attr;

  size_t itsize;

  size_t len;
  size_t cap;

  u_heap_cmp_fn cmp_fn;

  any_t items;
};

#undef at
#define at(idx) (self->items + self->itsize * (idx))

#undef selfof
#define selfof(self) (assert((self) != nullptr), as((self) - sizeof(heap_t), heap_t*))

static ret_t __heap_resize(heap_t* self) {
  size_t cap  = 0;
  any_t items = nullptr;

  cap = (self->cap < 1024) ? self->cap * 2 : self->cap + 512;

  items = u_realloc(self->items, self->itsize * cap);
  u_mem_if(items);

  infln("heap resize(cap(%zu -> %zu))", self->cap, cap);

  self->items = items;
  self->cap   = cap;

  return 0;

err:
  return -1;
}

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __heap_new(size_t itsize, u_heap_cmp_fn cmp_fn, enum u_heap_attr attr) {
  heap_t* self = nullptr;

  u_check_ret(itsize == 0, nullptr);
  u_check_ret(cmp_fn == nullptr, nullptr);
  u_check_ret(attr != U_HEAP_MIN || attr != U_HEAP_MAX, nullptr);

  self = u_zalloc(sizeof(heap_t) + itsize);
  u_mem_if(self);

  self->items = u_zalloc(itsize * U_HEAP_CAP + 1);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->len    = 0;
  self->cap    = U_HEAP_CAP;
  self->attr   = attr;
  self->cmp_fn = cmp_fn;

  infln("heap new(itsize(%zu), cmp_fn(%p), attr(%s))",
        itsize,
        cmp_fn,
        attr == U_HEAP_MIN ? "min" : "max");

  return self + 1;

err:
  u_free_if(self);

  return nullptr;
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __heap_itsize(any_t _self) {
  return selfof(_self)->itsize;
}

size_t __heap_len(any_t _self) {
  return selfof(_self)->len;
}

size_t __heap_cap(any_t _self) {
  return selfof(_self)->cap;
}

bool __heap_empty(any_t _self) {
  return selfof(_self)->len == 0;
}

void __heap_peek(any_t _self) {
  heap_t* self = selfof(_self);
  any_t item   = _self;

  u_check_nret(self->len == 0);

  memcpy(item, self->items, self->itsize);
}

void __heap_pop(any_t _self) {
  heap_t* self      = selfof(_self);
  any_t item        = _self;
  ssize_t i         = 0;
  ssize_t child_idx = 0;

  u_check_nret(self->len == 0);

  memcpy(item, self->items, self->itsize);
  self->len--;

  for (i = 0; child_idx < self->len; i = child_idx, child_idx = i * 2 + 1) {
    /* Takes the smaller value in the child node. */
    if (child_idx < self->len) {
      if (self->cmp_fn(at(child_idx), at(child_idx + 1)) == ((self->attr == U_HEAP_MIN) ? 1 : -1)) {
        child_idx++;
      }
    }

    if (self->cmp_fn(at(self->len), at(child_idx)) != ((self->attr == U_HEAP_MIN) ? 1 : -1)) {
      break;
    }

    memcpy(at(i), at(child_idx), self->itsize);
  }

  memcpy(at(i), at(self->len), self->itsize);
}

ret_t __heap_push(any_t _self) {
  heap_t* self      = selfof(_self);
  any_t item        = _self;
  size_t i          = 0;
  size_t parent_idx = 0;
  ret_t code        = 0;

  if (self->len == self->cap) {
    code = __heap_resize(self);
    u_err_if(code != 0, "heap push() resize failed.");
  }

  for (i = self->len; i > 0; i = parent_idx) {
    parent_idx = (i - 1) / 2;

    if (self->cmp_fn(item, at(parent_idx)) != ((self->attr == U_HEAP_MIN) ? -1 : 1)) {
      break;
    }

    memcpy(at(i), at(parent_idx), self->itsize);
  }

  memcpy(at(i), item, self->itsize);

  self->len++;

  return 0;

err:
  return -1;
}
