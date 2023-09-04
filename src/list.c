#include "list.h"

/*************************************************************************************************
 * Private type
 *************************************************************************************************/
typedef struct {
  any_t prev;
  any_t next;
} node_t;

/*************************************************************************************************
 * Private function
 *************************************************************************************************/
static node_t* __list_node(any_t _self, any_t it, any_t prev, any_t next) {
  node_t* node = nullptr;
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, nullptr);
  u_ret_if(it == nullptr, nullptr);

  node = u_talloc(sizeof(node_t) + self->itsize, node_t*);
  u_mem_if(node);

  memcpy(any(node) + sizeof(node_t), it, self->itsize);
  node->prev = prev;
  node->next = next;

  return node;

err:
  return nullptr;
}

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __list_new(size_t itsize) {
  list_t* self = nullptr;

  u_ret_if(itsize == 0, nullptr);

  self = u_talloc(sizeof(list_t) + itsize, list_t*);
  u_mem_if(self);

  self->itsize = itsize;
  self->len    = 0;
  self->itor   = nullptr;
  self->head   = nullptr;
  self->tail   = nullptr;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

ret_t __list_init(any_t* _self, size_t itsize) {
  u_ret_if(_self == nullptr, -1);
  u_ret_if(*_self != nullptr, -1);

  *_self = __list_new(itsize);
  u_err_if(*_self == nullptr);

  return 0;

err:
  return -2;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
ret_t __list_clear(any_t _self) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);

  /* success */
  u_ret_if(self->len == 0, 0);

  for (node_t* node = as(self->head, node_t*)->next; node != nullptr; node = node->next) {
    u_free(self->head);
    self->head = node;
  }

  u_free(self->head);

  self->head = self->tail = nullptr;
  self->len               = 0;

  return 0;
}

ret_t __list_cleanup(any_t _self) {
  ret_t code   = 0;
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);

  code = __list_clear(_self);

  self->itsize = 0;

  return code;
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
inline size_t __list_itsize(any_t _self) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);

  return self->itsize;
}

inline size_t __list_len(any_t _self) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, 0);

  return self->len;
}

inline bool __list_empty(any_t _self) {
  return __list_len(_self) == 0;
}

ret_t __list_erase(any_t _self, any_t idx) {
  node_t* node = nullptr;
  node_t* prev = nullptr;
  node_t* next = nullptr;
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx == nullptr, -1);

  u_ret_if(self->len == 0, -1);

  for (node = self->head; node != nullptr && node != idx; node = node->next) { }
  u_err_if(node == nullptr);

  prev = node->prev;
  next = node->next;

  if (prev == nullptr) {
    self->head = next;
  } else {
    prev->next = next;
  }

  if (next == nullptr) {
    self->tail = prev;
  } else {
    next->prev = prev;
  }

  u_free(node);
  self->len--;

  return 0;

err:
  return -2;
}

ret_t __list_push(any_t _self, any_t idx, any_t it) {
  node_t* node = nullptr;
  node_t* prev = nullptr;
  node_t* next = nullptr;
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  prev = idx;
  next = (idx == nullptr) ? self->head : as(idx, node_t*)->next;

  node = __list_node(_self, it, prev, next);
  u_mem_if(node);

  if (prev == nullptr) {
    self->head = node;
  } else {
    as(prev, node_t*)->next = node;
  }

  if (next == nullptr) {
    self->tail = node;
  } else {
    as(next, node_t*)->prev = node;
  }

  self->len++;

  return 0;

err:
  return -2;
}

inline ret_t __list_push_f(any_t _self, any_t it) {
  list_t* self = as(_self, list_t*);

  return __list_push(_self, self->head, it);
}

inline ret_t __list_push_b(any_t _self, any_t it) {
  list_t* self = as(_self, list_t*);

  return __list_push(_self, self->tail, it);
}

ret_t __list_pop(any_t _self, any_t idx, any_t it) {
  node_t* node = nullptr;
  node_t* prev = nullptr;
  node_t* next = nullptr;
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(idx == nullptr, -1);
  u_ret_if(it == nullptr, -1);

  u_ret_if(self->len == 0, -1);

  for (node = self->head; node != nullptr && node != idx; node = node->next) { }
  u_err_if(node == nullptr);

  prev = node->prev;
  next = node->next;

  memcpy(it, any(node) + sizeof(node_t), self->itsize);

  if (prev == nullptr) {
    self->head = next;
  } else {
    prev->next = next;
  }

  if (next == nullptr) {
    self->tail = prev;
  } else {
    next->prev = prev;
  }

  u_free(node);
  self->len--;

  return 0;

err:
  return -2;
}

inline ret_t __list_pop_f(any_t _self, any_t it) {
  list_t* self = as(_self, list_t*);

  return __list_pop(_self, self->head, it);
}

inline ret_t __list_pop_b(any_t _self, any_t it) {
  list_t* self = as(_self, list_t*);

  return __list_pop(_self, self->tail, it);
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
inline any_t __list_head(any_t _self, any_t it) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, nullptr);
  u_ret_if(it == nullptr, nullptr);

  self->itor = self->head;
  u_err_if(self->itor == nullptr);

  memcpy(it, any(self->itor) + sizeof(node_t), self->itsize);

  return self->itor;

err:
  return nullptr;
}

inline any_t __list_tail(any_t _self, any_t it) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, nullptr);
  u_ret_if(it == nullptr, nullptr);

  self->itor = self->tail;
  u_err_if(self->itor == nullptr);

  memcpy(it, any(self->itor) + sizeof(node_t), self->itsize);

  return self->itor;

err:
  return nullptr;
}

inline any_t __list_next(any_t _self, any_t it) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, nullptr);
  u_ret_if(it == nullptr, nullptr);
  u_ret_if(self->itor == nullptr, nullptr);

  self->itor = as(self->itor, node_t*)->next;
  u_err_if(self->itor == nullptr);

  memcpy(it, any(self->itor) + sizeof(node_t), self->itsize);

  return self->itor;

err:
  return nullptr;
}

inline any_t __list_prev(any_t _self, any_t it) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, nullptr);
  u_ret_if(it == nullptr, nullptr);
  u_ret_if(self->itor == nullptr, nullptr);

  self->itor = as(self->itor, node_t*)->prev;
  u_err_if(self->itor == nullptr);

  memcpy(it, any(self->itor) + sizeof(node_t), self->itsize);

  return self->itor;

err:
  return nullptr;
}

/*************************************************************************************************
 * Utils
 *************************************************************************************************/
any_t __list_find(any_t _self, any_t it, eq_fn fn) {
  node_t* node = nullptr;
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, nullptr);
  u_ret_if(it == nullptr, nullptr);
  u_ret_if(fn == nullptr, nullptr);

  u_ret_if(self->len == 0, nullptr);

  for (node = self->head; node != nullptr; node = node->next) {
    if (fn(any(node) + sizeof(node_t), it)) {
      return node;
    }
  }

  return nullptr;
}
