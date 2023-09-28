#include <u/core/list.h>

typedef struct list_node_t list_node_t;
struct list_node_t {
  list_node_t* prev;
  list_node_t* next;
};

typedef struct list_t list_t;
struct list_t {
  size_t len;
  size_t off;

  list_node_t* head;
  list_node_t* tail;

  list_node_t* itor;
};

#undef self_of
#define self_of(self) (assert((self) != nullptr), as((self) - sizeof(list_t), list_t*))

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __list_new(size_t off) {
  list_t* self = nullptr;

  self = u_zalloc(sizeof(list_t) + sizeof(any_t));
  u_mem_if(self);

  self->len  = 0;
  self->off  = off;
  self->head = nullptr;
  self->tail = nullptr;

  return self + 1;

err:
  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __list_cleanup(any_t _self) {
  list_t* self = self_of(_self);

  u_free_if(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __list_len(any_t _self) {
  return self_of(_self)->len;
}

bool __list_empty(any_t _self) {
  return self_of(_self)->len == 0;
}

void __list_pop(any_t _self, any_t idx) {
  list_t* self          = self_of(_self);
  list_node_t* node     = nullptr;
  list_node_t* idx_node = nullptr;

  u_assert(idx == nullptr);
  u_assert(self->len == 0);

  idx_node = idx + self->off;

  u_assert(idx_node->next == nullptr && self->tail != idx_node);

  for (node = self->head; node != nullptr; node = node->next) {
    if (node == idx_node) {
      break;
    }
  }

  u_assert(node == nullptr);

  if (self->head == idx_node) {
    self->head = idx_node->next;

    if (self->head != nullptr) {
      self->head->prev = nullptr;
    }
  } else if (self->tail == idx_node) {
    self->tail = idx_node->prev;

    if (self->tail != nullptr) {
      self->tail->next = nullptr;
    }
  } else {
    idx_node->prev->next = idx_node->next;
    idx_node->next->prev = idx_node->prev;
  }

  idx_node->prev = nullptr;
  idx_node->next = nullptr;

  self->len--;
}

void __list_pop_front(any_t _self) {
  list_t* self = self_of(_self);

  u_assert(self->len == 0);

  __list_pop(_self, as(self->head, any_t) - self->off);
}

void __list_pop_back(any_t _self) {
  list_t* self = self_of(_self);

  u_assert(self->len == 0);

  __list_pop(_self, as(self->tail, any_t) - self->off);
}

void __list_push(any_t _self, any_t idx, any_t item) {
  list_t* self           = self_of(_self);
  list_node_t* idx_node  = nullptr;
  list_node_t* item_node = nullptr;

  u_assert(item == nullptr);

  item_node = item + self->off;

  if (idx == nullptr) {
    item_node->prev = nullptr;
    item_node->next = self->head;

    self->head = item_node;
  } else {
    idx_node = idx + self->off;
    u_assert(self->len == 0);

    item_node->prev = idx_node;
    item_node->next = idx_node->next;

    idx_node->next = item_node;
  }

  if (item_node->next == nullptr) {
    self->tail = item_node;
  } else {
    if (idx != nullptr) {
      idx_node->next->prev = idx_node;
    }
  }

  self->len++;
}

void __list_push_front(any_t _self, any_t item) {
  __list_push(_self, nullptr, item);
}

void __list_push_back(any_t _self, any_t item) {
  list_t* self = self_of(_self);

  if (self->len == 0) {
    __list_push(_self, nullptr, item);
  } else {
    __list_push(_self, as(self->tail, any_t) - self->off, item);
  }
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __list_range(any_t _self, bool flag) {
  list_t* self       = self_of(_self);
  list_node_t** item = _self;

  u_ret_if(self->len == 0, false);

  if (*item == nullptr) {
    self->itor = (flag ? self->head : self->tail);
  } else {
    self->itor = (flag ? self->itor->next : self->itor->prev);
  }

  if (self->itor == nullptr) {
    return false;
  }

  *item = as(self->itor, any_t) - self->off;

  return true;
}
