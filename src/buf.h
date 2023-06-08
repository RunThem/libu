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
 *    buf_t buf = buf_new(a, 10);
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
