#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Data Structure
 *************************************************************************************************/
typedef struct buf_t* u_buf_t;

/*************************************************************************************************
 * Create
 *************************************************************************************************/
any_t __buf_new(size_t cap);
#define u_buf_new(cap) __buf_new(cap)

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __buf_clear(any_t _self);
#define u_buf_clear(buf) __buf_clear(buf)

void __buf_cleanup(any_t _self);
#define u_buf_cleanup(buf)                                                                         \
  do {                                                                                             \
    __buf_cleanup(buf);                                                                            \
    (buf) = nullptr;                                                                               \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
uint8_t* __buf_data(any_t _self);
#define u_buf_data(buf) __buf_data(buf)

size_t __buf_len(any_t _self);
#define u_buf_len(buf) __buf_len(buf)

size_t __buf_cap(any_t _self);
#define u_buf_cap(buf) __buf_cap(buf)

bool __buf_empty(any_t _self);
#define u_buf_empty(buf) __buf_empty(buf)

void __buf_slen(any_t _self, size_t len);
#define u_buf_slen(buf, len) __buf_slen(buf, len)

void __buf_peek(any_t _self, any_t mem, size_t len);
#define u_buf_peek(buf, mem, arg...) __buf_peek(buf, mem, va_0th(sizeof(*(mem)), arg))

void __buf_pop(any_t _self, any_t mem, size_t len);
#define u_buf_pop(buf, mem, arg...) __buf_pop(buf, mem, va_0th(sizeof(*(mem)), arg))

ret_t __buf_push(any_t _self, any_t mem, size_t len);
#define u_buf_push(buf, mem, arg...) __buf_push(buf, mem, va_0th(sizeof(*(mem)), arg))

#define u_buf_hex(buf) inf_hex(buf_data(buf), buf_len(buf))
