#define noused(x) ((void)(x))
#define chr(c)    (as(c, char))
#define any(p)    (as(p, any_t))

#ifdef USE_LAMBDA
#  define lm(ret, ...) ^ret(__VA_ARGS__)
#endif

#define as(v, type)   ((type)(v))
#define rs(...)       #__VA_ARGS__
#define me(type, ...) ((type){__VA_ARGS__})

#define bit(byte, n) (((byte) >> (n)) & 1)

#define each(i, num) for (size_t i = 0; (i) < (num); (i)++)

#define align_of(addr, size) ({ ((addr) + (size)-1) & (~((size)-1)); })
#define container_of(ptr, type, member)                                                            \
  ({                                                                                               \
    const typeof(((type*)0)->member)* _container_of__mptr = any(ptr);                              \
    (type*)((char*)_container_of__mptr - offsetof(type, member));                                  \
  })

/* clang-format off */
#define nsec_of(tv)         ((tv).tv_sec * 1000000000 + (tv).tv_nsec) /* timespec */
#define nsec_diff(tv1, tv2) (nsec_of(tv1) - nsec_of(tv2))
typedef struct {
  char* msg;
  struct timespec begin;
  struct timespec end;
  size_t diff;

  size_t i;
  size_t n;
} __tack_t;

extern bool __benchmark(__tack_t* tack);
#define benchmark(...)                                                                                 \
  for (__tack_t T = {__VA_ARGS__}; (T.i != 0 && T.i != T.n) || __benchmark(&T); T.i++)
/* clang-format on */

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

/*
 * '==' => true
 * '!=' => false
 * */
#define fn_eq(type) fn_eq_##type

/*
 * '>'  => 1
 * '==' => 0
 * '<'  => -1*/
#define fn_cmp(type) (fn_cmp_##type)

#define fn_compe_dec(type)                                                                         \
  extern bool fn_eq_##type(const void*, const void*);                                              \
  extern int fn_cmp_##type(const void*, const void*);

#define fn_compe_def(type, eq, cmp)                                                                \
  bool fn_eq_##type(const void* _x, const void* _y) {                                              \
    type x = *(type*)_x, y = *(type*)_y;                                                           \
    return (eq);                                                                                   \
  }                                                                                                \
  int fn_cmp_##type(const void* _x, const void* _y) {                                              \
    type x = *(type*)_x, y = *(type*)_y;                                                           \
    return (eq) ? 0 : ((cmp) ? 1 : -1);                                                            \
  }

/*
 * cmp & eq func from base type
 * */
fn_compe_def(char, (x == y), (x > y));
fn_compe_def(int, (x == y), (x > y));

fn_compe_def(i8_t, (x == y), (x > y));
fn_compe_def(u8_t, (x == y), (x > y));
fn_compe_def(i16_t, (x == y), (x > y));
fn_compe_def(u16_t, (x == y), (x > y));
fn_compe_def(i32_t, (x == y), (x > y));
fn_compe_def(u32_t, (x == y), (x > y));
fn_compe_def(i64_t, (x == y), (x > y));
fn_compe_def(u64_t, (x == y), (x > y));

fn_compe_def(size_t, (x == y), (x > y));
fn_compe_def(ssize_t, (x == y), (x > y));

#ifdef __SIZEOF_INT128__
fn_compe_def(i128_t, (x == y), (x > y));
fn_compe_def(u128_t, (x == y), (x > y));
#endif
