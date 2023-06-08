#include "u.h"

#define __map_header                                                                               \
  (sizeof(struct {                                                                                 \
    map_hash_t hash_func;                                                                          \
    size_t len;                                                                                    \
    size_t _cap;                                                                                   \
  }))
