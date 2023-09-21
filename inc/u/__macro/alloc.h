#pragma once

#ifdef USE_MIMALLOC
#  include <mimalloc.h>

#  define u_free(p) mi_free(p)

/* clang-format off */
#  define u_malloc(s)     ({ any_t _ptr = mi_malloc(s);     if (errno == 2) errno = 0; _ptr; })
#  define u_zalloc(s)     ({ any_t _ptr = mi_zalloc(s);     if (errno == 2) errno = 0; _ptr; })
#  define u_calloc(c, s)  ({ any_t _ptr = mi_calloc(c, s);  if (errno == 2) errno = 0; _ptr; })
#  define u_realloc(p, s) ({ any_t _ptr = mi_realloc(p, s); if (errno == 2) errno = 0; _ptr; })
/* clang-format on */

#else

#  define u_free(p) free(p)

#  define u_malloc(s)     malloc(s)
#  define u_zalloc(s)     calloc(1, s)
#  define u_calloc(c, s)  calloc(c, s)
#  define u_realloc(p, s) realloc(p, s)

#endif
