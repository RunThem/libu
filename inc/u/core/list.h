#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
#define u_lnode_t(T)                                                                               \
  struct {                                                                                         \
    T* prev;                                                                                       \
    T* next;                                                                                       \
  }

#define u_list_t(T)                                                                                \
  struct {                                                                                         \
    T* item;                                                                                       \
  }*

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __list_new(size_t off);
#define u_list_new(T, field) __list_new(offsetof(T, field))

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __list_cleanup(any_t _self);
#define list_cleanup(list)                                                                         \
  do {                                                                                             \
    __list_cleanup(list);                                                                          \
    (list) = nullptr;                                                                              \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __list_len(any_t _self);
#define u_list_len(list) __list_len(list)

bool __list_empty(any_t _self);
#define u_list_empty(list) __list_empty(list)

void __list_pop(any_t _self, any_t idx);
#define u_list_pop(list, node) __list_pop(list, node)

void __list_pop_front(any_t _self);
#define u_list_pop_front(list) __list_pop_front(list)

void __list_pop_back(any_t _self);
#define u_list_pop_back(list) __list_pop_back(list)

void __list_push(any_t _self, any_t idx, any_t item);
#define u_list_push(list, listnode, node) __list_push(list, listnode, node)

void __list_push_front(any_t _self, any_t item);
#define u_list_push_front(list, node) __list_push_front(list, node)

void __list_push_back(any_t _self, any_t item);
#define u_list_push_back(list, node) __list_push_back(list, node)

/*************************************************************************************************
 * Iterator
 *************************************************************************************************/
bool __list_range(any_t _self, bool flag);
#define list_for(list)  for ((list)->item = nullptr; __list_range(list, true);)
#define list_rfor(list) for ((list)->item = nullptr; __list_range(list, false);)