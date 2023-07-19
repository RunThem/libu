#pragma once

/*************************************************************************************************
 * __include__
 *************************************************************************************************/
#include <ctype.h>
#include <errno.h>
#include <mimalloc.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*************************************************************************************************
 * __va__
 *************************************************************************************************/
#define __va_cut_one(_0, ...) __VA_ARGS__
#define __va_at_one(_0, ...)  _0

#define __va_cut_0(...) __VA_ARGS__
#define __va_cut_1(...) __va_cut_one(__VA_ARGS__)
#define __va_cut_2(...) __va_cut_1(__va_cut_one(__VA_ARGS__))
#define __va_cut_3(...) __va_cut_2(__va_cut_one(__VA_ARGS__))
#define __va_cut_4(...) __va_cut_3(__va_cut_one(__VA_ARGS__))
#define __va_cut_5(...) __va_cut_4(__va_cut_one(__VA_ARGS__))

#define __va_at_0(...) __va_at_one(__VA_ARGS__)
#define __va_at_1(...) __va_at_0(__va_cut_1(__VA_ARGS__))
#define __va_at_2(...) __va_at_0(__va_cut_2(__VA_ARGS__))
#define __va_at_3(...) __va_at_0(__va_cut_3(__VA_ARGS__))
#define __va_at_4(...) __va_at_0(__va_cut_4(__VA_ARGS__))
#define __va_at_5(...) __va_at_0(__va_cut_5(__VA_ARGS__))

#define __va_cat(x, y)                                 x##y
#define __va_at(i, ...)                                __va_cat(__va_at_, i)(__VA_ARGS__)
#define __va_cut(i, ...)                               __va_cat(__va_cut_, i)(__VA_ARGS__)
#define __va_size(_0, _1, _2, _3, _4, _5, _6, _n, ...) _n
#define __va_opt(...)                                  __VA_OPT__(, )

#define va_cat(x, y)     __va_cat(x, y)
#define va_at(i, ...)    __va_at(i __VA_OPT__(, ) __VA_ARGS__)
#define va_cut(i, ...)   __va_cut(i __VA_OPT__(, ) __VA_ARGS__)
#define va_size(...)     __va_size("ignore" __VA_OPT__(, ) __VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define va_empty(...)    __va_size("ignore" __VA_OPT__(, ) __VA_ARGS__, 0, 0, 0, 0, 0, 0, 1)
#define va_opt(i, ...)   __va_opt(__va_cut(i, __VA_ARGS__))
#define va_slice(i, ...) va_opt(i, __VA_ARGS__) va_cut(i, __VA_ARGS__)
#define va_or(t, f, ...) va_at(1 __VA_OPT__(, ) va_at(0, __VA_ARGS__), f, t)

#define __va_th(...) va_at(0, __VA_ARGS__)

#define va_0th(def, ...) __va_th(va_cut(0, __VA_ARGS__) va_opt(0, __VA_ARGS__) def)
#define va_1th(def, ...) __va_th(va_cut(1, __VA_ARGS__) va_opt(1, __VA_ARGS__) def)
#define va_2th(def, ...) __va_th(va_cut(2, __VA_ARGS__) va_opt(2, __VA_ARGS__) def)
#define va_3th(def, ...) __va_th(va_cut(3, __VA_ARGS__) va_opt(3, __VA_ARGS__) def)
#define va_4th(def, ...) __va_th(va_cut(4, __VA_ARGS__) va_opt(4, __VA_ARGS__) def)
#define va_5th(def, ...) __va_th(va_cut(5, __VA_ARGS__) va_opt(5, __VA_ARGS__) def)

/*************************************************************************************************
 * __keyword__
 *************************************************************************************************/
#define auto __auto_type

#if !defined(__clang__) || __clang_major__ < 17
#  define nullptr NULL
#endif

/*************************************************************************************************
 * __type__
 *************************************************************************************************/
typedef void* any_t;

#define useno(x) ((void)(x))
#define chr(c)   (as(c, char))
#define any(p)   (as(p, any_t))

#define fnt(n, r, ...) r (*n)(__VA_ARGS__)

typedef fnt(cmp_fn, int, any_t, any_t);

typedef int ret_t;

typedef float f32_t;
typedef double f64_t;
typedef long double f128_t;

/*************************************************************************************************
 * __print__
 *************************************************************************************************/
#define __fmt_base(expr)                                                                           \
  _Generic((expr),                                                                                 \
      char: "\'%c\'",                                                                              \
      signed char: "%hhi",                                                                         \
      unsigned char: "%hhu",                                                                       \
      short: "%hi",                                                                                \
      unsigned short: "%hu",                                                                       \
      int: "%d",                                                                                   \
      unsigned int: "%u",                                                                          \
      long: "%ld",                                                                                 \
      unsigned long: "%lu",                                                                        \
      long long: "%lld",                                                                           \
      unsigned long long: "%llu",                                                                  \
      float: "%f",                                                                                 \
      double: "%lf",                                                                               \
      char*: "\"%s\"",                                                                             \
      const char*: "\"%s\"",                                                                       \
      signed char*: "\"%s\"",                                                                      \
      const signed char*: "\"%s\"",                                                                \
      unsigned char*: "\"%s\"",                                                                    \
      const unsigned char*: "\"%s\"",                                                              \
      default: "%p")

#define __prt(fmt, ...) fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)
#define __inf(fmt, ...)                                                                            \
  __prt("\x1b[02m[%s $%d %s]\x1b[0m: " fmt,                                                        \
        __FILE__,                                                                                  \
        __LINE__,                                                                                  \
        __FUNCTION__ __VA_OPT__(, ) __VA_ARGS__)

#define inf(fmt, ...) __inf(fmt "\n", __VA_ARGS__)

#define inf_arr(arr, arg...)                                                                       \
  do {                                                                                             \
    char* __delim  = "";                                                                           \
    char __fmt[72] = {0};                                                                          \
    size_t __size  = va_0th(array_len(arr), arg);                                                  \
                                                                                                   \
    snprintf(__fmt, array_len(__fmt), "%%s%s", __fmt_base(arr[0]));                                \
    __inf("\x1b[36;1m%s\x1b[0m = (len: %ld) [", #arr, __size);                                     \
    for (size_t __i = 0; __i < __size; __i++) {                                                    \
      __prt(__fmt, __delim, arr[__i]);                                                             \
      __delim = ", ";                                                                              \
    }                                                                                              \
    __prt("]\n");                                                                                  \
  } while (0)

#define inf_hex(ptr, arg...)                                                                       \
  do {                                                                                             \
                                                                                                   \
    char __buf[18] = {'\''};                                                                       \
    uint8_t* __ptr = (uint8_t*)ptr;                                                                \
    size_t __i     = 0;                                                                            \
    size_t __pos   = 0;                                                                            \
    size_t __size  = va_0th(sizeof(ptr), arg);                                                     \
    __inf("\x1b[36;1m%s\x1b[0m = (size: %ld)\n", #ptr, __size);                                    \
    while (__size > 0) {                                                                           \
      if ((__pos % 16) == 0)                                                                       \
        __prt("    %08lx: ", __pos);                                                               \
                                                                                                   \
      __prt("%02x ", __ptr[__pos] & 0xff);                                                         \
      if ((__pos % 16) == 7)                                                                       \
        __prt(" ");                                                                                \
                                                                                                   \
      if ((__pos % 16) == 15) {                                                                    \
        for (__i = 1; __i < 18; __i++) {                                                           \
          auto __idx = __pos - 16 + __i;                                                           \
          __buf[__i] = isprint(__ptr[__idx]) ? __ptr[__idx] : '.';                                 \
        }                                                                                          \
                                                                                                   \
        __buf[__i - 1] = '\0';                                                                     \
        __prt("%s'\n", __buf);                                                                     \
      }                                                                                            \
                                                                                                   \
      __pos++;                                                                                     \
      __size--;                                                                                    \
    }                                                                                              \
                                                                                                   \
    if ((__pos % 16) != 0) {                                                                       \
      for (__i = 1; __i < __pos % 16 + 2; __i++) {                                                 \
        auto __idx = __pos - (__pos % 16) + __i - 1;                                               \
        __buf[__i] = isprint(__ptr[__idx]) ? __ptr[__idx] : '.';                                   \
      }                                                                                            \
                                                                                                   \
      __buf[__i - 1] = '\0';                                                                       \
      __prt("%*s'\n", 48 - 3 * ((int)__pos % 16) + (int)__i, __buf);                               \
    }                                                                                              \
  } while (0)

#define err(fmt, ...)                                                                              \
  do {                                                                                             \
    const char* __ERROR__ = (errno != 0) ? strerror(errno) : "no errno";                           \
    inf("(\x1b[31m%s\x1b[0m) " fmt, __ERROR__ __VA_OPT__(, ) __VA_ARGS__);                         \
  } while (0)

/*************************************************************************************************
 * __debug__
 *************************************************************************************************/
#define u_if(expr, arg...)                                                                         \
  do {                                                                                             \
    if (expr)                                                                                      \
      err("(%s) " va_0th("", arg), #expr va_slice(1, arg));                                        \
  } while (0)

#define u_die_if(expr, arg...)                                                                     \
  do {                                                                                             \
    if (expr) {                                                                                    \
      err("(%s) " va_0th("", arg), #expr va_slice(1, arg));                                        \
      exit(EXIT_FAILURE);                                                                          \
    }                                                                                              \
  } while (0)

#define u_ret_if(expr, code, arg...)                                                               \
  do {                                                                                             \
    if (expr) {                                                                                    \
      err("(%s) " va_0th("", arg), #expr va_slice(1, arg));                                        \
      return code;                                                                                 \
    }                                                                                              \
  } while (0)

#define u_goto_if(expr, arg...)                                                                    \
  do {                                                                                             \
    if (expr) {                                                                                    \
      err("(%s) " va_1th("", arg), #expr va_slice(2, arg));                                        \
      goto va_0th(err, arg);                                                                       \
    }                                                                                              \
  } while (0)

#define u_free_if(mem)                                                                             \
  do {                                                                                             \
    if ((mem) != nullptr) {                                                                        \
      u_free(mem);                                                                                 \
    }                                                                                              \
  } while (0)

#define u_close_if(fd)                                                                             \
  do {                                                                                             \
    if ((fd) >= 0) {                                                                               \
      close(fd);                                                                                   \
    }                                                                                              \
  } while (0)

#define u_fclose_if(fp)                                                                            \
  do {                                                                                             \
    if ((fp) != nullptr) {                                                                         \
      fclose(fd);                                                                                  \
    }                                                                                              \
  } while (0)

#define u_alloc_if(mem, arg...) u_goto_if((mem) == nullptr, err, arg)

#define u_ret_no_if(expr, arg...) u_ret_if(expr, , arg)

/*************************************************************************************************
 * __misc__
 *************************************************************************************************/
#define me(v)    (&(struct { typeof(v) _; }){(v)})
#define as(v, T) ((T)(v))
#define rs(...)  #__VA_ARGS__

#define container_of(ptr, type, member)                                                            \
  ({                                                                                               \
    const typeof(((type*)0)->member)* _container_of__mptr = any(ptr);                              \
    (type*)((char*)_container_of__mptr - offsetof(type, member));                                  \
  })

#define align_of(addr, size) ({ ((addr) + (size)-1) & (~((size)-1)); })

#define arrlen(a) (sizeof(a) / sizeof((a)[0]))

#define swap(a, b)                                                                                 \
  do {                                                                                             \
    auto _swap__tmp = a;                                                                           \
    (a)             = (b);                                                                         \
    (b)             = (_swap__tmp);                                                                \
  } while (0)

#define fn_eq_use(name) (fn_eq_##name)
#define fn_eq_dec(name) bool fn_eq_##name(any_t _x, any_t _y)
#define fn_eq_def(name, type, code)                                                                \
  fn_eq_dec(name) {                                                                                \
    auto x = *(type*)_x;                                                                           \
    auto y = *(type*)_y;                                                                           \
    return (code);                                                                                 \
  }

#define fn_cmp_use(name) (fn_cmp_##name)
#define fn_cmp_dec(name) int fn_cmp_##name(any_t _x, any_t _y)
#define fn_cmp_def(name, type, code)                                                               \
  fn_cmp_dec(name) {                                                                               \
    auto x = *(type*)_x;                                                                           \
    auto y = *(type*)_y;                                                                           \
    if (fn_eq_use(name)(_x, _y)) {                                                                 \
      return 0;                                                                                    \
    }                                                                                              \
    return (code) ? 1 : -1;                                                                        \
  }

#define min(_0, _1, arg...)                                                                        \
  ({                                                                                               \
    typeof(_0) _min__arr[] = {(_0), (_1)va_slice(0, arg)};                                         \
    size_t _min__idx       = 0;                                                                    \
    for (size_t _min__i = 1; _min__i < arrlen(_min__arr); _min__i++)                               \
      if (_min__arr[_min__idx] > _min__arr[_min__i])                                               \
        _min__idx = _min__i;                                                                       \
                                                                                                   \
    _min__arr[_min__idx];                                                                          \
  })

#define max(_0, _1, arg...)                                                                        \
  ({                                                                                               \
    typeof(_0) _max__arr[] = {(_0), (_1)va_slice(0, arg)};                                         \
    size_t _max__idx       = 0;                                                                    \
    for (size_t _max__i = 1; _max__i < arrlen(_max__arr); _max__i++)                               \
      if (_max__arr[_max__idx] < _max__arr[_max__i])                                               \
        _max__idx = _max__i;                                                                       \
                                                                                                   \
    _max__arr[_max__idx];                                                                          \
  })

/*
 * a > b  ->  1
 * a == b ->  0
 * a < b  -> -1
 * */

#define min_idx(ptr, size, arg...)                                                                 \
  ({                                                                                               \
    size_t _min_idx__idx = 0;                                                                      \
    auto _min_idx__fn    = va_0th((nullptr), arg);                                                 \
    for (size_t _min_idx__i = 1; _min_idx__i < (size); _min_idx__i++)                              \
      if (va_or((ptr)[_min_idx__idx] > (ptr)[_min_idx__i],                                         \
                (1) == _min_idx__fn((ptr)[_min_idx__idx], (ptr)[_min_idx__i]),                     \
                arg))                                                                              \
        _min_idx__idx = _min_idx__i;                                                               \
    (void)_min_idx__fn;                                                                            \
    _min_idx__idx;                                                                                 \
  })

#define max_idx(ptr, size, arg...)                                                                 \
  ({                                                                                               \
    size_t _max_idx__idx = 0;                                                                      \
    auto _max_idx__lm    = va_0th((nullptr), arg);                                                 \
    for (size_t _max_idx__i = 1; _max_idx__i < (size); _max_idx__i++)                              \
      if (va_or((ptr)[_max_idx__idx] < (ptr)[_max_idx__i],                                         \
                (-1) == _max_idx__lm((ptr)[_max_idx__idx], (ptr)[_max_idx__i]),                    \
                arg))                                                                              \
        _max_idx__idx = _max_idx__i;                                                               \
    (void)_max_idx__lm;                                                                            \
    _max_idx__idx;                                                                                 \
  })

#define min_from(ptr, size, arg...) ({ (ptr)[min_idx(ptr, size, arg)]; })

#define max_from(ptr, size, arg...) ({ (ptr)[max_idx(ptr, size, arg)]; })

/*************************************************************************************************
 * __alloc__
 *************************************************************************************************/
#define u_malloc(size)                                                                             \
  ({                                                                                               \
    any_t _ptr = mi_malloc(size);                                                                  \
    if (errno == 2)                                                                                \
      errno = 0;                                                                                   \
    _ptr;                                                                                          \
  })

#define u_calloc(count, size)                                                                      \
  ({                                                                                               \
    any_t _ptr = mi_calloc(count, size);                                                           \
    if (errno == 2)                                                                                \
      errno = 0;                                                                                   \
    _ptr;                                                                                          \
  })

#define u_zalloc(size)                                                                             \
  ({                                                                                               \
    any_t _ptr = mi_calloc(1, size);                                                               \
    if (errno == 2)                                                                                \
      errno = 0;                                                                                   \
    _ptr;                                                                                          \
  })

#define u_talloc(size, type)                                                                       \
  ({                                                                                               \
    any_t _ptr = mi_calloc(1, size);                                                               \
    if (errno == 2)                                                                                \
      errno = 0;                                                                                   \
    (type) _ptr;                                                                                   \
  })

#define u_realloc(ptr, size)                                                                       \
  ({                                                                                               \
    any_t _ptr = mi_realloc(ptr, size);                                                            \
    if (errno == 2)                                                                                \
      errno = 0;                                                                                   \
    _ptr;                                                                                          \
  })

#define u_free(ptr) ({ mi_free(ptr); })
