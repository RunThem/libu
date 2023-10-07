#include <u/core/head.h>

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct head_t head_t;
struct head_t {
  enum u_head_attr attr;

  size_t itsize;

  size_t len;
  size_t cap;

  u_head_cmp_fn cmp_fn;

  any_t items;
};

#define head_item(head, idx) ((head)->items + (head)->itsize * (idx))

#undef self_of
#define self_of(self) (assert((self) != nullptr), as((self) - sizeof(head_t), head_t*))

static ret_t __head_resize(head_t* self) {
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
 * Create
 *************************************************************************************************/
any_t __head_new(size_t itsize, u_head_cmp_fn cmp_fn, enum u_head_attr attr) {
  head_t* self = nullptr;

  u_assert(itsize == 0);
  u_assert(cmp_fn == nullptr);

  self = u_zalloc(sizeof(head_t) + itsize);
  u_mem_if(self);

  self->items = u_zalloc(itsize * U_HEAD_CAP + 1);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->len    = 0;
  self->cap    = U_HEAD_CAP;
  self->attr   = attr;
  self->cmp_fn = cmp_fn;

  return self + 1;

err:
  u_free_if(self);

  return nullptr;
}

size_t __head_len(any_t _self) {
  return self_of(_self)->len;
}

size_t __head_cap(any_t _self) {
  return self_of(_self)->cap;
}

void __head_pop(any_t _self) {
  head_t* self      = self_of(_self);
  any_t item        = _self;
  ssize_t i         = 0;
  ssize_t child_idx = 0;
  ret_t code        = 0;

  u_assert(self->len == 0);

  memcpy(item, self->items, self->itsize);
  self->len--;
  // memcpy(self->items, head_item(self, self->len - 1), self->itsize);

  for (i = 0; child_idx < self->len; i = child_idx) {
    child_idx = i * 2 + 1; /* child index */

    /* Takes the smaller value in the child node. */
    if (child_idx < self->len) {
      if (self->cmp_fn(head_item(self, child_idx), head_item(self, child_idx + 1)) == 1) {
        child_idx++;
      }
    }

    if (self->cmp_fn(head_item(self, self->len), head_item(self, child_idx)) != 1) {
      break;
    }

    memcpy(head_item(self, i), head_item(self, child_idx), self->itsize);
  }

  memcpy(head_item(self, i), head_item(self, self->len), self->itsize);
}

ret_t __head_push(any_t _self) {
  head_t* self      = self_of(_self);
  any_t item        = _self;
  size_t i          = 0;
  size_t parent_idx = 0;
  ret_t code        = 0;

  if (self->len == self->cap) {
    code = __head_resize(self);
    u_err_if(code != 0);
  }

  for (i = self->len; i > 0; i = parent_idx) {
    parent_idx = (i - 1) / 2; /* parent index */

    if (self->cmp_fn(item, head_item(self, parent_idx)) != -1) {
      break;
    }

    memcpy(head_item(self, i), head_item(self, parent_idx), self->itsize);
  }

  memcpy(head_item(self, i), item, self->itsize);
  self->len++;

  return 0;

err:
  return -1;
}
