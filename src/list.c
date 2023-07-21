#include "list.h"

typedef struct {
  any_t prev;
  any_t next;
} node_t;

static node_t* __list_node(any_t _self, any_t it, any_t prev, any_t next);

ret_t __list_init(any_t _self, size_t itsize) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(itsize == 0, -1);

  self->itsize = itsize;

  return 0;
}

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

static node_t* __list_node(any_t _self, any_t it, any_t prev, any_t next) {
  node_t* node = nullptr;
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, nullptr);
  u_ret_if(it == nullptr, nullptr);

  node = u_talloc(sizeof(node_t) + self->itsize, node_t*);

  memcpy(any(node) + sizeof(node_t), it, self->itsize);
  node->prev = prev;
  node->next = next;

  return node;
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
  inf("prev(%p), next(%p)", prev, next);

  node = __list_node(_self, it, prev, next);
  u_alloc_if(node);

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
  u_goto_if(node == nullptr);

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