#pragma once
#include <u/u.h>

/***************************************************************************************************
 * iType
 **************************************************************************************************/
typedef struct {
}* u_vec_t;

typedef struct {
}* u_tbl_t;

typedef struct {
}* u_avl_t;

typedef fnt(u_avl_cmp_fn, int, const void*, const void*);

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
extern u_vec_t vec_new(size_t);
extern u_tbl_t tbl_new(size_t, size_t);
extern u_avl_t avl_new(size_t, size_t, u_avl_cmp_fn);

extern size_t vec_len(u_vec_t);
extern size_t tbl_len(u_tbl_t);
extern size_t avl_len(u_avl_t);

extern size_t vec_cap(u_vec_t);

extern bool tbl_isexist(u_tbl_t, any_t);
extern bool vec_isexist(u_vec_t, size_t);
extern bool avl_isexist(u_avl_t, any_t);

extern void vec_clear(u_vec_t);
extern void avl_clear(u_avl_t);
extern void tbl_clear(u_tbl_t);

extern void vec_cleanup(u_vec_t);
extern void tbl_cleanup(u_tbl_t);
extern void avl_cleanup(u_avl_t);

extern void vec_at(u_vec_t, size_t, any_t);
extern void tbl_at(u_tbl_t, any_t, any_t);
extern void avl_at(u_avl_t, any_t, any_t);

extern void vec_re(u_vec_t, size_t, any_t);
extern void tbl_re(u_tbl_t, any_t, any_t);
extern void avl_re(u_avl_t, any_t, any_t);

extern void vec_pop(u_vec_t, size_t, any_t);
extern void tbl_pop(u_tbl_t, any_t, any_t);
extern void avl_pop(u_avl_t, any_t, any_t);

extern void vec_put(u_vec_t, ssize_t, any_t);
extern void tbl_put(u_tbl_t, any_t, any_t);
extern void avl_put(u_avl_t, any_t, any_t);

extern bool vec_range(u_vec_t, ssize_t*, bool, any_t);
extern void tbl_range_init(u_tbl_t);
extern bool tbl_range(u_tbl_t, any_t, any_t);
extern void avl_range_init(u_avl_t);
extern bool avl_range(u_avl_t, bool, any_t, any_t);

typedef void** invalied_type_t;
/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define iu_vec(T)                                                                                  \
  struct [[gnu::packed]] {                                                                         \
    struct {                                                                                       \
      u_vec_t mate;                                                                                \
      ssize_t a;                                                                                   \
      T b;                                                                                         \
    } _;                                                                                           \
                                                                                                   \
    T it;                                                                                          \
  }*

#define iu_tbl(K, V)                                                                               \
  struct [[gnu::packed]] {                                                                         \
    struct {                                                                                       \
      u_tbl_t mate;                                                                                \
      K a;                                                                                         \
      V b;                                                                                         \
    } _;                                                                                           \
                                                                                                   \
    K key;                                                                                         \
    V val;                                                                                         \
  }*

#define iu_avl(K, V)                                                                               \
  struct [[gnu::packed]] {                                                                         \
    struct {                                                                                       \
      u_avl_t mate;                                                                                \
      K a;                                                                                         \
      V b;                                                                                         \
    } _;                                                                                           \
                                                                                                   \
    K key;                                                                                         \
    V val;                                                                                         \
  }*

/***************************************************************************************************
 * iApi
 **************************************************************************************************/

#define igeneric(mate, vec, tbl, avl, args...)                                                     \
  _Generic(mate,                                                                                   \
      u_vec_t: vec,                                                                                \
      u_tbl_t: tbl,                                                                                \
      u_avl_t: avl va_if(va_has(args))(, default                                                   \
                                       : va_at(0, args)))

#define typeeq(t1, t2) (__builtin_types_compatible_p(typeof(t1), typeof(t2)))

#define iu_init(u, args...)                                                                        \
  do {                                                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    u = u_zalloc(sizeof(*u));                                                                      \
                                                                                                   \
    if (typeeq(u->_.mate, u_vec_t)) {                                                              \
      u->_.mate = (any_t)vec_new(sizeof(u->_.a));                                                  \
    } else if (typeeq(u->_.mate, u_tbl_t)) {                                                       \
      u->_.mate = (any_t)tbl_new(sizeof(u->_.a), sizeof(u->_.b));                                  \
    } else if (typeeq(u->_.mate, u_avl_t)) {                                                       \
      u->_.mate = (any_t)avl_new(sizeof(u->_.a), sizeof(u->_.b), va_0th(nullptr, args));           \
    }                                                                                              \
  } while (0)

#define iu_new(u, args...)                                                                         \
  ({                                                                                               \
    iu_init(u, args);                                                                              \
                                                                                                   \
    u;                                                                                             \
  })

#define iu_len(u)                                                                                  \
  ({                                                                                               \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_len, tbl_len, avl_len);                                      \
                                                                                                   \
    fn(u->_.mate);                                                                                 \
  })

#define iu_cap(u)                                                                                  \
  ({                                                                                               \
    static_assert(igeneric(u->_.mate, 1, 0, 0, 0));                                                \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_cap, nullptr, nullptr);                                      \
                                                                                                   \
    fn(u->_.mate);                                                                                 \
  })

#define iu_isinit(u)                                                                               \
  ({                                                                                               \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    u != nullptr && u->_.mate != nullptr;                                                          \
  })

#define iu_isexist(u, args...)                                                                     \
  ({                                                                                               \
    static_assert(va_size(args) == 1);                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_isexist, tbl_isexist, avl_isexist);                          \
                                                                                                   \
    fn(u->_.mate, igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a));                                  \
  })

#define iu_isempty(u)                                                                              \
  ({                                                                                               \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    iu_len(u) == 0;                                                                                \
  })

#define iu_clear(u)                                                                                \
  do {                                                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_clear, tbl_clear, avl_clear);                                \
                                                                                                   \
    fn(u->_.mate);                                                                                 \
  } while (0)

#define iu_cleanup(u)                                                                              \
  do {                                                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_cleanup, tbl_cleanup, avl_cleanup);                          \
                                                                                                   \
    fn(u->_.mate);                                                                                 \
                                                                                                   \
    u_free(u);                                                                                     \
    u = nullptr;                                                                                   \
  } while (0)

#define iu_at(u, args...)                                                                          \
  ({                                                                                               \
    static_assert(va_size(args) == 1);                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_at, tbl_at, avl_at);                                         \
                                                                                                   \
    fn(u->_.mate, igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a), &u->_.b);                         \
                                                                                                   \
    u->_.b;                                                                                        \
  })

#define iu_re(u, args...)                                                                          \
  do {                                                                                             \
    static_assert(va_size(args) == 2);                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
    u->_.b = va_at(1, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_re, tbl_re, avl_re);                                         \
                                                                                                   \
    fn(u->_.mate, igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a), &u->_.b);                         \
  } while (0)

#define iu_pop(u, args...)                                                                         \
  ({                                                                                               \
    static_assert(va_size(args) == 1);                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_pop, tbl_pop, avl_pop);                                      \
                                                                                                   \
    fn(u->_.mate, igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a), &u->_.b);                         \
                                                                                                   \
    u->_.b;                                                                                        \
  })

#define iu_put(u, args...)                                                                         \
  do {                                                                                             \
    static_assert(va_size(args) == 2);                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0));                                                \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
    u->_.b = va_at(1, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_put, tbl_put, avl_put);                                      \
                                                                                                   \
    fn(u->_.mate, igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a), &u->_.b);                         \
  } while (0)
