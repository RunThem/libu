#pragma once

/*************************************************************************************************
 * __include__
 *************************************************************************************************/
#include <assert.h>
#include <ctype.h>
#include <errno.h>
#include <stdalign.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

#define va_0or(t, f, ...) va_at(1 va_opt(0, __VA_ARGS__) va_at(0, __VA_ARGS__), t, f)
#define va_1or(t, f, ...) va_at(1 va_opt(1, __VA_ARGS__) va_at(1, __VA_ARGS__), t, f)
#define va_2or(t, f, ...) va_at(1 va_opt(2, __VA_ARGS__) va_at(2, __VA_ARGS__), t, f)
#define va_3or(t, f, ...) va_at(1 va_opt(3, __VA_ARGS__) va_at(3, __VA_ARGS__), t, f)
#define va_4or(t, f, ...) va_at(1 va_opt(4, __VA_ARGS__) va_at(4, __VA_ARGS__), t, f)
#define va_5or(t, f, ...) va_at(1 va_opt(5, __VA_ARGS__) va_at(5, __VA_ARGS__), t, f)

/*************************************************************************************************
 * __print__
 *************************************************************************************************/
#define print(fmt, ...)   fprintf(stdout, fmt __VA_OPT__(, ) __VA_ARGS__)
#define println(fmt, ...) fprintf(stdout, fmt "\n" __VA_OPT__(, ) __VA_ARGS__)

#ifdef NDEBUG
#  define __prt(fmt, ...)
#else
#  define __prt(fmt, ...) fprintf(stderr, fmt __VA_OPT__(, ) __VA_ARGS__)
#endif

#define __file__ __FILE__
#define __line__ __LINE__

#define __inf(fmt, ...)                                                                            \
  __prt("\x1b[02m[%s $%d %s]\x1b[0m: " fmt, __file__, __line__, __func__ __VA_OPT__(, ) __VA_ARGS__)

#define inf(fmt, ...) __inf(fmt "\n", __VA_ARGS__)
#define err(fmt, ...)                                                                              \
  do {                                                                                             \
    const char* __ERROR__ = (errno != 0) ? strerror(errno) : "no errno";                           \
    inf("(\x1b[31m%s\x1b[0m) " fmt, __ERROR__ __VA_OPT__(, ) __VA_ARGS__);                         \
    noused(__ERROR__);                                                                             \
  } while (0)

/*************************************************************************************************
 * __debug__
 *************************************************************************************************/
/* clang-format off */
#define u_assert(expr)                                                                             \
  do {                                                                                             \
    if (expr) {                                                                                    \
      fprintf(stderr, "Assertion failed: %s (%s: %s: %d)\n", #expr, __file__, __func__, __line__); \
    }                                                                                              \
  } while (0)

#define u_if(expr, arg...)              do { if (expr) { err("(%s) " va_0th("", arg), #expr va_slice(1, arg));                        } } while (0)
#define u_die_if(expr, arg...)          do { if (expr) { err("(%s) " va_0th("", arg), #expr va_slice(1, arg)); exit(EXIT_FAILURE);    } } while (0)
#define u_ret_if(expr, code, arg...)    do { if (expr) { err("(%s) " va_0th("", arg), #expr va_slice(1, arg)); return  code;          } } while (0)
#define u_err_if(expr, arg...)          do { if (expr) { err("(%s) " va_1th("", arg), #expr va_slice(2, arg)); goto va_0th(err, arg); } } while (0)
#define u_free_if(mem)                  do { if ((mem) != nullptr) { u_free(mem);                                                     } } while (0)
#define u_close_if(fd)                  do { if ((fd) >= 0) { close(fd);                                                              } } while (0)
#define u_fclose_if(fp)                 do { if ((fp) != nullptr) { fclose(fd);                                                       } } while (0)

#define u_mem_if(mem, arg...)           u_err_if((mem) == nullptr, err, arg)
#define u_ret_no_if(expr, arg...)       u_ret_if(expr, , arg)

/* clang-format on */

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
#define fnt(n, r, ...) r (*(n))(__VA_ARGS__)

typedef void* any_t;

/*
 * a > b  ->  1
 * a == b ->  0
 * a < b  -> -1
 * */
typedef fnt(cmp_fn, int, const void*, const void*);
typedef fnt(eq_fn, bool, const void*, const void*);

/* clang-format off */
typedef int ret_t;

typedef float       f32_t;
typedef double      f64_t;
typedef long double f128_t;

typedef _Atomic(int8_t)              atomic_i8_t;
typedef _Atomic(uint8_t)             atomic_u8_t;
typedef _Atomic(int16_t)             atomic_i16_t;
typedef _Atomic(uint16_t)            atomic_u16_t;
typedef _Atomic(int32_t)             atomic_i32_t;
typedef _Atomic(uint32_t)            atomic_u32_t;
typedef _Atomic(int64_t)             atomic_i64_t;
typedef _Atomic(uint64_t)            atomic_u64_t;
/* clang-format on */

/*************************************************************************************************
 * __misc__
 *************************************************************************************************/
#define noused(x) ((void)(x))
#define chr(c)    (as(c, char))
#define any(p)    (as(p, any_t))

#ifdef USE_LAMBDA
#  define lm(ret, ...) ^ret(__VA_ARGS__)
#endif

#define me(v)    (&(struct { typeof(v) _; }){(v)})
#define as(v, T) ((T)(v))
#define rs(...)  #__VA_ARGS__

#define align_of(addr, size) ({ ((addr) + (size)-1) & (~((size)-1)); })
#define container_of(ptr, type, member)                                                            \
  ({                                                                                               \
    const typeof(((type*)0)->member)* _container_of__mptr = any(ptr);                              \
    (type*)((char*)_container_of__mptr - offsetof(type, member));                                  \
  })

#define swap(a, b)                                                                                 \
  do {                                                                                             \
    auto _swap__tmp = a;                                                                           \
    (a)             = (b);                                                                         \
    (b)             = (_swap__tmp);                                                                \
  } while (0)

/* array utils function */
#define arr_len(a) (sizeof(a) / sizeof((a)[0]))

#define arr_for(arr, i) for (size_t i = 0; (i) < arr_len(arr); (i)++)

#define arr_sort(arr, size, fn) qsort(arr, size, sizeof((arr)[0]), fn)

#define arr_min(ptr, size, fn)                                                                     \
  ({                                                                                               \
    size_t _arr_min__idx = 0;                                                                      \
    for (size_t _arr_min__i = 0; _arr_min__i < (size); _arr_min__i++) {                            \
      if (fn(any(&(ptr)[_arr_min__idx]), any(&(ptr)[_arr_min__i])) == 1)                           \
        _arr_min__idx = _arr_min__i;                                                               \
    }                                                                                              \
                                                                                                   \
    _arr_min__idx;                                                                                 \
  })

#define arr_max(ptr, size, fn)                                                                     \
  ({                                                                                               \
    size_t _arr_max__idx = 0;                                                                      \
    for (size_t _arr_max__i = 0; _arr_max__i < (size); _arr_max__i++) {                            \
      if (fn(any(&(ptr)[_arr_max__idx]), any(&(ptr)[_arr_max__i])) == -1)                          \
        _arr_max__idx = _arr_max__i;                                                               \
    }                                                                                              \
                                                                                                   \
    _arr_max__idx;                                                                                 \
  })

#define fn_eq_use(name) (fn_eq_##name)
#define fn_eq_dec(name) bool fn_eq_##name(const void* _x, const void* _y)
#define fn_eq_def(name, type, code)                                                                \
  fn_eq_dec(name) {                                                                                \
    auto x = *(type*)_x;                                                                           \
    auto y = *(type*)_y;                                                                           \
    return (code);                                                                                 \
  }

#define fn_cmp_use(name) (fn_cmp_##name)
#define fn_cmp_dec(name) int fn_cmp_##name(const void* _x, const void* _y)
#define fn_cmp_def(name, type, code)                                                               \
  fn_cmp_dec(name) {                                                                               \
    auto x = *(type*)_x;                                                                           \
    auto y = *(type*)_y;                                                                           \
    if (fn_eq_use(name)(_x, _y)) {                                                                 \
      return 0;                                                                                    \
    }                                                                                              \
    return (code) ? 1 : -1;                                                                        \
  }

static fn_eq_def(bool, bool, (x == y));
static fn_eq_def(char, char, (x == y));
static fn_eq_def(int, int, (x == y));
static fn_eq_def(int8, int8_t, (x == y));
static fn_eq_def(int16, int16_t, (x == y));
static fn_eq_def(int32, int32_t, (x == y));
static fn_eq_def(int64, int64_t, (x == y));
static fn_eq_def(uint8, uint8_t, (x == y));
static fn_eq_def(uint16, uint16_t, (x == y));
static fn_eq_def(uint32, uint32_t, (x == y));
static fn_eq_def(uint64, uint64_t, (x == y));

static fn_cmp_def(bool, bool, (x > y));
static fn_cmp_def(char, char, (x > y));
static fn_cmp_def(int, int, (x > y));
static fn_cmp_def(int8, int8_t, (x > y));
static fn_cmp_def(int16, int16_t, (x > y));
static fn_cmp_def(int32, int32_t, (x > y));
static fn_cmp_def(int64, int64_t, (x > y));
static fn_cmp_def(uint8, uint8_t, (x > y));
static fn_cmp_def(uint16, uint16_t, (x > y));
static fn_cmp_def(uint32, uint32_t, (x > y));
static fn_cmp_def(uint64, uint64_t, (x > y));

/* clang-format off */
#define min(x, y) ({ auto __min_x__ = (x); auto __min_y__ = (y); noused(&__min_x__ == &__min_y__); __min_x__ < __min_y__ ? __min_x__ : __min_y__; })
#define max(x, y) ({ auto __max_x__ = (x); auto __max_y__ = (y); noused(&__max_x__ == &__max_y__); __max_x__ < __max_y__ ? __max_x__ : __max_y__; })
/* clang-format on */

/*************************************************************************************************
 * __alloc__
 *************************************************************************************************/
/* clang-format off */
#ifdef USE_MIMALLOC
#  include <mimalloc.h>
#  define u_free(ptr)               ({ mi_free(ptr); })
#  define u_malloc(size)            ({ any_t _ptr = mi_malloc(size);        if (errno == 2) { errno = 0; } _ptr; })
#  define u_calloc(count, size)     ({ any_t _ptr = mi_calloc(count, size); if (errno == 2) { errno = 0; } _ptr; })
#  define u_realloc(ptr, size)      ({ any_t _ptr = mi_realloc(ptr, size);  if (errno == 2) { errno = 0; } _ptr; })
#  define u_zalloc(size)            ({ any_t _ptr = mi_calloc(1, size);     if (errno == 2) { errno = 0; } _ptr; })
#  define u_talloc(size, type)      ({ any_t _ptr = mi_calloc(1, size);     if (errno == 2) { errno = 0; } (type)_ptr; })
#else
#  define u_free(ptr)               ({ free(ptr); })
#  define u_malloc(size)            ({ any_t _ptr = malloc(size);        _ptr; })
#  define u_calloc(count, size)     ({ any_t _ptr = calloc(count, size); _ptr; })
#  define u_realloc(ptr, size)      ({ any_t _ptr = realloc(ptr, size);  _ptr; })
#  define u_zalloc(size)            ({ any_t _ptr = calloc(1, size);     _ptr; })
#  define u_talloc(size, type)      ({ any_t _ptr = calloc(1, size);     (type)_ptr; })
#endif
/* clang-format on */

/*************************************************************************************************
 * function
 *************************************************************************************************/
static void __inf_hex(const uint8_t* arr, size_t size);
#define inf_hex(arr, size)                                                                         \
  do {                                                                                             \
    __inf("\x1b[36;1m%s\x1b[0m(%ld)\n", #arr, size);                                               \
    __inf_hex(as(arr, uint8_t*), size);                                                            \
  } while (0)

static void __inf_hex(const uint8_t* arr, size_t size) {
  char buf[17] = {0};
  size_t i     = 0;
  size_t pos   = 0;
  size_t idx   = 0;

  for (; pos < size; pos++) {
    if ((pos % 16) == 0) {
      printf("   %08lx: ", pos);
    }

    printf("%02x %s", arr[pos] & 0xff, (pos % 16) == 7 ? " " : "");
    if ((pos % 16) == 15) {
      for (i = 0; i < arr_len(buf); i++) {
        idx    = pos - 16 + i;
        buf[i] = isprint(arr[idx]) ? arr[idx] : '.';
      }

      buf[i - 1] = '\0';
      printf("%s\n", buf);
    }
  }

  if ((pos % 16) != 0) {
    for (i = 0; i < pos % 16 + 2; i++) {
      idx    = pos - (pos % 16) + i - 1;
      buf[i] = isprint(arr[idx]) ? arr[idx] : '.';
    }

    buf[i - 1] = '\0';
    printf("%*s\n", 47 - 3 * ((int)pos % 16) + (int)i, buf);
  }
}
