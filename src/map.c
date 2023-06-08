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

#define map_node(map, _key, _value, _hash)                                                         \
  ({                                                                                               \
    map_T(map)* _m_node__node = u_talloc(map_itsize(map), map_T(map)*);                            \
    _m_node__node->hash       = (_hash);                                                           \
    _m_node__node->key        = (_key);                                                            \
    _m_node__node->value      = (_value);                                                          \
    _m_node__node;                                                                                 \
  })

#define __map_push(map, _key, _value)                                                              \
  ({                                                                                               \
    ssize_t _m_push__ret      = 0;                                                                 \
    map_K(map) _m_push__key   = (_key);                                                            \
    map_V(map) _m_push__value = (_value);                                                          \
    size_t _m_push__hash      = _(map)->hash_func((void*)&_m_push__key, sizeof(map_K(map)));       \
    size_t _m_push__idx       = _m_push__hash % (_(map)->_cap);                                    \
    map_T(map)* _m_push__node = (_(map)->nodes[_m_push__idx].next == nullptr) ?                    \
                                    &_(map)->nodes[_m_push__idx] :                                 \
                                    _(map)->nodes[_m_push__idx].next;                              \
                                                                                                   \
    /*                                                                                             \
        inf("push hash 0x%ld, idx %ld, key %d,\t\t value \"%s\"", _m_push__hash, _m_push__idx,     \
       _m_push__key, _m_push__value);                                                              \
    */                                                                                             \
                                                                                                   \
    for (; _m_push__node != nullptr; _m_push__node = _m_push__node->next) {                        \
      if (_m_push__node->hash == _m_push__hash &&                                                  \
          !memcmp((void*)&_m_push__node->key, (void*)&_m_push__key, sizeof(map_K(map)))) {         \
        memcpy((void*)&_m_push__node->value, (void*)&_m_push__value, sizeof(map_V(map)));          \
        _m_push__ret = 1;                                                                          \
      }                                                                                            \
      if (_m_push__node->next == nullptr) {                                                        \
        break;                                                                                     \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    if (_m_push__ret == 0) {                                                                       \
      _m_push__node->next = map_node(map, _key, _value, _m_push__hash);                            \
      _(map)->nodes[_m_push__idx].hash++;                                                          \
      _(map)->len++;                                                                               \
    }                                                                                              \
                                                                                                   \
    _m_push__ret;                                                                                  \
  })

#define __map_pop(map, _key)                                                                       \
  ({                                                                                               \
    map_K(map) _m_pop__key   = (_key);                                                             \
    size_t _m_pop__hash      = _(map)->hash_func((void*)&_m_pop__key, sizeof(map_K(map)));         \
    size_t _m_pop__idx       = _m_pop__hash & (_(map)->_cap - 1);                                  \
    map_T(map)* _m_pop__node = &_(map)->nodes[_m_pop__idx];                                        \
    struct {                                                                                       \
      map_K(map) key;                                                                              \
      map_V(map) value;                                                                            \
    } _m_pop__N = {0};                                                                             \
                                                                                                   \
    for (; _m_pop__node != nullptr; _m_pop__node = _m_pop__node->next) {                           \
      map_T(map)* _m_pop___node = _m_pop__node->next;                                              \
      if (_m_pop___node->hash == _m_pop__hash &&                                                   \
          !memcmp((void*)&_m_pop___node->key, (void*)&_m_pop__key, sizeof(map_K(map)))) {          \
        memcpy((void*)&_m_pop__N, (void*)&_m_pop___node->key, sizeof(_m_pop__N));                  \
                                                                                                   \
        _m_pop__node->next = _m_pop___node->next;                                                  \
        u_free(_m_pop___node);                                                                     \
        _m_pop__node->hash--;                                                                      \
                                                                                                   \
        break;                                                                                     \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    _m_pop__N;                                                                                     \
  })

#define __map_at(map, _key)                                                                        \
  ({                                                                                               \
    map_K(map) _m_at__key      = (_key);                                                           \
    size_t _m_at__hash         = _(map)->hash_func((void*)&_m_at__key, sizeof(map_K(map)));        \
    size_t _m_at__idx          = _m_at__hash & (_(map)->_cap - 1);                                 \
    map_T(map)* _m_at__node    = _(map)->nodes[_m_at__idx].next;                                   \
    map_V(map)* _m_at__value_p = nullptr;                                                          \
                                                                                                   \
    for (; _m_at__node != nullptr; _m_at__node = _m_at__node->next) {                              \
      if (_m_at__node->hash == _m_at__hash &&                                                      \
          !memcmp((void*)&_m_at__node->key, (void*)&_m_at__key, sizeof(map_K(map)))) {             \
        _m_at__value_p = &_m_at__node->value;                                                      \
        break;                                                                                     \
      }                                                                                            \
    }                                                                                              \
                                                                                                   \
    _m_at__value_p;                                                                                \
  })

#define __map_set(map, _key, _value)                                                               \
  ({                                                                                               \
    map_V(map)* _m_set__value_p = map_at(map, _key);                                               \
    map_V(map) _m_set__value    = *_m_set__value_p;                                                \
    map_V(map) _m_set___value   = (_value);                                                        \
                                                                                                   \
    memcpy((void*)_m_set__value_p, (void*)&_m_set___value, sizeof(map_V(map)));                    \
                                                                                                   \
    _m_set__value;                                                                                 \
  })

#define __map_for(map, it)                                                                         \
  for (size_t _m_for__i = 0; _m_for__i < _(map)->_cap; _m_for__i++)                                \
    for (map_T(map) * (it) = _(map)->nodes[_m_for__i].next; (it) != nullptr; (it) = (it)->next)

#define __map_keys(map)                                                                            \
  ({                                                                                               \
    vec(map_K(map)) _m_keys__vec = nullptr;                                                        \
    vec_init(&_m_keys__vec, _(map)->len);                                                          \
                                                                                                   \
    map_for(map, _m_keys__it) {                                                                    \
      vec_push_b(&_m_keys__vec, _m_keys__it->key);                                                 \
    }                                                                                              \
                                                                                                   \
    _m_keys__vec;                                                                                  \
  })

#define __map_values(map)                                                                          \
  ({                                                                                               \
    vec(map_V(map)) _m_values__vec = nullptr;                                                      \
    vec_init(&_m_values__vec, _(map)->len);                                                        \
                                                                                                   \
    map_for(map, _m_values__it) {                                                                  \
      vec_push_b(&_m_values__vec, _m_values__it->value);                                           \
    }                                                                                              \
                                                                                                   \
    _m_values__vec;                                                                                \
  })

#define __map_dis(map, fn)                                                                         \
  do {                                                                                             \
    inf("len %ld, _cap %ld: {", _(map)->len, _(map)->_cap);                                        \
                                                                                                   \
    for (size_t __i = 0; __i < _(map)->_cap; __i++) {                                              \
      if (_(map)->nodes[__i].hash == 0) {                                                          \
        continue;                                                                                  \
      }                                                                                            \
                                                                                                   \
      __prt("  [%ld] len %ld { \n", __i, _(map)->nodes[__i].hash);                                 \
                                                                                                   \
      map_T(map)* __node = _(map)->nodes[__i].next;                                                \
      for (; __node != nullptr; __node = __node->next) {                                           \
        __prt("    ");                                                                             \
        fn(__node->key, __node->value);                                                            \
      }                                                                                            \
                                                                                                   \
      __prt("  }\n");                                                                              \
    }                                                                                              \
                                                                                                   \
    __prt("}\n");                                                                                  \
  } while (0)
