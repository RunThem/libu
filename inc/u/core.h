#pragma once

#include "core/iavl.h"
#include "core/itbl.h"
#include "core/ivec.h"

typedef void** invalied_type_t;
/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define u_vec(T)                                                                                   \
  struct [[gnu::packed]] {                                                                         \
    struct {                                                                                       \
      u_vec_t mate;                                                                                \
      typeof(T*) a;                                                                                \
      typeof(T*) b;                                                                                \
    } _;                                                                                           \
                                                                                                   \
    T it;                                                                                          \
  }*

#define u_tbl(K, V)                                                                                \
  struct [[gnu::packed]] {                                                                         \
    struct {                                                                                       \
      u_tbl_t mate;                                                                                \
      typeof(K*) a;                                                                                \
      typeof(V*) b;                                                                                \
    } _;                                                                                           \
                                                                                                   \
    K key;                                                                                         \
    V val;                                                                                         \
  }*

#define u_avl(K, V)                                                                                \
  struct [[gnu::packed]] {                                                                         \
    struct {                                                                                       \
      u_avl_t mate;                                                                                \
      typeof(K*) a;                                                                                \
      typeof(V*) b;                                                                                \
    } _;                                                                                           \
                                                                                                   \
    K key;                                                                                         \
    V val;                                                                                         \
  }*

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#define typeeq(t1, t2) (__builtin_types_compatible_p(t1, t2))
#define __set(x, args...)                                                                          \
  do {                                                                                             \
    _Generic(x->mate,                                                                              \
        u_vec_t: ({                                                                                \
               static_assert(1 == va_size(args) || !typeeq(typeof(x->mate), u_vec_t));             \
                                                                                                   \
               *(x->a) = va_0th((typeof(*x->a)){}, args);                                          \
             }),                                                                                   \
        u_tbl_t: ({                                                                                \
               static_assert(2 == va_size(args) || !typeeq(typeof(x->mate), u_tbl_t));             \
                                                                                                   \
               *(x->a) = va_0th((typeof(*x->a)){}, args);                                          \
               *(x->b) = va_1th((typeof(*x->a)){}, args);                                          \
             }),                                                                                   \
        u_avl_t: ({                                                                                \
               static_assert(2 == va_size(args) || !typeeq(typeof(x->mate), u_avl_t));             \
                                                                                                   \
               *(x->a) = va_0th((typeof(*x->a)){}, args);                                          \
               *(x->b) = va_1th((typeof(*x->a)){}, args);                                          \
             }),                                                                                   \
        default: abort());                                                                         \
  } while (0)

#define __get(x, ...)                                                                              \
  ({                                                                                               \
    _Generic(x->mate,                                                                              \
        u_vec_t: (*(x->b)),                                                                        \
        u_tbl_t: (*(x->b)),                                                                        \
        u_avl_t: (*(x->b)),                                                                        \
        default: abort();                                                                          \
  })

#define u_init(u, args...)                                                                         \
  do {                                                                                             \
    (u) = u_zalloc(sizeof(typeof(*(u))) + sizeof(size_t));                                         \
                                                                                                   \
    _Generic((u)->_.mate,                                                                          \
        u_vec_t: ({                                                                                \
               static_assert((0 == va_size(args)) + !typeeq(typeof((u)->_.mate), u_vec_t));        \
                                                                                                   \
               (u)->_.mate = (any_t)vec_new(sizeof(*(u)->_.a));                                    \
               (u)->_.a    = any(u) + sizeof((u)->_);                                              \
               (u)->_.b    = any(u) + sizeof((u)->_) + sizeof(*(u)->_.a);                          \
             }),                                                                                   \
        u_tbl_t: ({                                                                                \
               static_assert((0 == va_size(args)) + !typeeq(typeof((u)->_.mate), u_tbl_t));        \
                                                                                                   \
               (u)->_.mate = (any_t)tbl_new(sizeof(*(u)->_.a), sizeof(*(u)->_.b));                 \
               (u)->_.a    = any(u) + sizeof((u)->_);                                              \
               (u)->_.b    = any(u) + sizeof((u)->_) + sizeof(*(u)->_.a);                          \
             }),                                                                                   \
        u_avl_t: ({                                                                                \
               static_assert((1 == va_size(args)) + !typeeq(typeof((u)->_.mate), u_avl_t));        \
                                                                                                   \
               (u)->_.mate =                                                                       \
                   (any_t)avl_new(sizeof(*(u)->_.a), sizeof(*(u)->_.b), va_0th(nullptr, args));    \
               (u)->_.a = any(u) + sizeof((u)->_);                                                 \
               (u)->_.b = any(u) + sizeof((u)->_) + sizeof(*(u)->_.a);                             \
             }));                                                                                  \
  } while (0)

#define u_len(u)                                                                                   \
  ({ _Generic((u)->_.mate, u_vec_t: vec_len, u_tbl_t: tbl_len, u_avl_t: avl_len)((u)->_.mate); })

#define u_cap(u) ({ _Generic((u)->_.mate, u_vec_t: vec_cap)((u)->_.mate); })

#define u_put(u, args...)                                                                          \
  do {                                                                                             \
    _Generic((u)->_.mate,                                                                          \
        u_vec_t: ({                                                                                \
               static_assert((2 == va_size(args)) + !typeeq(typeof((u)->_.mate), u_vec_t));        \
                                                                                                   \
               *(u)->_.a = va_at(1, args);                                                         \
               vec_put((any_t)(u)->_.mate, va_at(0, args), (u)->_.a);                              \
             }),                                                                                   \
        u_tbl_t: ({                                                                                \
               static_assert((2 == va_size(args)) + !typeeq(typeof((u)->_.mate), u_tbl_t));        \
                                                                                                   \
               *(u)->_.a = va_at(0, args);                                                         \
               *(u)->_.b = va_at(1, args);                                                         \
               tbl_put((any_t)(u)->_.mate, (u)->_.a, (u)->_.b);                                    \
             }),                                                                                   \
        u_avl_t: ({                                                                                \
               static_assert((2 == va_size(args)) + !typeeq(typeof((u)->_.mate), u_avl_t));        \
                                                                                                   \
               *(u)->_.a = va_at(0, args);                                                         \
               *(u)->_.b = va_at(1, args);                                                         \
               tbl_put((any_t)(u)->_.mate, (u)->_.a, (u)->_.b);                                    \
             }));                                                                                  \
  } while (0)

#define __u_put(u, args...)                                                                        \
  do {                                                                                             \
    *(u)->_.a = va_at(0, args);                                                                    \
    *(u)->_.b = va_at(1, args);                                                                    \
                                                                                                   \
    _Generic((u)->_.mate,                                                                          \
        u_vec_t: ({ vec_put((any_t)(u)->_.mate, *(u)->_.a, (u)->_.a); }),                           \
        u_tbl_t: ({ tbl_put((any_t)(u)->_.mate, (u)->_.a, (u)->_.b); }),                           \
        u_avl_t: ({ tbl_put((any_t)(u)->_.mate, (u)->_.a, (u)->_.b); }));                          \
  } while (0)
