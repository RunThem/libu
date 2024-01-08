#pragma once

#include "core/iavl.h"
#include "core/itbl.h"
#include "core/ivec.h"

/* init */
#define u_init(u, args...)                                                                         \
  _Generic((u)->_,                                                                                 \
      u_vec_t: ({                                                                                  \
             (u)    = u_zalloc(sizeof(typeof(*u)));                                                \
             (u)->_ = vec_new(sizeof(typeof((u)->it)), va_0th(U_VEC_CAP, args));                   \
           }),                                                                                     \
      u_tbl_t: ({                                                                                  \
             (u)    = u_zalloc(sizeof(typeof(*(u))));                                              \
             (u)->_ = tbl_new(sizeof(typeof((u)->key)), sizeof(typeof((u)->val)), eq_fn);          \
           }),                                                                                     \
      u_avl_t: ({                                                                                  \
             (u)    = u_zalloc(sizeof(typeof(*(u))));                                              \
             (u)->_ = avl_new(sizeof(typeof((u)->key)), sizeof(typeof((u)->val)), cmp_fn);         \
           }),                                                                                     \
      default: (void)u)

#if 0
/* from */
#  define u_from(u, ...)                                                                           \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_from,                                                                       \
        u_tbl_t: u_tbl_from,                                                                       \
        u_avl_t: u_avl_from,                                                                       \
        default: nullptr)(u, __VA_ARGS__)

/* len */
#  define u_len(u, ...)                                                                            \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_len,                                                                        \
        u_tbl_t: u_tbl_len,                                                                        \
        u_avl_t: u_avl_len,                                                                        \
        default: nullptr)(u, __VA_ARGS__)

/* cap */
#  define u_cap(u, ...) _Generic((u)->_, u_vec_t: u_vec_cap, default: nullptr)(u, __VA_ARGS__)

/* isinit */
#  define u_isinit(u, ...)                                                                         \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_isinit,                                                                     \
        u_tbl_t: u_tbl_isinit,                                                                     \
        u_avl_t: u_avl_isinit,                                                                     \
        default: nullptr)(u, __VA_ARGS__)

/* isempty */
#  define u_isempty(u, ...)                                                                        \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_isempty,                                                                    \
        u_tbl_t: u_tbl_isempty,                                                                    \
        u_avl_t: u_avl_isempty,                                                                    \
        default: nullptr)(u, __VA_ARGS__)

/* isexist */
#  define u_isexist(u, ...)                                                                        \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_isexist,                                                                    \
        u_tbl_t: u_tbl_isexist,                                                                    \
        u_avl_t: u_avl_isexist,                                                                    \
        default: nullptr)(u, __VA_ARGS__)

/* clear */
#  define u_clear(u, ...)                                                                          \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_clear,                                                                      \
        u_tbl_t: u_tbl_clear,                                                                      \
        u_avl_t: u_avl_clear,                                                                      \
        default: nullptr)(u, __VA_ARGS__)

/* cleanup */
#  define u_cleanup(u, ...)                                                                        \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_cleanup,                                                                    \
        u_tbl_t: u_tbl_cleanup,                                                                    \
        u_avl_t: u_avl_cleanup,                                                                    \
        default: nullptr)(u, __VA_ARGS__)

/* at */
#  define u_at(u, ...)                                                                             \
    _Generic((u)->_, u_vec_t: u_vec_at, u_tbl_t: u_tbl_at, u_avl_t: u_avl_at, default: nullptr)(   \
        u,                                                                                         \
        __VA_ARGS__)

/* re */
#  define u_re(u, ...)                                                                             \
    _Generic((u)->_, u_vec_t: u_vec_re, u_tbl_t: u_tbl_re, u_avl_t: u_avl_re, default: nullptr)(   \
        u,                                                                                         \
        __VA_ARGS__)

/* pop */
#  define u_pop(u, ...)                                                                            \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_pop,                                                                        \
        u_tbl_t: u_tbl_pop,                                                                        \
        u_avl_t: u_avl_pop,                                                                        \
        default: nullptr)(u, __VA_ARGS__)

/* put */
#  define u_put(u, ...)                                                                            \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_put,                                                                        \
        u_tbl_t: u_tbl_put,                                                                        \
        u_avl_t: u_avl_put,                                                                        \
        default: nullptr)(u, __VA_ARGS__)

/* for */
#  define u_for(u, ...)                                                                            \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_for,                                                                        \
        u_tbl_t: u_tbl_for,                                                                        \
        u_avl_t: u_avl_for,                                                                        \
        default: nullptr)(u, __VA_ARGS__)

/* rfor */
#  define u_rfor(u, ...)                                                                           \
    _Generic((u)->_,                                                                               \
        u_vec_t: u_vec_rfor,                                                                       \
        u_tbl_t: u_tbl_rfor,                                                                       \
        u_avl_t: u_avl_rfor,                                                                       \
        default: nullptr)(u, __VA_ARGS__)

#endif
