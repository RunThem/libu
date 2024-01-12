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

extern u_vec_t vec_new(size_t itsize);

extern size_t vec_len(u_vec_t _self);
extern size_t vec_cap(u_vec_t _self);

extern bool vec_isexist(u_vec_t _self, size_t idx);

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
#if 0
#  define u_vec(T)                                                                                 \
    struct [[gnu::packed]] {                                                                       \
      u_vec_t _;                                                                                   \
      T it;                                                                                        \
    }*

#  define u_vec_init(vec, args...)                                                                 \
    do {                                                                                           \
      (vec)    = u_zalloc(sizeof(typeof(*vec)));                                                   \
      (vec)->_ = vec_new(sizeof(typeof((vec)->it)), va_0th(U_VEC_CAP, args));                      \
    } while (0)

#  define u_vec_from(vec, ...)                                                                     \
    do {                                                                                           \
      typeof((vec)->it) _its[] = __VA_ARGS__;                                                      \
                                                                                                   \
      u_vec_init(vec, sizeof(_its) / sizeof(_its[0]));                                             \
                                                                                                   \
      for (size_t i = 0; i < sizeof(_its) / sizeof(_its[0]); i++) {                                \
        u_vec_put(vec, -1, _its[i]);                                                               \
      }                                                                                            \
    } while (0)

#  define u_vec_len(vec) ({ vec_len((vec)->_); })

#  define u_vec_cap(vec) ({ vec_cap((vec)->_); })

#  define u_vec_isinit(vec) ({ (vec) != nullptr && (vec)->_ != nullptr; })

#  define u_vec_isempty(vec) ({ vec_len((vec)->_) == 0; })

#  define u_vec_isexist(vec, idx) ({ vec_isexist(vec, idx); })

#  define u_vec_clear(vec)                                                                         \
    do {                                                                                           \
      vec_clear((vec)->_);                                                                         \
    } while (0)

#  define u_vec_cleanup(vec)                                                                       \
    do {                                                                                           \
      vec_cleanup((vec)->_);                                                                       \
      u_free_if(vec);                                                                              \
      (vec) = nullptr;                                                                             \
    } while (0)

#  define u_vec_at(vec, idx)                                                                       \
    ({                                                                                             \
      vec_at((vec)->_, idx, &(vec)->it);                                                           \
      (vec)->it;                                                                                   \
    })

#  define u_vec_re(vec, idx, _it)                                                                  \
    do {                                                                                           \
      (vec)->it = _it;                                                                             \
      vec_re((vec)->_, idx, &(vec)->it);                                                           \
    } while (0)

#  define u_vec_pop(vec, idx)                                                                      \
    ({                                                                                             \
      vec_pop((vec)->_, idx, &(vec)->it);                                                          \
      (void)((vec)->it);                                                                           \
    })

#  define u_vec_put(vec, idx, _it)                                                                 \
    do {                                                                                           \
      (vec)->it = _it;                                                                             \
      vec_put((vec)->_, idx, &(vec)->it);                                                          \
    } while (0)

#  define u_vec_for(vec, i) for (ssize_t i = -2; vec_range((vec)->_, &(i), 1, &(vec)->it); (i)++)

#  define u_vec_rfor(vec, i) for (ssize_t i = -2; vec_range((vec)->_, &(i), 0, &(vec)->it); (i)--)
#endif
