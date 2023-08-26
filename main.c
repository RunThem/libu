/* libs */
#include "buf.h"
#include "fs.h"
#include "list.h"
#include "map.h"
#include "que.h"
#include "sock.h"
#include "stack.h"
#include "str.h"
#include "u.h"
// #include "vec.h"

#define _typeof(t) __builtin_classify_type(t)

#define _list(T)                                                                                   \
  struct {                                                                                         \
    list_t _;                                                                                      \
    struct {                                                                                       \
      any_t prev;                                                                                  \
      any_t next;                                                                                  \
    } __;                                                                                          \
    T it;                                                                                          \
  }

static inline ret_t _list_head(any_t _self) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->head == nullptr, -1);

  memcpy(self + 1, self->head, sizeof(struct {
                                 any_t prev;
                                 any_t next;
                               }) + self->itsize);

  return 0;
}

static inline ret_t _list_tail(any_t _self) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->tail == nullptr, -1);

  memcpy(self + 1, self->tail, sizeof(struct {
                                 any_t prev;
                                 any_t next;
                               }) + self->itsize);

  return 0;
}

static inline ret_t _list_next(any_t _self) {
  list_t* self = as(_self, list_t*);

  u_ret_if(_self == nullptr, -1);
  u_ret_if(self->tail == nullptr, -1);

  return 0;
}

int main(int argc, const char** argv) {

  list(int) lst = {0};

  list_init(&lst);

  list_push_b(lst, 1);
  list_push_b(lst, 12);
  list_push_b(lst, 13);
  list_push_b(lst, 14);
  list_push_b(lst, 15);
  list_push_b(lst, 2);

  list_for(lst, it) {
    inf("%d", it->it);
  }

  return 0;
}
