#pragma once

#define noused(x) ((void)(x))
#define chr(c)    (as(c, char))
#define any(p)    (as(p, any_t))

#ifdef USE_LAMBDA
#  define lm(ret, ...) ^ret(__VA_ARGS__)
#endif

#define as(v, T)              ((T)(v))
#define rs(...)               #__VA_ARGS__
#define me(type, __VA_ARGS__) (&(type){__VA_ARGS__})

#define bit(byte, n) (((byte) >> (n)) & 1)

#define each(i, start, end, step) for (ssize_t i = (start); (i) < (end); (i) += (step))

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

#if 0 /* linux array size macro */
#  define __same_type(a, b)      __builtin_types_compatible_p(typeof(a), typeof(b))
#  define __build_bug_on_zero(e) (sizeof(struct { int : -!!(e); }))
#  define __must_be_array(a)     __build_bug_on_zero(__same_type((a), &(a)[0]))
#  define array_size(arr)        (sizeof(arr) / sizeof((arr)[0]) + __must_be_array(arr))
#endif

/* array utils function */
#define arr_len(arr)            (sizeof(arr) / sizeof((arr)[0]))
#define arr_for(arr, i)         for (ssize_t i = 0; (i) < arr_len(arr); (i)++)
#define arr_rfor(arr, i)        for (ssize_t i = arr_len(arr); (i) >= 0; (i)--)
#define arr_sort(arr, size, fn) qsort(arr, size, sizeof((arr)[0]), fn)

#define min(x, y)                                                                                  \
  ({                                                                                               \
    auto __min_x__ = (x);                                                                          \
    auto __min_y__ = (y);                                                                          \
    noused(&__min_x__ == &__min_y__);                                                              \
    __min_x__ < __min_y__ ? __min_x__ : __min_y__;                                                 \
  })

#define max(x, y)                                                                                  \
  ({                                                                                               \
    auto __max_x__ = (x);                                                                          \
    auto __max_y__ = (y);                                                                          \
    noused(&__max_x__ == &__max_y__);                                                              \
    __max_x__ > __max_y__ ? __max_x__ : __max_y__;                                                 \
  })

#define fn_eq_use(name) (fn_eq_##name)
#define fn_eq_dec(name) bool fn_eq_##name(const void* _x, const void* _y)
#define fn_eq_def(name, type, code)                                                                \
  fn_eq_dec(name) {                                                                                \
    auto x = *(type*)_x;                                                                           \
    auto y = *(type*)_y;                                                                           \
                                                                                                   \
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
                                                                                                   \
    return (code) ? 1 : -1;                                                                        \
  }

extern fn_eq_dec(bool);
extern fn_eq_dec(char);
extern fn_eq_dec(int);
extern fn_eq_dec(int8);
extern fn_eq_dec(int16);
extern fn_eq_dec(int32);
extern fn_eq_dec(int64);
extern fn_eq_dec(uint8);
extern fn_eq_dec(uint16);
extern fn_eq_dec(uint32);
extern fn_eq_dec(uint64);

extern fn_cmp_dec(bool);
extern fn_cmp_dec(char);
extern fn_cmp_dec(int);
extern fn_cmp_dec(int8);
extern fn_cmp_dec(int16);
extern fn_cmp_dec(int32);
extern fn_cmp_dec(int64);
extern fn_cmp_dec(uint8);
extern fn_cmp_dec(uint16);
extern fn_cmp_dec(uint32);
extern fn_cmp_dec(uint64);