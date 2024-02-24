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

#if 0
typedef(lstn_t) {
  lstn_t* prev;
  lstn_t* next;

  any_t item;
};
#endif

typedef struct {
}* u_lst_t;

typedef fnt(u_eq_fn, bool, const void*, const void*);

/***************************************************************************************************
 * Api
 **************************************************************************************************/
extern u_vec_t vec_new(size_t);
extern u_tbl_t tbl_new(size_t, size_t);
extern u_avl_t avl_new(size_t, size_t, u_cmp_fn);
// extern u_lst_t lst_new(size_t);

extern size_t vec_len(u_vec_t);
extern size_t tbl_len(u_tbl_t);
extern size_t avl_len(u_avl_t);
// extern size_t lst_len(u_lst_t);

extern size_t vec_cap(u_vec_t);

extern bool vec_exist(u_vec_t, size_t);
extern bool tbl_exist(u_tbl_t, any_t);
extern bool avl_exist(u_avl_t, any_t);
// extern bool lst_isexist(u_lst_t, any_t);

extern void vec_clear(u_vec_t);
extern void tbl_clear(u_tbl_t);
extern void avl_clear(u_avl_t);

extern void vec_cleanup(u_vec_t);
extern void tbl_cleanup(u_tbl_t);
extern void avl_cleanup(u_avl_t);
// extern void lst_cleanup(u_lst_t);

extern void vec_at(u_vec_t, size_t, any_t);
extern void tbl_at(u_tbl_t, any_t, any_t);
extern void avl_at(u_avl_t, any_t, any_t);

extern void vec_re(u_vec_t, size_t, any_t);
extern void tbl_re(u_tbl_t, any_t, any_t);
extern void avl_re(u_avl_t, any_t, any_t);

extern void vec_pop(u_vec_t, size_t, any_t);
extern void tbl_pop(u_tbl_t, any_t, any_t);
extern void avl_pop(u_avl_t, any_t, any_t);
// extern void lst_pop(u_lst_t, any_t, any_t);

extern void vec_put(u_vec_t, ssize_t, any_t);
extern void tbl_put(u_tbl_t, any_t, any_t);
extern void avl_put(u_avl_t, any_t, any_t);
// extern void lst_put(u_lst_t, any_t, any_t);

// extern void lst_first(u_lst_t, any_t);
// extern void lst_last(u_lst_t, any_t);
// extern void lst_next(u_lst_t, any_t, any_t);
// extern void lst_prev(u_lst_t, any_t, any_t);

extern any_t vec_each_init(u_vec_t, bool);
extern any_t tbl_each_init(u_tbl_t, bool);
extern any_t avl_each_init(u_avl_t, bool);
// extern bool lst_for_init(u_lst_t, bool);

extern bool vec_each(u_vec_t, size_t*, any_t);
extern bool tbl_each(u_tbl_t, any_t, any_t);
extern bool avl_each(u_avl_t, any_t, any_t);
// extern bool lst_for(u_lst_t, any_t, any_t);

typedef void** invalied_type_t;
/***************************************************************************************************
 * iType
 **************************************************************************************************/
#define uvec(T) typeof(T(*(*)(size_t*, T*))[sizeof(size_t)][sizeof(T)])

#define utbl(K, V) typeof(V(*(*)(K*, V*))[sizeof(K)][sizeof(V)])

#define uavl(K, V) typeof(V(*(*)(K*, V*))[sizeof(K)][sizeof(V)])

/***************************************************************************************************
 * iApi vec
 **************************************************************************************************/
#define uv_init(u)                                                                                 \
  do {                                                                                             \
    u = any(                                                                                       \
        vec_new(sizeof(typeof(**u(nullptr, nullptr))) / sizeof(typeof(***u(nullptr, nullptr)))));  \
  } while (0)

#define uv_len(u) ({ vec_len(any(u)); })

#define uv_cap(u) ({ vec_cap(any(u)); })

#define uv_empty(u) ({ 0 == vec_len(any(u)); })

#define uv_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    vec_exist(any(u), _a);                                                                         \
  })

#define uv_clear(u)                                                                                \
  do {                                                                                             \
    vec_clear(any(u));                                                                             \
  } while (0)

#define uv_cleanup(u)                                                                              \
  do {                                                                                             \
    vec_cleanup(any(u));                                                                           \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

#define uv_at(u, ...)                                                                              \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    vec_at(any(u), _a, &_b);                                                                       \
                                                                                                   \
    _b;                                                                                            \
  })

#define uv_re(u, ...)                                                                              \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = va_at(1, __VA_ARGS__);                                         \
                                                                                                   \
    vec_re(any(u), _a, &_b);                                                                       \
  } while (0)

#define uv_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    vec_pop(any(u), _a, &_b);                                                                      \
                                                                                                   \
    _b;                                                                                            \
  })

#define uv_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = va_at(1, __VA_ARGS__);                                         \
                                                                                                   \
    vec_put(any(u), _a, &_b);                                                                      \
  } while (0)

#define uv_each(u, i, it)                                                                          \
  for (typeof(u(&i, &it)) $ = vec_each_init(any(u), 1); vec_each(any(u), &i, &it);)

#define uv_reach(u, i, it)                                                                         \
  for (typeof(u(&i, &it)) $ = vec_each_init(any(u), 0); vec_each(any(u), &i, &it);)

/***************************************************************************************************
 * iApi tbl
 **************************************************************************************************/
#define ut_init(u)                                                                                 \
  do {                                                                                             \
    u = any(                                                                                       \
        tbl_new(sizeof(typeof(*u(nullptr, nullptr))) / sizeof(typeof(**u(nullptr, nullptr))),      \
                sizeof(typeof(**u(nullptr, nullptr))) / sizeof(typeof(***u(nullptr, nullptr)))));  \
  } while (0)

#define ut_len(u) ({ tbl_len(any(u)); })

#define ut_empty(u) ({ 0 == tbl_len(any(u)); })

#define ut_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    tbl_exist(any(u), &_a);                                                                        \
  })

#define ut_clear(u)                                                                                \
  do {                                                                                             \
    tbl_clear(u);                                                                                  \
  } while (0)

#define ut_cleanup(u)                                                                              \
  do {                                                                                             \
    tbl_cleanup(any(u));                                                                           \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

#define ut_at(u, ...)                                                                              \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    tbl_at(any(u), &_a, &_b);                                                                      \
                                                                                                   \
    _b;                                                                                            \
  })

#define ut_re(u, ...)                                                                              \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = va_at(1, __VA_ARGS__);                                         \
                                                                                                   \
    tbl_re(any(u), &_a, &_b);                                                                      \
  } while (0)

#define ut_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    tbl_pop(any(u), &_a, &_b);                                                                     \
                                                                                                   \
    _b;                                                                                            \
  })

#define ut_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = va_at(1, __VA_ARGS__);                                         \
                                                                                                   \
    tbl_put(any(u), &_a, &_b);                                                                     \
  } while (0)

#define ut_each(u, k, v)                                                                           \
  for (typeof(u(&k, &v)) $ = tbl_each_init(any(u), 1); tbl_each(any(u), &k, &v);)

#define ut_reach(u, k, v)                                                                          \
  for (typeof(u(&k, &v)) $ = tbl_each_init(any(u), 0); tbl_each(any(u), &k, &v);)

/***************************************************************************************************
 * iApi avl
 **************************************************************************************************/
#define ua_init(u, ...)                                                                            \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    u = any(                                                                                       \
        avl_new(sizeof(typeof(*u(nullptr, nullptr))) / sizeof(typeof(**u(nullptr, nullptr))),      \
                sizeof(typeof(**u(nullptr, nullptr))) / sizeof(typeof(***u(nullptr, nullptr))),    \
                va_at(0, __VA_ARGS__)));                                                           \
  } while (0)

#define ua_len(u) ({ avl_len(any(u)); })

#define ua_empty(u) ({ 0 == avl_len(any(u)); })

#define ua_exist(u, ...)                                                                           \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    avl_exist(any(u), &_a);                                                                        \
  })

#define ua_clear(u)                                                                                \
  do {                                                                                             \
    avl_clear(u);                                                                                  \
  } while (0)

#define ua_cleanup(u)                                                                              \
  do {                                                                                             \
    avl_cleanup(any(u));                                                                           \
                                                                                                   \
    u = nullptr;                                                                                   \
  } while (0)

#define ua_at(u, ...)                                                                              \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 1.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    avl_at(any(u), &_a, &_b);                                                                      \
                                                                                                   \
    _b;                                                                                            \
  })

#define ua_re(u, ...)                                                                              \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = va_at(1, __VA_ARGS__);                                         \
                                                                                                   \
    avl_re(any(u), &_a, &_b);                                                                      \
  } while (0)

#define ua_pop(u, ...)                                                                             \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) == 1, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = {};                                                            \
                                                                                                   \
    avl_pop(any(u), &_a, &_b);                                                                     \
                                                                                                   \
    _b;                                                                                            \
  })

#define ua_put(u, ...)                                                                             \
  do {                                                                                             \
    static_assert(va_size(__VA_ARGS__) == 2, "The number of '...' is 2.");                         \
                                                                                                   \
    auto _a                       = va_at(0, __VA_ARGS__);                                         \
    typeof(***u(&_a, nullptr)) _b = va_at(1, __VA_ARGS__);                                         \
                                                                                                   \
    avl_put(any(u), &_a, &_b);                                                                     \
  } while (0)

#define ua_each(u, k, v)                                                                           \
  for (typeof(u(&k, &v)) $ = avl_each_init(any(u), 1); avl_each(any(u), &k, &v);)

#define ua_reach(u, k, v)                                                                          \
  for (typeof(u(&k, &v)) $ = avl_each_init(any(u), 0); avl_each(any(u), &k, &v);)

#if 0
#  define u_first(u)                                                                               \
    ({                                                                                             \
      u_type_check(u, 0, 0, 0, 1, "u_first");                                                      \
                                                                                                   \
      typeof(*u##_env_.b) _b = {};                                                                 \
      auto fn                = igeneric(u, nullptr, nullptr, nullptr, lst_first);                  \
                                                                                                   \
      fn(u, &_b);                                                                                  \
                                                                                                   \
      _b;                                                                                          \
    })

#  define u_last(u)                                                                                \
    ({                                                                                             \
      u_type_check(u, 0, 0, 0, 1, "u_last");                                                       \
                                                                                                   \
      typeof(*u##_env_.b) _b = {};                                                                 \
      auto fn                = igeneric(u, nullptr, nullptr, nullptr, lst_last);                   \
                                                                                                   \
      fn(u, &_b);                                                                                  \
                                                                                                   \
      _b;                                                                                          \
    })

#  define u_next(u, ...)                                                                           \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1);                                                    \
      u_type_check(u, 0, 0, 0, 1, "u_next");                                                       \
                                                                                                   \
      typeof(*u##_env_.a) _a = va_at(0, __VA_ARGS__);                                              \
      typeof(*u##_env_.b) _b = {};                                                                 \
      auto fn                = igeneric(u, nullptr, nullptr, nullptr, lst_next);                   \
                                                                                                   \
      fn(u, &_a, &_b);                                                                             \
                                                                                                   \
      _b;                                                                                          \
    })

#  define u_prev(u, ...)                                                                           \
    ({                                                                                             \
      static_assert(va_size(__VA_ARGS__) == 1);                                                    \
      u_type_check(u, 0, 0, 0, 1, "u_prev");                                                       \
                                                                                                   \
      typeof(*u##_env_.a) _a = va_at(0, __VA_ARGS__);                                              \
      typeof(*u##_env_.b) _b = {};                                                                 \
      auto fn                = igeneric(u, nullptr, nullptr, nullptr, lst_prev);                   \
                                                                                                   \
      fn(u, &_a, &_b);                                                                             \
                                                                                                   \
      _b;                                                                                          \
    })

#  if 0
#    define u_for(u, idx, it)                                                                      \
      for (typeof(u->_.a) idx;                                                                     \
           igeneric(u->_.mate, vec_for_init, tbl_for_init, avl_for_init, lst_for_init)(u->_.mate,  \
                                                                                       1);)        \
        for (typeof(u->_.b) it;                                                                    \
             igeneric(u->_.mate, vec_for, tbl_for, avl_for, lst_for)(u->_.mate, any(&idx), &it);)

#    define u_rfor(u, idx, it)                                                                     \
      for (typeof(u->_.a) idx;                                                                     \
           igeneric(u->_.mate, vec_for_init, tbl_for_init, avl_for_init, lst_for_init)(u->_.mate,  \
                                                                                       0);)        \
        for (typeof(u->_.b) it;                                                                    \
             igeneric(u->_.mate, vec_for, tbl_for, avl_for, lst_for)(u->_.mate, any(&idx), &it);)
#  endif
#endif
