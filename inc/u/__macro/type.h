#include <stdint.h>

#define fnt(n, r, ...) r (*(n))(__VA_ARGS__)

/*
 * a >  b  ->  1
 * a == b  ->  0
 * a <  b  -> -1
 * */
typedef fnt(cmp_fn, int, const void*, const void*);
typedef fnt(eq_fn, bool, const void*, const void*);

/* clang-format off */

typedef int   ret_t;
typedef void* any_t;

typedef float       f32_t;
typedef double      f64_t;
typedef long double f128_t;

typedef _Atomic(int8_t)   atomic_i8_t;
typedef _Atomic(uint8_t)  atomic_u8_t;
typedef _Atomic(int16_t)  atomic_i16_t;
typedef _Atomic(uint16_t) atomic_u16_t;
typedef _Atomic(int32_t)  atomic_i32_t;
typedef _Atomic(uint32_t) atomic_u32_t;
typedef _Atomic(int64_t)  atomic_i64_t;
typedef _Atomic(uint64_t) atomic_u64_t;

/* clang-format on */
