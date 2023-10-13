#include <u/core/list.h>

typedef struct u_list_node_t list_node_t;

typedef struct list_t list_t;
struct list_t {
  size_t len;
  size_t off;

  list_node_t* head;
  list_node_t* tail;

  list_node_t* itor;
};

#undef selfof
#define selfof(self) (assert((self) != nullptr), as((self) - sizeof(list_t), list_t*))

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

  infln("list new(off(%zu))", off);

  return self + 1;

err:
  return nullptr;
}

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __list_cleanup(any_t _self) {
  list_t* self = selfof(_self);

  u_free(self);
}

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __list_len(any_t _self) {
  return selfof(_self)->len;
}

bool __list_empty(any_t _self) {
  return selfof(_self)->len == 0;
}

void __list_pop(any_t _self, any_t idx) {
  list_t* self          = selfof(_self);
  list_node_t* node     = nullptr;
  list_node_t* idx_node = nullptr;

  u_check_nret(idx == nullptr);
  u_check_nret(self->len == 0);

  idx_node = idx + self->off;

  u_check_nret(idx_node->next == nullptr && self->tail != idx_node,
               "node is not in a linked list.");

  for (node = self->head; node != nullptr; node = node->next) {
    if (node == idx_node) {
      break;
    }
  }

  u_check_nret(node == nullptr, "node is not in a linked list.");

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
  list_t* self = selfof(_self);

  u_check_nret(self->head == nullptr);

  __list_pop(_self, as(self->head, any_t) - self->off);
}

void __list_pop_back(any_t _self) {
  list_t* self = selfof(_self);

  u_check_nret(self->tail == nullptr);

  __list_pop(_self, as(self->tail, any_t) - self->off);
}

void __list_push(any_t _self, any_t idx, any_t item) {
  list_t* self           = selfof(_self);
  list_node_t* idx_node  = nullptr;
  list_node_t* item_node = nullptr;
  list_node_t* prev_node = nullptr;
  list_node_t* next_node = nullptr;

  u_check_nret(item == nullptr);

  item_node = item + self->off;
  idx_node  = idx + self->off;

  if (idx != nullptr) {
    prev_node = idx + self->off;
  }

  next_node = (prev_node != nullptr) ? prev_node->next : self->head;

  item_node->prev = prev_node;
  item_node->next = next_node;

  if (prev_node != nullptr) {
    prev_node->next = item_node;
  } else {
    self->head = item_node;
  }

  if (next_node != nullptr) {
    next_node->prev = item_node;
  } else {
    self->tail = item_node;
  }

  self->len++;
}

void __list_push_front(any_t _self, any_t item) {
  __list_push(_self, nullptr, item);
}

void __list_push_back(any_t _self, any_t item) {
  list_t* self = selfof(_self);

  if (self->len == 0) {
    __list_push(_self, nullptr, item);
  } else {
    __list_push(_self, as(self->tail, any_t) - self->off, item);
  }
}

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
void __list_range_init(any_t _self) {
  selfof(_self)->itor = nullptr;
}

bool __list_range(any_t _self, bool flag) {
  list_t* self = selfof(_self);
  any_t* item  = _self;

  u_ret_if(self->len == 0, false);

  if (self->itor == nullptr) {
    self->itor = flag ? self->head : self->tail;
  } else {
    self->itor = flag ? self->itor->next : self->itor->prev;
  }

  u_ret_if(self->itor == nullptr, false);

  *item = as(self->itor, any_t) - self->off;

  return true;
}
