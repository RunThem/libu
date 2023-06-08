#pragma once

#include "u.h"

typedef uint8_t* c_buf;
struct __buf {
  size_t len;
  size_t cap;
  uint8_t c_buf[0];
};

typedef struct __buf* buf_t;

/*
 * 新建一个buf, 并复制一些数据进去
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    // buf -> (10, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00};
 * */
buf_t buf_new(c_buf c_buffer, size_t len);

/*
 * 新建一个buf, 指定buf的大小
 *
 * code:
 *    buf_t buf;
 *
 *    buf_init(&buf, 100);
 *
 *    // buf -> (0, 100) { }
 * */
int buf_init(buf_t* buf, size_t len);

/*
 * 判断buf是否为空
 * */
bool buf_empty(buf_t* buf);

/*
 * 扩展buf, len 必须大于 buf->cap;
 * */
int buf_resize(buf_t* buf, size_t len);

/*
 * 清除buf, 设置长度为0
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    buf_clear(&buf);
 *
 *    // buf -> (0, 32) { };
 * */
void buf_clear(buf_t* buf);

/*
 * 释放buf
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    buf_clear(&buf);
 *
 *    assert(nullptr == buf);
 * */
void buf_cleanup(buf_t* buf);

/*
 * 在buf结尾追加数据
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    // buf -> (10, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00};
 *
 *    int b[] = { 0x11, 0x3 };
 *
 *    buf_push(&buf, p(b), 6);
 *
 *    // buf -> (6, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x11, 0x00,
 *    0x00, 0x00, 0x03, 0x00};
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    // buf -> (10, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00};
 *
 *    buf_push(&buf, 0x0a);
 *
 *    // buf -> (14, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00, 0x0a, 0x00,
 *    0x00, 0x00};
 * */
int __buf_push(buf_t* buf, c_buf c_buffer, size_t size);
#define buf_push(buf, c_buffer, arg...)                                                            \
  ({                                                                                               \
    typeof(c_buffer) _b_push__c_buf = (c_buffer);                                                  \
    __buf_push(buf,                                                                                \
               va_or((c_buf)&_b_push__c_buf, (c_buf)_b_push__c_buf, arg),                          \
               va_0th(sizeof(typeof(c_buffer)), arg));                                             \
  })

/*
 * 从buf结尾拷贝出一部分字符串, 并将长度 - len
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    // buf -> (10, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00};
 *
 *    int b[2] = {  };
 *
 *    buf_pop(&buf, p(b), 8);
 *
 *    // buf -> (2, 32) { 0x01, 0x00 };
 *    // b   ->         { 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00 };
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    // buf -> (10, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00};
 *
 *    int b = 0;
 *    buf_push(&buf, b);
 *
 *    // buf -> (6, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00
 *    // a   ->         { 0x00, 0x00, 0x03, 0x00 };
 * */
int __buf_pop(buf_t* buf, c_buf c_buffer, size_t size);
#define buf_pop(buf, c_buffer, arg...)                                                             \
  ({                                                                                               \
    __buf_pop(buf,                                                                                 \
              va_or((c_buf)&c_buffer, (c_buf)c_buffer, arg),                                       \
              va_0th(sizeof(typeof(c_buffer)), arg));                                              \
  })

/*
 * 与 `buf_pop` 类似, 不同的是长度不会发生改变
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    // buf -> (10, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00};
 *
 *    int b[2] = {  };
 *
 *    buf_pop(&buf, p(b), 8);
 *
 *    // buf -> (2, 32) { 0x01, 0x00 };
 *    // b   ->         { 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00 };
 *
 * code:
 *    int a[] = { 0x01, 0x05, 0x03 };
 *    buf_t buf = buf_new(p(a), 10);
 *
 *    // buf -> (10, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x03, 0x00};
 *
 *    int b = 0;
 *    buf_pop(&buf, b);
 *
 *    // buf -> (6, 32) { 0x01, 0x00, 0x00, 0x00, 0x05, 0x00
 *    // b   ->         { 0x00, 0x00, 0x03, 0x00 };
 * */
int __buf_peek(buf_t* buf, c_buf c_buffer, size_t size);
#define buf_peek(buf, c_buffer, arg...)                                                            \
  ({                                                                                               \
    __buf_peek(buf,                                                                                \
               va_or((c_buf)&c_buffer, (c_buf)c_buffer, arg),                                      \
               va_0th(sizeof(typeof(c_buffer)), arg));                                             \
  })

#define buf_dis(buf) inf_hex((*(buf))->c_buf, (*(buf))->len)
