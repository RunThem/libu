#pragma once

#include <u/u.h>

typedef char* u_str_t;

typedef const char* u_not_use_the_type_;
typedef char* const u_not_use_the_type__;
typedef const char* const u_not_use_the_type___;

#define s(_str)                                                                                    \
  (((struct {                                                                                      \
     size_t cap;                                                                                   \
     size_t len;                                                                                   \
     char data[sizeof(_str)];                                                                      \
   }){.len = sizeof(_str) - 1, .data = (_str)})                                                    \
       .data)

/*************************************************************************************************
 * Create
 *************************************************************************************************/
u_str_t __str_new(size_t cap, u_str_t fmt, ...);
#define u_str_new(any, ...)                                                                        \
  __str_new(_Generic(any, u_str_t: 0, default: any),                                               \
            _Generic(any, u_str_t: any, default: nullptr) __VA_OPT__(, ) __VA_ARGS__)

/*************************************************************************************************
 * Destruction
 *************************************************************************************************/
void __str_clear(u_str_t* _self);
#define u_str_clear(str) __str_clear(str)

void __str_cleanup(u_str_t* _self);
#define u_str_cleanup()                                                                            \
  do {                                                                                             \
    __str_cleanup(str);                                                                            \
  } while (0)

/*************************************************************************************************
 * Interface
 *************************************************************************************************/
size_t __str_len(u_str_t* _self);
#define u_str_len(str) __str_len(str)

size_t __str_cap(u_str_t* _self);
#define u_str_cap(str) __str_cap(str)

bool __str_empty(u_str_t* _self);
#define u_str_empty(str) __str_empty(str)

ret_t __str_append(u_str_t* _self, u_str_t fmt, ...);
#define u_str_append(str, fmt, ...) __str_append(str, fmt __VA_OPT__(, ) __VA_ARGS__)

ret_t __str_erase(u_str_t* _self, size_t idx, size_t len);
#define u_str_erase(str, idx, ...) __str_erase(str, idx, va_0th(1, __VA_ARGS__))
