#pragma once

#if 0
#  include "core/iavl.h"
#  include "core/itbl.h"
#  include "core/ivec.h"

typedef void** invalied_type_t;
/***************************************************************************************************
 * iType
 **************************************************************************************************/
#  define u_vec(T)                                                                                 \
    struct [[gnu::packed]] {                                                                       \
      struct {                                                                                     \
        u_vec_t mate;                                                                              \
        ssize_t* a;                                                                                \
        typeof(T*) b;                                                                              \
      } _;                                                                                         \
                                                                                                   \
      T it;                                                                                        \
    }*

#  define u_tbl(K, V)                                                                              \
    struct [[gnu::packed]] {                                                                       \
      struct {                                                                                     \
        u_tbl_t mate;                                                                              \
        typeof(K*) a;                                                                              \
        typeof(V*) b;                                                                              \
      } _;                                                                                         \
                                                                                                   \
      K key;                                                                                       \
      V val;                                                                                       \
    }*

#  define u_avl(K, V)                                                                              \
    struct [[gnu::packed]] {                                                                       \
      struct {                                                                                     \
        u_avl_t mate;                                                                              \
        typeof(K*) a;                                                                              \
        typeof(V*) b;                                                                              \
      } _;                                                                                         \
                                                                                                   \
      K key;                                                                                       \
      V val;                                                                                       \
    }*

/***************************************************************************************************
 * iApi
 **************************************************************************************************/

#  define igeneric(mate, vec, tbl, avl, def, ...)                                                  \
    (_Generic(mate, u_vec_t: vec, u_tbl_t: tbl, u_avl_t: avl, default: def))
#endif
