#include <u/core.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct lst_t lst_t;
struct lst_t {
  bool flags[4];
  uvec(any_t) items;

  size_t iter;
};

/***************************************************************************************************
 * Macro
 **************************************************************************************************/

/***************************************************************************************************
 * Function
 **************************************************************************************************/
u_lst_t lst_new() {

  lst_t* self = nullptr;

  self = u_zalloc(sizeof(lst_t));
  u_mem_if(self);

  uv_init(self->items);
  u_mem_if(self->items);

  self->iter = 0;

  return as(self, u_lst_t);

err:
  u_free_if(self);

  return nullptr;
}

void lst_cleanup(u_lst_t _self) {
  lst_t* self = as(_self, lst_t*);

  uv_cleanup(self->items);

  u_free_if(self);
}

size_t lst_len(u_lst_t _self) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, 0);

  return uv_len(self->items);
}

bool lst_exist(u_lst_t _self, any_t ptr) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, false);
  u_check_ret(ptr == nullptr, false);
  u_check_ret(uv_empty(self->items), false);

  uv_foreach(self->items, i, node, {
    if (node == ptr) {
      return true;
    }
  });

  return false;
}

any_t lst_first(u_lst_t _self) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, nullptr);
  u_check_ret(uv_empty(self->items), nullptr);

  return uv_at(self->items, 0ul);
}

any_t lst_last(u_lst_t _self) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, nullptr);
  u_check_ret(uv_empty(self->items), nullptr);

  return uv_at(self->items, -1ul);
}

any_t lst_next(u_lst_t _self, any_t idx) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, nullptr);
  u_check_ret(idx == nullptr, nullptr);

  uv_foreach(self->items, i, node, {
    if (node == idx) {
      return uv_at(self->items, i + 1);
    }
  });

  return nullptr;
}

any_t lst_prev(u_lst_t _self, any_t idx) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, nullptr);
  u_check_ret(idx == nullptr, nullptr);

  uv_foreach(self->items, i, node, {
    if (node == idx) {
      return (i == 0) ? nullptr : uv_at(self->items, i - 1);
    }
  });

  return nullptr;
}

void lst_pop(u_lst_t _self, any_t ptr) {
  lst_t* self = as(_self, lst_t*);

  u_check_nret(self == nullptr);
  u_check_nret(ptr == nullptr);
  u_check_nret(uv_empty(self->items));

  uv_foreach(self->items, i, node, {
    if (node == ptr) {
      uv_pop(self->items, i);
      break;
    }
  });
}

void lst_put(u_lst_t _self, any_t idx, any_t ptr) {
  lst_t* self = as(_self, lst_t*);

  u_check_nret(self == nullptr);
  u_check_nret(ptr == nullptr);

  if (idx == nullptr) {
    uv_put(self->items, 0, ptr);
  } else {
    uv_foreach(self->items, i, node, {
      if (node == idx) {
        uv_put(self->items, i + 1, ptr);
        break;
      }
    });
  }
}

any_t lst_each_init(u_lst_t _self, bool flag) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, nullptr);

  self->flags[1] = flag;
  self->flags[2] = true;
  self->flags[3] = false;

  return nullptr;
}

any_t lst_each(u_lst_t _self) {
  lst_t* self = as(_self, lst_t*);

  u_check_ret(self == nullptr, nullptr);
  u_check_ret(uv_empty(self->items), nullptr);
  u_check_ret(self->flags[3], nullptr);

  /* 初始化 */
  if (self->flags[2]) {
    self->iter     = self->flags[1] ? 0 : uv_len(self->items) - 1;
    self->flags[2] = !self->flags[2];
  } else { /* 迭代 */
    self->iter += (self->flags[1] ? +1 : -1);

    /* 判断是否继续迭代 */
    if (self->iter == 0 || self->iter == uv_len(self->items) - 1) {
      self->flags[3] = true;
    }
  }

  return uv_at(self->items, self->iter);
}
