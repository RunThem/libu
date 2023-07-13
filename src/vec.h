#pragma once

#include "u.h"
#include "vec.in"

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
 * 访问vec中指定索引元素, 从vec中删除该元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto p = vec_pop(&v, 1);
 *
 *    assert(4 == *p);
 *
 *    // v   -> (3, 8) { 1, 5, 10 };
 * */
#define vec_pop(vec, idx) __vec_pop(vec, idx)

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

/*
 * 删除vec中部分元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    vec_erase(&v, 1, 2);
 *
 *    // v   -> (2, 8) { 1, 10 };
 * */
#define vec_erase(vec, idx, arg...) __vec_erase(vec, idx, arg)

/*
 * 在vec中查照元素, 并返回下标
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto idx = vec_find(&v, 5);
 *
 *    assert(2 == idx);
 * */
#define vec_find(vec, item, arg...) __vec_find(vec, item, arg)

/*
 * 在vec中是否包含某元素
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto is = vec_contain(&v, 5);
 *
 *    assert(idx);
 *
 *    is = vec_contain(&v, 100);
 *
 *    assert(!idx);
 * */
#define vec_contain(vec, item, arg...) ({ (bool)(vec_find(vec, item, arg) != -1); })

/*
 * 比较两个vec是否长度相同, 元素相同
 *
 * code:
 *    vec(int) v = nullptr;
 *
 *    vec_init_from(&vv, 1, 4, 5, 10);
 *
 *    auto v_1 = vec_clone(&vv);
 *
 *    auto is = vec_comp(&v, v_1);
 *
 *    assert(idx);
 * */
#define vec_comp(vec, v, arg...) __vec_comp(vec, v, arg)

#define vec_for(vec, it)                                                                           \
  for (auto(it) = vec_at(vec, 0); _(vec)->len > 0 && (it) <= vec_at(vec, _(vec)->len - 1); (it)++)

#define vec_each(vec, i) for (size_t(i) = 0; (i) < _(vec)->len; (i)++)

#define vec_dis(v, fn) __vec_dis(v, fn)
