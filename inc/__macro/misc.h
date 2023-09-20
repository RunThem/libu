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

/* array utils function */
#define arr_len(a)              (sizeof(a) / sizeof((a)[0]))
#define arr_for(arr, i)         for (ssize_t i = 0; (i) < arr_len(arr); (i)++)
#define arr_rfor(arr, i)        for (ssize_t i = arr_len(arr); (i) >= 0; (i)--)
#define arr_sort(arr, size, fn) qsort(arr, size, sizeof((arr)[0]), fn)

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

#define inf_hex(arr, size)                                                                         \
  do {                                                                                             \
    __inf("\x1b[36;1m%s\x1b[0m(%ld)\n", #arr, size);                                               \
    __inf_hex(as(arr, uint8_t*), size);                                                            \
  } while (0)

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
