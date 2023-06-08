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
 * */
#define vec_clear(vec) __vec_clear(vec)

/*
 * 释放vec
 * */
#define vec_cleanup(vec) __vec_cleanup(vec)
