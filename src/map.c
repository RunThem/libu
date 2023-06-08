#include "u.h"

#define __map_header                                                                               \
  (sizeof(struct {                                                                                 \
    map_hash_t hash_func;                                                                          \
    size_t len;                                                                                    \
    size_t _cap;                                                                                   \
  }))

#define __map_init(map, arg...)                                                                    \
  ({                                                                                               \
    size_t _m_init__cap = va_0th(128, arg);                                                        \
    _(map)              = u_talloc(__map_header + map_itsize(map) * _m_init__cap, typeof(_(map))); \
    _(map)->_cap        = (_(map) == nullptr) ? -1 : _m_init__cap;                                 \
    _(map)->hash_func   = map_mem_hash;                                                            \
  })

#define __map_clear(map)                                                                           \
  ({                                                                                               \
    for (size_t _m_clear__i = 0; _m_clear__i < _(map)->_cap; _m_clear__i++) {                      \
      if (_(map)->nodes[_m_clear__i].hash == 0) {                                                  \
        continue;                                                                                  \
      }                                                                                            \
                                                                                                   \
      map_T(map)* _m_clear__node = _(map)->nodes[_m_clear__i].next;                                \
      while (_m_clear__node != nullptr) {                                                          \
        void* _m_clear__tmp = _m_clear__node->next;                                                \
        u_free(_m_clear__node);                                                                    \
        _m_clear__node = _m_clear__tmp;                                                            \
      };                                                                                           \
                                                                                                   \
      _(map)->nodes[_m_clear__i].hash = 0;                                                         \
      _(map)->nodes[_m_clear__i].next = nullptr;                                                   \
    }                                                                                              \
                                                                                                   \
    _(map)->len = 0;                                                                               \
  })

#define __map_cleanup(map)                                                                         \
  ({                                                                                               \
    map_clear(map);                                                                                \
    u_free(_(map));                                                                                \
    _(map) = nullptr;                                                                              \
  })
