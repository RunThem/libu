#pragma once
#include <u/u.h>

/***************************************************************************************************
 * Type
 **************************************************************************************************/
typedef struct {
}* u_vec_t;

typedef struct {
}* u_tbl_t;

typedef struct {
}* u_avl_t;

typedef fnt(u_cmp_fn, int, const void*, const void*);

typedef(lstn_t) {
  lstn_t* prev;
  lstn_t* next;

  any_t item;
};

typedef struct {
}* u_lst_t;

typedef fnt(u_eq_fn, bool, const void*, const void*);

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern u_vec_t vec_new(size_t);
extern u_tbl_t tbl_new(size_t, size_t);
extern u_avl_t avl_new(size_t, size_t, u_cmp_fn);
extern u_lst_t lst_new(size_t);

extern size_t vec_len(u_vec_t);
extern size_t tbl_len(u_tbl_t);
extern size_t avl_len(u_avl_t);
extern size_t lst_len(u_lst_t);

extern size_t vec_cap(u_vec_t);

extern bool tbl_isexist(u_tbl_t, any_t);
extern bool vec_isexist(u_vec_t, size_t);
extern bool avl_isexist(u_avl_t, any_t);
extern bool lst_isexist(u_lst_t, any_t);

extern void vec_clear(u_vec_t);
extern void avl_clear(u_avl_t);
extern void tbl_clear(u_tbl_t);

extern void vec_cleanup(u_vec_t);
extern void tbl_cleanup(u_tbl_t);
extern void avl_cleanup(u_avl_t);
extern void lst_cleanup(u_lst_t);

extern void vec_at(u_vec_t, size_t, any_t);
extern void tbl_at(u_tbl_t, any_t, any_t);
extern void avl_at(u_avl_t, any_t, any_t);

extern void vec_re(u_vec_t, size_t, any_t);
extern void tbl_re(u_tbl_t, any_t, any_t);
extern void avl_re(u_avl_t, any_t, any_t);

extern void vec_pop(u_vec_t, size_t, any_t);
extern void tbl_pop(u_tbl_t, any_t, any_t);
extern void avl_pop(u_avl_t, any_t, any_t);
extern void lst_pop(u_lst_t, any_t, any_t);

extern void vec_put(u_vec_t, ssize_t, any_t);
extern void tbl_put(u_tbl_t, any_t, any_t);
extern void avl_put(u_avl_t, any_t, any_t);
extern void lst_put(u_lst_t, any_t, any_t);

extern void lst_first(u_lst_t, any_t);
extern void lst_last(u_lst_t, any_t);
extern void lst_next(u_lst_t, any_t, any_t);
extern void lst_prev(u_lst_t, any_t, any_t);

extern bool vec_for_init(u_vec_t, bool);
extern bool vec_for(u_vec_t, size_t*, any_t);
extern bool tbl_for_init(u_tbl_t, bool);
extern bool tbl_for(u_tbl_t, any_t, any_t);
extern bool avl_for_init(u_avl_t, bool);
extern bool avl_for(u_avl_t, any_t, any_t);
extern bool lst_for_init(u_lst_t, bool);
extern bool lst_for(u_lst_t, any_t, any_t);

typedef void** invalied_type_t;
/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define u_vec(T)    typeof(u_vec_t(*(*)(size_t*, T*))[sizeof(size_t)][sizeof(T)])
#define u_tbl(K, V) typeof(u_tbl_t(*(*)(K*, V*))[sizeof(K)][sizeof(V)])
#define u_avl(K, V) typeof(u_avl_t(*(*)(K*, V*))[sizeof(K)][sizeof(V)])
#define u_lst(T)    typeof(u_lst_t(*(*)(T*, T*))[sizeof(T)][sizeof(T)])

/***************************************************************************************************
 * iApi
 **************************************************************************************************/
#define igeneric(mate, vec, tbl, avl, lst, args...)                                                \
  _Generic(mate,                                                                                   \
      u_vec_t: vec,                                                                                \
      u_tbl_t: tbl,                                                                                \
      u_avl_t: avl,                                                                                \
      u_lst_t: lst va_if(va_has(args))(, default                                                   \
                                       : va_at(0, args)))

#define typeeq(t1, t2) (__builtin_types_compatible_p(typeof(t1), typeof(t2)))

#define u_init(u, ...)                                                                             \
  do {                                                                                             \
    typeof(u(nullptr, nullptr)[0][0][0]) _ = nullptr;                                              \
                                                                                                   \
    static_assert(igeneric(_, 1, 1, 1, 1, 0));                                                     \
                                                                                                   \
    auto fn = igeneric(_, vec_new, tbl_new, avl_new, lst_new);                                     \
                                                                                                   \
    u = any(fn(__VA_ARGS__));                                                                      \
  } while (0)

#define u_len(u)                                                                                   \
  ({                                                                                               \
    typeof(u(nullptr, nullptr)[0][0][0]) _ = nullptr;                                              \
                                                                                                   \
    static_assert(igeneric(_, 1, 1, 1, 1, 0));                                                     \
                                                                                                   \
    auto fn = igeneric(_, vec_len, tbl_len, avl_len, lst_len);                                     \
                                                                                                   \
    fn(any(u));                                                                                    \
  })

#define u_cap(u)                                                                                   \
  ({                                                                                               \
    typeof(u(nullptr, nullptr)[0][0][0]) _ = nullptr;                                              \
                                                                                                   \
    static_assert(igeneric(_, 1, 0, 0, 0, 0));                                                     \
                                                                                                   \
    auto fn = igeneric(_, vec_cap, nullptr, nullptr, nullptr);                                     \
                                                                                                   \
    fn(any(u));                                                                                    \
  })

#define u_exist(u, ...)                                                                            \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1);                                                      \
                                                                                                   \
    auto _x                            = va_at(0, __VA_ARGS__);                                    \
    typeof(u(&_x, nullptr)[0][0][0]) _ = nullptr;                                                  \
                                                                                                   \
    static_assert(igeneric(_, 1, 1, 1, 1, 0));                                                     \
                                                                                                   \
    auto fn = igeneric(_, vec_isexist, tbl_isexist, avl_isexist, lst_isexist);                     \
                                                                                                   \
    fn(any(u), igeneric(_, _x, &_x, &_x, _x));                                                     \
  })

#define u_empty(u)                                                                                 \
  ({                                                                                               \
    typeof(u(nullptr, nullptr)[0][0][0]) _ = nullptr;                                              \
                                                                                                   \
    static_assert(igeneric(_, 1, 1, 1, 1, 0));                                                     \
                                                                                                   \
    auto fn = igeneric(_, vec_len, tbl_len, avl_len, lst_len);                                     \
                                                                                                   \
    0 == fn(any(u));                                                                               \
  })

#define u_clear(u)                                                                                 \
  do {                                                                                             \
    typeof(u(nullptr, nullptr)[0][0][0]) _ = nullptr;                                              \
                                                                                                   \
    static_assert(igeneric(_, 1, 1, 1, 0, 0));                                                     \
                                                                                                   \
    auto fn = igeneric(_, vec_clear, tbl_clear, avl_clear, nullptr);                               \
                                                                                                   \
    fn(any(u));                                                                                    \
  } while (0)

#define u_cleanup(u)                                                                               \
  do {                                                                                             \
    typeof(u(nullptr, nullptr)[0][0][0]) _ = nullptr;                                              \
                                                                                                   \
    static_assert(igeneric(_, 1, 1, 1, 1, 0));                                                     \
                                                                                                   \
    auto fn = igeneric(_, vec_cleanup, tbl_cleanup, avl_cleanup, lst_cleanup);                     \
                                                                                                   \
    fn(any(u));                                                                                    \
                                                                                                   \
    u_free(any(u));                                                                                \
    u = nullptr;                                                                                   \
  } while (0)

#define u_at(u, ...)                                                                               \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1);                                                      \
                                                                                                   \
    auto _x                           = va_at(0, args);                                            \
    typeof(u(_x, nullptr)[0][0][0]) _ = nullptr;                                                   \
                                                                                                   \
    static_assert(igeneric(_, 1, 1, 1, 0, 0));                                                     \
                                                                                                   \
    auto fn = igeneric(_, vec_at, tbl_at, avl_at, nullptr);                                        \
                                                                                                   \
    fn(any(u), igeneric(_, _x, &_x, &_x, nullptr), &u->_.b);                                       \
                                                                                                   \
    u->_.b;                                                                                        \
  })

#define u_re(u, args...)                                                                           \
  do {                                                                                             \
    auto _ = u(&x, nullptr)[0][0][0];                                                              \
    static_assert(va_size(args) == 2);                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 0, 0));                                             \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
    u->_.b = va_at(1, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_re, tbl_re, avl_re, nullptr);                                \
                                                                                                   \
    fn(u->_.mate, igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a, nullptr), &u->_.b);                \
  } while (0)

#define u_pop(u, args...)                                                                          \
  ({                                                                                               \
    auto _ = u(&x, nullptr)[0][0][0];                                                              \
    static_assert(va_size(args) == 1);                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 1, 0));                                             \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_pop, tbl_pop, avl_pop, lst_pop);                             \
                                                                                                   \
    fn(u->_.mate, igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a, u->_.a), &u->_.b);                 \
                                                                                                   \
    u->_.b;                                                                                        \
  })

#define u_put(u, args...)                                                                          \
  do {                                                                                             \
    auto _ = u(&x, nullptr)[0][0][0];                                                              \
    static_assert(va_size(args) == 2);                                                             \
    static_assert(igeneric(u->_.mate, 1, 1, 1, 1, 0));                                             \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
    u->_.b = va_at(1, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, vec_put, tbl_put, avl_put, lst_put);                             \
                                                                                                   \
    fn(u->_.mate,                                                                                  \
       igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a, u->_.a),                                      \
       igeneric(u->_.mate, &u->_.b, &u->_.b, &u->_.b, u->_.b));                                    \
  } while (0)

#define u_first(u)                                                                                 \
  ({                                                                                               \
    auto _ = u(&x, nullptr)[0][0][0];                                                              \
    static_assert(igeneric(u->_.mate, 0, 0, 0, 1, 0));                                             \
                                                                                                   \
    auto fn = igeneric(u->_.mate, nullptr, nullptr, nullptr, lst_first);                           \
                                                                                                   \
    fn(u->_.mate, &u->_.b);                                                                        \
                                                                                                   \
    u->_.b;                                                                                        \
  })

#define u_last(u)                                                                                  \
  ({                                                                                               \
    auto _ = u(&x, nullptr)[0][0][0];                                                              \
    static_assert(igeneric(u->_.mate, 0, 0, 0, 1, 0));                                             \
                                                                                                   \
    auto fn = igeneric(u->_.mate, nullptr, nullptr, nullptr, lst_last);                            \
                                                                                                   \
    fn(u->_.mate, &u->_.b);                                                                        \
                                                                                                   \
    u->_.b;                                                                                        \
  })

#define u_next(u, args...)                                                                         \
  ({                                                                                               \
    auto _ = u(&x, nullptr)[0][0][0];                                                              \
    static_assert(va_size(args) == 1);                                                             \
    static_assert(igeneric(u->_.mate, 0, 0, 0, 1, 0));                                             \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, nullptr, nullptr, nullptr, lst_next);                            \
                                                                                                   \
    fn(u->_.mate, &u->_.a, &u->_.b);                                                               \
                                                                                                   \
    u->_.b;                                                                                        \
  })

#define u_prev(u, args...)                                                                         \
  ({                                                                                               \
    auto _ = u(&x, nullptr)[0][0][0];                                                              \
    static_assert(va_size(args) == 1);                                                             \
    static_assert(igeneric(u->_.mate, 0, 0, 0, 1, 0));                                             \
                                                                                                   \
    u->_.a = va_at(0, args);                                                                       \
                                                                                                   \
    auto fn = igeneric(u->_.mate, nullptr, nullptr, nullptr, lst_prev);                            \
                                                                                                   \
    fn(u->_.mate, &u->_.a, &u->_.b);                                                               \
                                                                                                   \
    u->_.b;                                                                                        \
  })

#define u_for(u, idx, it)                                                                          \
  for (typeof(u->_.a) idx;                                                                         \
       igeneric(u->_.mate, vec_for_init, tbl_for_init, avl_for_init, lst_for_init)(u->_.mate, 1);) \
    for (typeof(u->_.b) it;                                                                        \
         igeneric(u->_.mate, vec_for, tbl_for, avl_for, lst_for)(u->_.mate, any(&idx), &it);)

#define u_rfor(u, idx, it)                                                                         \
  for (typeof(u->_.a) idx;                                                                         \
       igeneric(u->_.mate, vec_for_init, tbl_for_init, avl_for_init, lst_for_init)(u->_.mate, 0);) \
    for (typeof(u->_.b) it;                                                                        \
         igeneric(u->_.mate, vec_for, tbl_for, avl_for, lst_for)(u->_.mate, any(&idx), &it);)
