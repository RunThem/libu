#pragma once

#include "list.in"
#include "u.h"

#define list(T)                                                                                    \
  struct {                                                                                         \
    size_t len;                                                                                    \
    void* head; /* head */                                                                         \
    void* tail; /* tail */                                                                         \
    struct {                                                                                       \
      void* prev; /* head */                                                                       \
      void* next; /* tail */                                                                       \
      T val;                                                                                       \
    } node[0];                                                                                     \
  }*

#undef _
#define _(list)      (*(list))
#define list_T(list) typeof(_(list)->node[0])
#define list_V(list) typeof(_(list)->node[0].val)

#define list_itsize(list) ({ sizeof(list_T(list)); })

/*
 * 初始化list
 * */
#define list_init(list) __list_init(list)

/*
 * 清除list
 * */
#define list_clear(list) __list_clear(list)

/*
 * 释放list
 * */
#define list_cleanup(list) __list_cleanup(list)

/*
 * 在list结尾添加元素
 *
 * code:
 *    list(int) lst = nullptr;
 *
 *    list_init(&lst);
 *
 *    list_push_b(&lst, 10);
 *    list_push_b(&lst, 20);
 *    list_push_b(&lst, 30);
 *    list_push_b(&lst, 40);
 *
 *    // lst -> { 10, 20, 30, 40 }
 * */
#define list_push_b(list, _val) __list_push_b(list, _val)

/*
 * 在list开头添加元素
 *
 * code:
 *    list(int) lst = nullptr;
 *
 *    list_init(&lst);
 *
 *    list_push_f(&lst, 10);
 *    list_push_f(&lst, 20);
 *    list_push_f(&lst, 30);
 *    list_push_f(&lst, 40);
 *
 *    // lst -> { 40, 30, 20, 10 }
 * */
#define list_push_f(list, _val) __list_push_f(list, _val)
