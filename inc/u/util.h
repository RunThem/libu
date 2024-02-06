#pragma once

#include <u/u.h>

/***************************************************************************************************
 * String
 **************************************************************************************************/
typedef char* str_t;

typedef const char* u_not_use_the_type_;
typedef char* const u_not_use_the_type__;
typedef const char* const u_not_use_the_type___;

#define u_str(s)                                                                                   \
  (((struct {                                                                                      \
     size_t cap;                                                                                   \
     size_t len;                                                                                   \
     char data[_Generic(s, char*: sizeof(typeof(s)) - 1, default: s) + 1];                         \
   }){.cap  = _Generic(s, char*: sizeof(typeof(s)) - 1, default: s),                               \
      .len  = _Generic(s, char*: sizeof(typeof(s)) - 1, default: 0),                               \
      .data = {_Generic(s, char*: s, default: "")}})                                               \
       .data)

str_t __str_new(size_t cap, str_t fmt, ...);
#define u_str_new(any, ...)                                                                        \
  (__str_new(_Generic(any, u_str_t: 0, default: any),                                              \
             _Generic(any, u_str_t: any, default: nullptr) __VA_OPT__(, ) __VA_ARGS__))

void __str_clear(str_t* _self);
#define u_str_clear(str) (__str_clear(str))

void __str_cleanup(str_t* _self);
#define u_str_cleanup(str)                                                                         \
  do {                                                                                             \
    __str_cleanup(str);                                                                            \
  } while (0)

void __str_slen(str_t* _self, size_t nlen);
#define u_str_slen(str, nlen) (__str_slen(str, nlen))

size_t __str_len(str_t* _self);
#define u_str_len(str) (__str_len(str))

size_t __str_cap(str_t* _self);
#define u_str_cap(str) (__str_cap(str))

bool __str_empty(str_t* _self);
#define u_str_empty(str) (__str_empty(str))

ret_t __str_append(str_t* _self, str_t fmt, ...);
#define u_str_append(str, fmt, ...) (__str_append(str, fmt __VA_OPT__(, ) __VA_ARGS__))

ret_t __str_erase(str_t* _self, size_t idx, size_t len);
#define u_str_erase(str, idx, ...) (__str_erase(str, idx, va_0th(1, __VA_ARGS__)))

ssize_t __str_read_fd(str_t* _self, int fd, size_t size);
#define u_str_read(str, var, ...)                                                                  \
  (_Generic(var, int: __str_read_fd, default: nullptr)(str, var, __VA_ARGS__))

/***************************************************************************************************
 * File System
 **************************************************************************************************/
extern off_t u_fs_size(str_t file);

extern bool u_fs_exist(str_t file);

extern str_t u_fs_read(str_t file);

extern off_t u_fs_write(str_t file, str_t buf, size_t len);

extern bool u_fs_remove(str_t file);
