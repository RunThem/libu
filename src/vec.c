#include "u.h"

#define __vec_header                                                                               \
  (sizeof(struct {                                                                                 \
    size_t len;                                                                                    \
    size_t cap;                                                                                    \
  }))

size_t __vec__cap(size_t size) {
  if (size < 8)
    return 8;
  if (size < 1024)
    return size * 2;
  return size + 512;
}

#undef _
#define _(v) (*(v))

#define __vec_init(vec, arg...)                                                                    \
  ({                                                                                               \
    size_t _v_init__cap = __vec__cap(va_0th(0, arg));                                              \
    _(vec)              = u_talloc(__vec_header + vec_itsize(vec) * _v_init__cap, typeof(_(vec))); \
    _(vec)->cap         = (_(vec) == nullptr) ? -1 : _v_init__cap;                                 \
  })

#define __vec_init_from(vec, item, arg...)                                                         \
  ({                                                                                               \
    typeof(item) _v_init_from__arr[] = {item va_slice(0, arg)};                                    \
    vec_init(vec, arrlen(_v_init_from__arr));                                                      \
    vec_push_b(vec, item, arg);                                                                    \
                                                                                                   \
    (void)_v_init_from__arr;                                                                       \
    _(vec)->len;                                                                                   \
  })

#define __vec_clone(vec)                                                                           \
  ({                                                                                               \
    size_t _v_clone__size = __vec_header + vec_itsize(vec) * _(vec)->cap;                          \
    auto _v_clone__vec    = u_talloc(_v_clone__size, typeof(_(vec)));                              \
    memcpy(_v_clone__vec, _(vec), _v_clone__size);                                                 \
                                                                                                   \
    _v_clone__vec;                                                                                 \
  })

#define __vec_empty(vec) ({ _(vec)->len == 0; })
