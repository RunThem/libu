#include <u/core/head.h>

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct head_t head_t;
struct head_t {
  size_t ksize;
  size_t vsize;

  size_t len;
  size_t cap;

  enum u_head_attr attr;
  u_head_cmp_fn cmp_fn;

  any_t items;
};

#define head_key(head, idx) ((head)->items + ((head)->ksize + (head)->vsize) * (idx))
#define head_val(head, idx)                                                                        \
  ((head)->items + ((head)->ksize + (head)->vsize) * (idx) + (head)->ksize)

#undef self_of
#define self_of(self) (assert((self) != nullptr), as((self) - sizeof(head_t), head_t*))

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __head_new(size_t ksize, size_t vsize, u_head_cmp_fn cmp_fn, enum u_head_attr attr) {
  head_t* self = nullptr;

  u_assert(ksize == 0);
  u_assert(vsize == 0);
  u_assert(cmp_fn == nullptr);

  self = u_zalloc(sizeof(head_t) + ksize + sizeof(any_t) + vsize);
  u_mem_if(self);

  self->items = u_zalloc((ksize + vsize) * U_HEAD_CAP);
  u_mem_if(self->items);

  self->ksize  = ksize;
  self->vsize  = vsize;
  self->len    = 0;
  self->cap    = U_HEAD_CAP;
  self->attr   = attr;
  self->cmp_fn = cmp_fn;

  return self + 1;

err:
  u_free_if(self);

  return nullptr;
}

ret_t __head_push(any_t _self) {
  head_t* self = self_of(_self);

  return 0;
}
