#pragma once

#include "u.h"

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef struct buf_t* buf_t;
struct buf_t {
  size_t len;
  size_t cap;
  uint8_t* c_buf;
};

/*************************************************************************************************
 * Create & Clone
 *************************************************************************************************/
any_t __buf_new(size_t cap);
#define buf_new(cap) __buf_new(cap)

ret_t __buf_init(any_t* _self, size_t cap);
#define buf_init(buf, cap) __buf_init(any(buf), cap)

/*************************************************************************************************
 * Expansion & Destruction
 *************************************************************************************************/
ret_t __buf_resize(any_t _self, size_t cap);
#define buf_resize(buf, cap) __buf_resize(buf, cap)

void __buf_clear(any_t _self);
#define buf_clear(buf) __buf_clear(buf)

ret_t __buf_cleanup(any_t _self);
#define buf_cleanup(buf) __buf_cleanup(buf)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __buf_len(any_t _self);
#define buf_len(buf) __buf_len(buf)

size_t __buf_cap(any_t _self);
#define buf_cap(buf) __buf_cap(buf)

bool __buf_empty(any_t _self);
#define buf_empty(buf) __buf_empty(buf)

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
