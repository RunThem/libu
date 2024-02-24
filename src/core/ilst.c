#include <u/core.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct lstn_t lstn_t;
struct lstn_t {
  size_t prev;
  size_t next;
  any_t ptr;
};

typedef struct lst_t lst_t;
struct lst_t {
  size_t len;

  uvec(lstn_t) items;
};

/***************************************************************************************************
 * Function
 **************************************************************************************************/
u_lst_t lst_new() {
  lst_t* self = nullptr;

  self = u_zalloc(sizeof(lst_t));
  u_mem_if(self);

  self->len = 0;

  infln("lst new(itsize(%zu))", off);

  return as(self, u_lst_t);

err:
  return nullptr;
}

#if 0


/***************************************************************************************************
 * Macro
 **************************************************************************************************/

/***************************************************************************************************
 * Function
 **************************************************************************************************/
u_lst_t lst_new(size_t off) {
  lst_t* self = nullptr;

  self = u_zalloc(sizeof(lst_t) + off);
  u_mem_if(self);

  self->off = off;
  self->len = 0;

  infln("lst new(itsize(%zu))", off);

  return as(self, u_lst_t);

err:
  return nullptr;
}

void lst_cleanup(u_lst_t _self) {
  lst_t* self = as(_self, lst_t*);

  u_check_nret(self == nullptr);

  u_free(self);
}

size_t lst_len(u_lst_t _self) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, 0);

  return self->len;
}

bool lst_isexist(u_lst_t _self, any_t idx) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, false);
  u_check_ret(idx == nullptr, false);
  u_check_ret(self->len == 0, false);

  for (auto node = self->head; node != nullptr; node = node->next) {
    if (node->item == idx) {
      return true;
    }
  }

  return false;
}

void lst_pop(u_lst_t _self, any_t idx, any_t item) {
  lst_t* self  = as(_self, lst_t*);
  lstn_t* node = nullptr;
  lstn_t* prev = nullptr;
  lstn_t* next = nullptr;

  u_check_nret(self == nullptr);
  u_check_nret(idx == nullptr || !lst_isexist(_self, idx));
  u_check_nret(self->len == 0);

  node = as(idx + self->off, lstn_t*);

  if (self->head == idx) {
    self->head = node->next;

    if (self->head != nullptr) {
      self->head->prev = nullptr;
    }
  } else if (self->tail == idx) {
    self->tail = node->prev;

    if (self->tail != nullptr) {
      self->tail->next = nullptr;
    }
  } else {
    node->prev->next = node->next;
    node->next->prev = node->prev;
  }

  node->prev = node->next = nullptr;

  self->len--;

  *as(item, any_t*) = idx;
}

void lst_put(u_lst_t _self, any_t idx, any_t item) {
  lst_t* self  = as(_self, lst_t*);
  lstn_t* node = nullptr;
  lstn_t* prev = nullptr;
  lstn_t* next = nullptr;

  u_check_nret(self == nullptr);
  u_check_nret(item == nullptr);
  u_check_nret(idx != nullptr && !lst_isexist(_self, idx));

  /*
   * <- {a} <-> {b} <-> {c} ->
   *     ^
   *
   * <- {a} <-> {x} <-> {b} <-> {c} ->
   * */

  node       = as(item + self->off, lstn_t*);
  node->item = item;

  if (idx == nullptr) {
    prev = nullptr;
    next = self->head;
  } else {
    prev = as(idx + self->off, lstn_t*);
    next = as(idx + self->off, lstn_t*)->next;
  }

  node->prev = prev;
  node->next = next;

  if (prev == nullptr) {
    self->head = node;
  } else {
    prev->next = node;
  }

  if (next == nullptr) {
    self->tail = node;
  } else {
    next->prev = node;
  }

  self->len++;
}

void lst_first(u_lst_t _self, any_t item) {
  lst_t* self = as(_self, lst_t*);

  u_check_nret(self == nullptr);
  u_check_nret(self->len == 0);

  *as(item, any_t*) = self->head->item;
}

void lst_last(u_lst_t _self, any_t item) {
  lst_t* self = as(_self, lst_t*);

  u_check_nret(self == nullptr);
  u_check_nret(self->len == 0);

  *as(item, any_t*) = self->tail->item;
}

void lst_next(u_lst_t _self, any_t idx, any_t item) {
  lst_t* self = as(_self, lst_t*);

  u_check_nret(self == nullptr);
  u_check_nret(self->len == 0);
  u_check_nret(idx == nullptr);
  u_check_nret(*as(idx, any_t*) == nullptr);

  *as(item, any_t*) = as(*as(idx, any_t*) + self->off, lstn_t*)->next;
}

void lst_prev(u_lst_t _self, any_t idx, any_t item) {
  lst_t* self = as(_self, lst_t*);

  u_check_nret(self == nullptr);
  u_check_nret(self->len == 0);
  u_check_nret(idx == nullptr);
  u_check_nret(*as(idx, any_t*) == nullptr);

  *as(item, any_t*) = as(*as(idx, any_t*) + self->off, lstn_t*)->prev;
}

bool lst_for_init(u_lst_t _self, bool flag) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, false);
  u_check_ret(self->len == 0, false);

  self->flags[0] = !self->flags[0];
  self->flags[1] = flag;
  self->flags[2] = true;
  self->flags[3] = false;

  self->iter = nullptr;

  return self->flags[0];
}

bool lst_for(u_lst_t _self, any_t idx, any_t item) {
  lst_t* self  = as(_self, lst_t*);
  lstn_t* node = nullptr;

  u_check_ret(self == nullptr, false);
  u_check_ret(self->flags[3], false);

  if (self->flags[2]) {
    self->iter     = self->flags[1] ? self->head : self->tail;
    self->flags[2] = !self->flags[2];
  } else {
    self->iter = self->flags[1] ? self->iter->next : self->iter->prev;
  }

  if ((self->flags[1] && self->iter == self->tail) ||
      (!self->flags[1] && self->iter == self->head)) {
    self->flags[3] = true;
  }

  *as(idx, any_t*)  = self->iter->item;
  *as(item, any_t*) = self->iter->item;

  return true;
}
#endif
