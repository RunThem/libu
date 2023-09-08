#include "vec.h"

/*
 *
 * 0        8        16       24       32       40
 * +--------+--------+--------+--------+--------+--------------+
 * | itsize |  len   |  cap   | items  |  item  |     ....     |
 * +--------+--------+--------+--------+--------+--------------+
 *                                |    ^   T*       sizeof(T)
 *                                |    |
 *                                |    +
 *                                |  return point
 *                                |
 *                                |
 *                                |
 *                                |      +-----+-----+-----+
 *                                +----->+  T  |  T  |  T  |...
 *                                       +-----+-----+-----+
 *
 * @param
 * itsize
 *      memory size of the element.
 * len
 *      number of elements in the vector.
 * cap
 *      number of elements the vector can hold.
 * items
 *      pointer to the underlying array.
 * item
 *      iterator pointer, which can only be used in `vec_for() & vec_rfor()`, is also the only field
 *      exposed to the outside world(holds the type of the vector element -- C23.typeof()).
 * item
 *      buffer used to pass element data when an element is deposited
 *
 * @code
 * ```
 * int main(int argc, const char** argv) {
 *   vec(int) vv = nullptr;
 *
 *   vv = vec_new(int, 64);
 *
 *   for (size_t i = 1; i <= 100; i++) {
 *     vec_push_back(vv, i);
 *   }
 *
 *   size_t sum = 0;
 *   vec_for(vv, i) {
 *     sum += *vv->item;
 *   }
 *
 *   printf("sum is %zu\n", sum);
 *   // output: sum is 5050
 *
 *   vec_for(vv, i) {
 *     *vv->item = *vv->item * 2;
 *   }
 *
 *   sum = 0;
 *   vec_for(vv, i) {
 *     inf("%d", *vv->item);
 *     sum += *vv->item;
 *   }
 *
 *   printf("sum is %zu\n", sum);
 *   // output: sum is 10100
 *
 *   vec_cleanup(vv);
 *
 *   return 0;
 * }
 * ```
 * */

/*************************************************************************************************
 * Private
 *************************************************************************************************/
typedef struct {
  size_t itsize;
  size_t len;
  size_t cap;
  any_t items;
  any_t item;
} vec_t;

#define vec_of(self) (assert(self != nullptr), as(container_of(self, vec_t, item), vec_t*))

static ret_t __vec_resize(vec_t* self) {
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
 * Create & Clone
 *************************************************************************************************/
any_t __vec_new(size_t itsize, size_t cap) {
  vec_t* self = nullptr;

  u_assert(itsize == 0);

  self = u_talloc(sizeof(vec_t) + itsize, vec_t*);
  u_mem_if(self);

  cap = (cap < U_VEC_CAP) ? U_VEC_CAP : cap;

  self->items = u_zalloc(itsize * cap);
  u_mem_if(self->items);

  self->itsize = itsize;
  self->cap    = cap;

  return &self->item;

err:
  u_free_if(self);

  return nullptr;
}

any_t __vec_clone(any_t _self) {
  vec_t* self = vec_of(_self);
  vec_t* vec  = nullptr;

  vec = __vec_new(self->itsize, self->cap);
  u_mem_if(vec);

  vec = container_of(vec, vec_t, item);

  memcpy(vec->items, self->items, self->itsize * self->len);
  vec->len = self->len;

  return &vec->item;

err:
  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __vec_clear(any_t _self) {
  vec_of(_self)->len = 0;
}

void __vec_cleanup(any_t _self) {
  vec_t* self = vec_of(_self);

  u_free_if(self->items);
  u_free_if(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __vec_itsize(any_t _self) {
  return vec_of(_self)->itsize;
}

size_t __vec_len(any_t _self) {
  return vec_of(_self)->len;
}

size_t __vec_cap(any_t _self) {
  return vec_of(_self)->cap;
}

bool __vec_empty(any_t _self) {
  return vec_of(_self)->len == 0;
}

any_t __vec_at(any_t _self, size_t idx) {
  vec_t* self = vec_of(_self);

  u_assert(idx >= self->len);

  return self->items + idx * self->itsize;
}

void __vec_pop(any_t _self, size_t idx) {
  vec_t* self = vec_of(_self);
  any_t point = nullptr;

  u_assert(self == nullptr);
  u_noret_if(idx >= self->len);

  if (idx != self->len - 1) {
    point = self->items + self->itsize * idx;
    memmove(point, point + self->itsize, self->itsize * (self->len - idx - 1));
  }

  self->len--;
}

ret_t __vec_push(any_t _self, size_t idx) {
  vec_t* self = vec_of(_self);
  any_t item  = self + 1;
  any_t items = nullptr;
  ret_t code  = 0;

  u_ret_if(idx > self->len, -1);

  if (self->len == self->cap) {
    code = __vec_resize(self);
    u_err_if(code != 0);
  }

  items = self->items + self->itsize * idx;
  if (idx != self->len) {
    memmove(items + self->itsize, items, self->itsize * (self->len - idx));
  }

  memcpy(items, item, self->itsize);
  self->len++;

  return 0;

err:
  return -2;
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __vec_range(any_t _self, bool flag) {
  vec_t* self = vec_of(_self);
  any_t item  = _self;

  /* initialize */
  if (self->item == nullptr) {
    self->item = flag ? self->items : self->items + (self->len - 1) * self->itsize;
  } else { /* iteration */
    self->item += flag ? self->itsize : -self->itsize;
  }

  /* whether it is out of range */
  u_ret_if(self->item == self->items + self->len * self->itsize, false);
  u_ret_if(self->item == self->items - self->itsize, false);

  return true;
}

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
void __vec_sort(any_t _self, cmp_fn fn) {
  vec_t* self = vec_of(_self);

  u_assert(fn == nullptr);

  qsort(self->items, self->len, self->itsize, fn);
}

ssize_t __vec_find(any_t _self, eq_fn fn) {
  vec_t* self = vec_of(_self);
  any_t item  = _self;

  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * i, item)) {
      return i;
    }
  }

  return -2;
}

ssize_t __vec_min(any_t _self, cmp_fn fn) {
  vec_t* self = vec_of(_self);
  ssize_t idx = 0;

  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * idx, self->items + self->itsize * i) == 1) {
      idx = i;
    }
  }

  return idx;
}

ssize_t __vec_max(any_t _self, cmp_fn fn) {
  vec_t* self = vec_of(_self);
  ssize_t idx = 0;

  u_assert(fn == nullptr);

  for (ssize_t i = 0; i < self->len; i++) {
    if (fn(self->items + self->itsize * idx, self->items + self->itsize * i) == -1) {
      idx = i;
    }
  }

  return idx;
}
