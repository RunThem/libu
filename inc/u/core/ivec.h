#pragma once

#include <u/u.h>

/*************************************************************************************************
 * Macro
 *************************************************************************************************/
#define U_VEC_CAP 16

/*************************************************************************************************
 * iApi
 *************************************************************************************************/
typedef struct {
}* u_vec_t;

extern u_vec_t vec_new(size_t itsize, size_t cap);

extern size_t vec_len(u_vec_t _self);
extern size_t vec_cap(u_vec_t _self);
extern bool vec_isempty(u_vec_t _self);

extern void vec_clear(u_vec_t _self);
extern void vec_cleanup(u_vec_t _self);
extern void vec_at(u_vec_t self, size_t idx, any_t item);
extern void vec_re(u_vec_t self, size_t idx, any_t item);
extern void vec_pop(u_vec_t self, size_t idx, any_t item);
extern void vec_put(u_vec_t _self, ssize_t idx, any_t item);

extern bool vec_range(u_vec_t _self, ssize_t* idx, bool flag, any_t item);

/*************************************************************************************************
 * Api
 *************************************************************************************************/
#define u_vec(T)                                                                                   \
  struct [[gnu::packed]] {                                                                         \
    u_vec_t _;                                                                                     \
    T item;                                                                                        \
  }

#define u_vec_new(T, args...)                                                                      \
  { vec_new(sizeof(T), va_0th(U_VEC_CAP, args)) }

#define u_vec_init(vec, args...)                                                                   \
  do {                                                                                             \
    (vec)._ = vec_new(sizeof((vec).item), args);                                                   \
  } while (0)

#define u_vec_len(vec)     ({ vec_len((vec)._); })
#define u_vec_cap(vec)     ({ vec_cap((vec)._); })
#define u_vec_isempty(vec) ({ vec_isempty((vec)._); })
#define u_vec_isinit(vec)  ({ (vec)._ != nullptr; })

#define u_vec_clear(vec)                                                                           \
  do {                                                                                             \
    vec_clear((vec)._);                                                                            \
  } while (0)

#define u_vec_cleanup(vec)                                                                         \
  do {                                                                                             \
    vec_cleanup((vec)._);                                                                          \
    (vec)._ = nullptr;                                                                             \
  } while (0)

#define u_vec_at(vec, idx)                                                                         \
  ({                                                                                               \
    vec_at((vec)._, idx, &(vec).item);                                                             \
    (vec).item;                                                                                    \
  })

#define u_vec_re(vec, idx, _item)                                                                  \
  do {                                                                                             \
    (vec).item = _item;                                                                            \
    vec_re((vec)._, idx, &(vec).item);                                                             \
  } while (0)

#define u_vec_pop(vec, idx)                                                                        \
  ({                                                                                               \
    vec_pop((vec)._, idx, &(vec).item);                                                            \
    (void)((vec).item);                                                                            \
  })

#define u_vec_put(vec, idx, _item)                                                                 \
  do {                                                                                             \
    (vec).item = _item;                                                                            \
    vec_put((vec)._, idx, &(vec).item);                                                            \
  } while (0)

#define u_vec_for(vec, i)  for (ssize_t i = -2; vec_range((vec)._, &(i), 1, &(vec).item); (i)++)
#define u_vec_rfor(vec, i) for (ssize_t i = -2; vec_range((vec)._, &(i), 0, &(vec).item); (i)--)
