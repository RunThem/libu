#pragma once

#include "u.h"
#include "vec.c"

#define vec(T)                                                                                     \
  struct {                                                                                         \
    size_t len;                                                                                    \
    size_t cap;                                                                                    \
    T data[0];                                                                                     \
  }*

/*
 * vec 中的元素类型
 * */
#define vec_T(v) typeof(_(v)->data[0])

/*
 * vec 中的元素大小
 * */
#define vec_itsize(vec) ({ sizeof(vec_T(vec)); })

/*
 * 初始化一个vec, 确定其大小
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init(&vv, 4);
 *
 *    // v -> (0, 8) { };
 * */
#define vec_init(vec, arg...) __vec_init(vec, arg)

/*
 * 初始化一个vec, 并添加一些数据进去
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    // v -> (4, 8) { 1, 4, 5, 10 };
 * */
#define vec_init_from(vec, item, arg...) __vec_init_from(vec, item, arg)

/*
 * 克隆一个vec, 内存布局相同
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto v_1 = vec_clone(&v);
 *
 *    // v   -> (4, 8) { 1, 4, 5, 10 };
 *    // v_1 -> (4, 8) { 1, 4, 5, 10 };
 * */
#define vec_clone(vec) __vec_clone(vec)

/*
 * 判断vec是否为空
 * */
#define vec_empty(vec) __vec_empty(vec)

/*
 * 扩容vec, size 必须大于 vec->len;
 * */
#define vec_resize(vec, size) __vec_resize(vec, size)

/*
 * 清除vec, 设置长度为0
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    vec_clear(&v);
 *
 *    // v   -> (0, 8) { };
 * */
#define vec_clear(vec) __vec_clear(vec)

/*
 * 释放vec
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    vec_clear(&v);
 *
 *    assert(nullptr == v);
 * */
#define vec_cleanup(vec) __vec_cleanup(vec)

/*
 * 访问vec中某一元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto p = vec_at(&v, 2)
 *
 *    assert(5 == *p);
 * */
#define vec_at(vec, idx) __vec_at(vec, idx)

/*
 * 在vec结尾追加元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    vec_push_b(&v, 2);
 *
 *    // v   -> (5, 8) { 1, 4, 5, 10, 2 };
 * */
#define vec_push_b(vec, item, arg...) __vec_insert(vec, (*vec)->len, item, arg)

/*
 * 在vec开头追加元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    vec_push_f(&v, 2);
 *
 *    // v   -> (5, 8) { 2, 1, 4, 5, 10 };
 * */
#define vec_push_f(vec, item, arg...) __vec_insert(vec, 0, item, arg)

/*
 * 访问vec中结尾元素, 从vec中删除结尾元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto p = vec_pop_b(&v);
 *
 *    assert(10 == *p);
 *
 *    // v   -> (3, 8) { 1, 4, 5 };
 * */
#define vec_pop_b(vec) __vec_pop_b(vec)

/*
 * 访问vec中开头元素, 从vec中删除开头元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto p = vec_pop_f(&v);
 *
 *    assert(1 == *p);
 *
 *    // v   -> (3, 8) { 4, 5, 10 };
 * */
#define vec_pop_f(vec) __vec_pop_f(vec)

/*
 * 与 `vec_pop_b`类似, 但不删除元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto p = vec_peek_b(&v);
 *
 *    assert(1 == *p);
 *
 *    // v   -> (4, 8) { 1, 4, 5, 10 };
 * */
#define vec_peek_b(vec) ({ _(vec)->data[(_(vec)->len == 0) ? 0 : _(vec)->len - 1]; })

/*
 * 与 `vec_pop_f`类似, 但不删除元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto p = vec_peek_f(&v);
 *
 *    assert(1 == *p);
 *
 *    // v   -> (4, 8) { 1, 4, 5, 10 };
 * */
#define vec_peek_f(vec) ({ _(vec)->data[0]; })

/*
 * 在vec中插入元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    vec_insert(&v, 2, 8, 9);
 *
 *    // v   -> (6, 8) { 1, 4, 8, 9, 5, 10 };
 * */
#define vec_insert(vec, idx, item, arg...) __vec_insert(vec, idx, item, arg)
