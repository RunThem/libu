#pragma once

#include "u.h"

typedef struct {
  size_t len;
  size_t cap;
  uint8_t* c_buf;
} buf_t;

#define buf_isinit(buf) ((buf)->c_buf == 0)
#define buf_len(buf)    ((buf)->len)
#define buf_cap(buf)    ((buf)->cap)
#define buf_empty(buf)  ((buf)->len == 0)
#define buf_data(buf)   ((buf)->c_buf)
#define buf_clear(buf)  ((buf)->len = 0)

ret_t __buf_init(any_t _self, size_t cap);
#define buf_init(buf, cap) __buf_init(buf, cap)

ret_t __buf_resize(any_t _self, size_t cap);
#define buf_resize(buf, cap) __buf_resize(buf, cap)

ret_t __buf_cleanup(any_t _self);
#define buf_cleanup(buf) __buf_cleanup(buf)

ret_t __buf_push(any_t _self, any_t mem, size_t len);
#define buf_push(buf, mem, arg...)                                                                 \
  ({                                                                                               \
    auto __mem = (mem);                                                                            \
    __buf_push(buf, va_0or(__mem, (uint8_t*)&(__mem), arg), va_0th(sizeof(__mem), arg));           \
  })

ret_t __buf_pop(any_t _self, any_t mem, size_t len);
#define buf_pop(buf, mem, arg...) __buf_pop(buf, mem, va_0th(sizeof(*(mem)), arg))

ret_t __buf_peek(any_t _self, any_t mem, size_t len);
#define buf_peek(buf, mem, arg...) __buf_peek(buf, mem, va_0th(sizeof(*(mem)), arg))

#define buf_hex(buf) inf_hex(buf_data(buf), buf_len(buf))
