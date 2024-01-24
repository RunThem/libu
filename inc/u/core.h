#pragma once

#if 1
#  include "core/iavl.h"
#  include "core/itbl.h"
#  include "core/ivec.h"

typedef void** invalied_type_t;
/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define iu_vec(T)                                                                                \
    struct [[gnu::packed]] {                                                                       \
      struct {                                                                                     \
        u_vec_t mate;                                                                              \
        ssize_t a;                                                                                 \
        T b;                                                                                       \
      } _;                                                                                         \
                                                                                                   \
      T it;                                                                                        \
    }*

#  define iu_tbl(K, V)                                                                             \
    struct [[gnu::packed]] {                                                                       \
      struct {                                                                                     \
        u_tbl_t mate;                                                                              \
        K a;                                                                                       \
        V b;                                                                                       \
      } _;                                                                                         \
                                                                                                   \
      K key;                                                                                       \
      V val;                                                                                       \
    }*

#  define iu_avl(K, V)                                                                             \
    struct [[gnu::packed]] {                                                                       \
      struct {                                                                                     \
        u_avl_t mate;                                                                              \
        K a;                                                                                       \
        V b;                                                                                       \
      } _;                                                                                         \
                                                                                                   \
      K key;                                                                                       \
      V val;                                                                                       \
    }*

/***************************************************************************************************
 * iApi
 **************************************************************************************************/

#  define igeneric(mate, vec, tbl, avl) _Generic(mate, u_vec_t: vec, u_tbl_t: tbl, u_avl_t: avl)

#endif

#define iu_put(u, args...)                                                                         \
  do {                                                                                             \
    u->_.a  = va_at(0, args);                                                                      \
    u->_.b  = va_at(1, args);                                                                      \
    auto fn = igeneric(u->_.mate, vec_put, tbl_put, avl_put);                                      \
                                                                                                   \
    fn(u->_.mate, igeneric(u->_.mate, u->_.a, &u->_.a, &u->_.a), &u->_.b);                         \
  } while (0)
