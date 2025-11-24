#include <Block.h>
#include <arpa/inet.h>
#include <tbox/tbox.h>
#include <u/u.h>

/*
 * #[[flag]]
 *
 * */

/*
 * namespace
 *
 * ua: arr
 * uv: vec
 * um: map
 * ut: avl
 * ul: lst
 * us: str
 * ub: buf
 * uf: file
 * ug: log
 * un: net
 * */

/*
 * network
 *
 * tcp4://0.0.0.0:8080
 * udp4://0.0.0.0:8080
 * tcp6://[::1]:8080
 * udp6://[::1]:8080
 * tcpu:///mnt/tcp.sock
 * udpu:///mnt/tcp.sock
 * */

#define lambda(...) ^(__VA_ARGS__)

#define defstruct(T, ...)                                                                          \
  struct T;                                                                                        \
  typedef typeof(struct T) T##_t;                                                                  \
  typedef typeof(struct T*) T##_mut_t;                                                             \
  typedef typeof(const struct T*) T##_ref_t;                                                       \
  struct T

defstruct(ApiArgs) {
  int a;
  int b;
  int c;
};

/*
 * json = string : (null | true | false| string | number | array | object)
 **/

typedef struct json_t json_t, *json_ref_t;
struct json_t {
  u64_t type: 4;  /* {0: null, 1: true, 2: false, 3: string, 4: array, 5: object} */
  u64_t klen: 30; /* key len */

  /*
   * If type == 3, len is the string length.
   * If type == 4, len is the number of elements in the array.
   * If type == 5, len is the number of elements in the object.
   * */
  u64_t vlen: 30;

  char* key;

  union {
    char* s;
    f64_t n;
    u_vec_t(json_ref_t) a;
    u_vec_t(json_ref_t) o;
  };
};

#define json_n(_key)                                                                               \
  char auto json = new (json_ref_t, .type = 0, .key = _key, .klen = strlen(_key));                 \
                                                                                                   \
  json;                                                                                            \
  })

#define json_t(_key)                                                                               \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 1, .key = _key, .klen = strlen(_key));                    \
                                                                                                   \
    json;                                                                                          \
  })

#define json_f(_key)                                                                               \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 2, .key = _key, .klen = strlen(_key));                    \
                                                                                                   \
    json;                                                                                          \
  })

#define json_s(_key, _str, ...)                                                                    \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 3, .key = _key, .klen = strlen(_key), );                  \
                                                                                                   \
    json->s    = _str;                                                                             \
    json->vlen = strlen(_str);                                                                     \
                                                                                                   \
    json;                                                                                          \
  })

#define json_a(_key, ...)                                                                          \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 4, .key = _key, .klen = strlen(_key), );                  \
                                                                                                   \
    json->a    = u_vec_new(json_ref_t);                                                            \
    json->vlen = 0;                                                                                \
                                                                                                   \
    json;                                                                                          \
  })

#define json_o(_key, ...)                                                                          \
  ({                                                                                               \
    auto json = new (json_ref_t, .type = 5, .key = _key, .klen = strlen(_key), );                  \
                                                                                                   \
    json->o    = u_vec_new(json_ref_t);                                                            \
    json->vlen = 0;                                                                                \
                                                                                                   \
    json;                                                                                          \
  })

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static uint8_t maskbit[] = {0x00, 0x80, 0xc0, 0xe0, 0xf0, 0xf8, 0xfc, 0xfe, 0xff};

struct prefix {
  uint8_t family;
  uint8_t prefixlen;
  uint8_t pad1;
  uint8_t pad2;

  union {
    uint8_t prefix;
    uint8_t val[76];
  } u;
};

int prefix_cmp(struct prefix* p1, struct prefix* p2) {
  int offset;
  int shift;

  uint8_t* pp1 = (uint8_t*)&p1->u.prefix;
  uint8_t* pp2 = (uint8_t*)&p2->u.prefix;

  if (p1->family != p2->family || p1->prefixlen != p2->prefixlen) {
    return 1;
  }

  offset = p1->prefixlen / 8;
  shift  = p1->prefixlen % 8;

  if (shift) {
    if (maskbit[shift] & (pp1[offset] ^ pp2[offset])) {
      return 1;
    }
  }

  while (offset--) {
    if (pp1[offset] != pp2[offset]) {
      return 1;
    }
  }

  return 0;
}

int prefix_cmp2(struct prefix* p1, struct prefix* p2) {
  int offset;
  int shift;

  uint8_t* pp1 = (u_int8_t*)&p1->u.prefix;
  uint8_t* pp2 = (u_int8_t*)&p2->u.prefix;

  if (*(u_int16_t*)p1 != *(u_int16_t*)p2) {
    return 1;
  }

  offset = p1->prefixlen / 8;
  shift  = p1->prefixlen % 8;

  return (memcmp(pp1, pp2, offset) != 0 || maskbit[shift] & (pp1[offset] ^ pp2[offset])) ? 1 : 0;
}

int main(int argc, const u_cstr_t argv[]) {
#if 0
  {
    struct prefix a = {.family = 2, .prefixlen = 24};
    struct prefix b = {.family = 2, .prefixlen = 24};

    inet_pton(AF_INET, "192.168.3.0", &a.u.prefix);
    inet_pton(AF_INET, "192.168.3.0", &b.u.prefix);

    printf("a cmp b => %d\n", prefix_cmp(&a, &b));
    printf("a cmp b => %d\n", prefix_cmp2(&a, &b));
  }

  {
    struct prefix a = {.family = 2, .prefixlen = 24};
    struct prefix b = {.family = 2, .prefixlen = 24};

    inet_pton(AF_INET, "192.168.3.0", &a.u.prefix);
    inet_pton(AF_INET, "192.168.3.1", &b.u.prefix);

    printf("a cmp b => %d\n", prefix_cmp(&a, &b));
    printf("a cmp b => %d\n", prefix_cmp2(&a, &b));
  }

  {
    struct prefix a = {.family = 2, .prefixlen = 23};
    struct prefix b = {.family = 2, .prefixlen = 23};

    inet_pton(AF_INET, "192.168.3.1", &a.u.prefix);
    inet_pton(AF_INET, "192.168.3.1", &b.u.prefix);

    printf("a cmp b => %d\n", prefix_cmp(&a, &b));
    printf("a cmp b => %d\n", prefix_cmp2(&a, &b));
  }

  {
    struct prefix a = {.family = 2, .prefixlen = 23};
    struct prefix b = {.family = 2, .prefixlen = 23};

    inet_pton(AF_INET, "192.168.3.1", &a.u.prefix);
    inet_pton(AF_INET, "192.168.4.1", &b.u.prefix);

    printf("a cmp b => %d\n", prefix_cmp(&a, &b));
    printf("a cmp b => %d\n", prefix_cmp2(&a, &b));
  }

  {
    struct prefix a = {.family = 2, .prefixlen = 23};
    struct prefix b = {.family = 2, .prefixlen = 23};

    inet_pton(AF_INET, "192.168.3.1", &a.u.prefix);
    inet_pton(AF_INET, "192.168.3.1", &b.u.prefix);

    u_bench("prefix_cmp", 1'0000) {
      auto result = prefix_cmp(&a, &b);
      (void)result;
    }

    u_bench("prefix_cmp", 1'0000) {
      auto result = prefix_cmp2(&a, &b);
      (void)result;
    }
  }

#endif

  u_inf("hello");
  u_war("hello");
  u_err("hello");
  u_dbg("hello");

#if 0

  json_ref_t json = json_n("null");

  extern void dyad(/* dyad.c */);
  // dyad();

  tb_init(nullptr, nullptr);

  tb_trace_i("hello");
  tb_trace_d("debug");

  auto f = ^(int a) {
    return a * 2;
  };

  auto l = lambda(int a) {
    return a + 3;
  };

  tb_trace_i("%d", f(32));

  u_vec_t(int) v = u_vec_new(int);

  for (int i = 0; i < 10; i++) {
    u_vec_insert(v, i);
  }

  u_vec_each (v, it) {
    tb_trace_i("%d", it);
  }

#  define u_vec_map_by(self, ...)                                                                  \
    ({                                                                                             \
      typeof(self) __self__ = u_vec_new(typeof(self->_[0].val));                                   \
                                                                                                   \
      $vec_each(self->ref, nullptr);                                                               \
      for (typeof_unqual(self->_[0].val) it = {}; $vec_each(self->ref, &it);) {                    \
        __VA_ARGS__;                                                                               \
                                                                                                   \
        u_vec_insert(__self__, it);                                                                \
      }                                                                                            \
                                                                                                   \
      __self__;                                                                                    \
    })

#  define u_vec_filter_by(self, ...)                                                               \
    ({                                                                                             \
      typeof(self) __self__ = u_vec_new(typeof(self->_[0].val));                                   \
                                                                                                   \
      $vec_each(self->ref, nullptr);                                                               \
      for (typeof_unqual(self->_[0].val) it = {}; $vec_each(self->ref, &it);) {                    \
        if (__VA_ARGS__) {                                                                         \
          u_vec_insert(__self__, it);                                                              \
        }                                                                                          \
      }                                                                                            \
                                                                                                   \
      __self__;                                                                                    \
    })

#  define u_vec_all_by(self, ...)                                                                  \
    ({                                                                                             \
      bool __result__ = true;                                                                      \
                                                                                                   \
      $vec_each(self->ref, nullptr);                                                               \
      for (typeof_unqual(self->_[0].val) it = {}; $vec_each(self->ref, &it);) {                    \
        if (!(__VA_ARGS__)) {                                                                      \
          __result__ = false;                                                                      \
          break;                                                                                   \
        }                                                                                          \
      }                                                                                            \
                                                                                                   \
      __result__;                                                                                  \
    })

#  define u_vec_any_by(self, ...)                                                                  \
    ({                                                                                             \
      bool __result__ = false;                                                                     \
                                                                                                   \
      $vec_each(self->ref, nullptr);                                                               \
      for (typeof_unqual(self->_[0].val) it = {}; $vec_each(self->ref, &it);) {                    \
        if (__VA_ARGS__) {                                                                         \
          __result__ = true;                                                                       \
          break;                                                                                   \
        }                                                                                          \
      }                                                                                            \
                                                                                                   \
      __result__;                                                                                  \
    })

  typeof(v) vv = u_vec_map_by(v, { it = it * 2; });

  u_inf("%d", u_vec_all_by(v, it >= 0));

  u_vec_each (vv, it) {
    u_inf("%d", it);
  }

  u_vec_each_if (v, it, it % 2 == 1) {
    u_inf("%d", it);
  }

  u_vec_each (v, it) {
    u_inf("%d", it);
  }

  auto d = u_vec_find_if(v, it < 3);

  auto fn = lambda(int* a, int* b) {
    return *a - *b;
  };

#  define labmbda_cmp(type) ^int(type * x, type * y)

  auto k = labmbda_cmp(int) {
    return *x - *y;
  };

  auto m = Block_copy(f);

  u_inf("blocks is %d", __has_extension(blocks));

  tb_exit();

  {  // Vec
    u_inf("Vec");

    u_vec_t(int) v = u_vec_new(int);

    u_vec_insert(v, 1);
    u_vec_insert(v, 0, 2);
    u_vec_insert(v, 0);

    auto f = u_vec_at(v, 0);

    u_vec_each (v, it) {
      u_inf("%d", it);
    }

    auto ref = u_vec_at_ref(v, 0);
    u_inf("%p is %d", ref, *ref);

    auto mut = u_vec_at_mut(v, 0);
    *mut     = 32;

    u_inf("%p is %d", ref, *ref);

#  undef u_vec_map_by
#  define u_vec_map_by(self, code, ...)                                                            \
    ({                                                                                             \
      typeof(self) __self__ = u_vec_new(typeof(self->_[0].val));                                   \
                                                                                                   \
      $vec_each(self->ref, nullptr);                                                               \
      for (typeof_unqual(self->_[0].val) it = {}; $vec_each(self->ref, &it);) {                    \
        code;                                                                                      \
                                                                                                   \
        u_vec_insert(__self__, it);                                                                \
      }                                                                                            \
                                                                                                   \
      __self__;                                                                                    \
    })

    u_vec_each (v, it) {
      u_inf("%d", it);
    }

    u_vec_map_by(v, free(nullptr), {
      it = it * 2;
      it = it * 23;
    });

    u_vec_each (v, it) {
      u_inf("%d", it);
    }

    u_vec_clear(v, { (void)it; });

    u_vec_at(v, 5);
  }
#endif

#if 0
  { /* Vec */
    u_vec_t(int) v = u_vec_new(int);

    v = u_vec_new(int, 64);

    if (u_vec_is_valid(v, 32)) {
    }

    {
      auto r = u_vec_resize(v, 128);
    }

    {
      auto it = u_vec_at(v, 0);
    }

    {
      auto it = u_vec_at(v, 0, 32);
    }

    {
      auto ref = u_vec_at_ref(v, 0);
      // *ref = 1;
      ref      = nullptr;
    }
    {
      auto mut = u_vec_at_mut(v, 0);
      *mut     = 1;
      mut      = nullptr;
    }

    u_vec_try_at(v, 0) {
      (void)it;
    }

    u_vec_try_at(v, 0, it) {
      (void)it;
    }

    u_vec_try_at_ref(v, 0) {
      (void)it;
    }

    u_vec_try_at_ref(v, 0, it) {
      (void)it;
    }

    u_vec_try_at_mut(v, 0) {
      (void)it;
    }

    u_vec_try_at_mut(v, 0, it) {
      (void)it;
    }

    {
      auto it = u_vec_remove_back(v);
    }

    {
      auto it = u_vec_remove_front(v);
    }

    u_vec_insert_back(v, 32);
    u_vec_insert_front(v, 325);

    u_vec_each (v, it) {
      (void)it;
    }

    u_vec_each_if (v, it, it > 0) {
      (void)it;
    }

    u_vec_each_ref(v, it) {
      (void)it;
    }

    u_vec_each_if_ref(v, it, *it > 0) {
      (void)it;
    }

    u_vec_each_mut(v, it) {
      (void)it;
    }

    u_vec_each_if_mut(v, it, *it > 0) {
      (void)it;
    }

    u_vec_reach(v, it) {
      (void)it;
    }

    u_vec_reach_if(v, it, it > 0) {
      (void)it;
    }

    u_vec_reach_ref(v, it) {
      (void)it;
    }

    u_vec_reach_if_ref(v, it, *it > 0) {
      (void)it;
    }

    u_vec_reach_mut(v, it) {
      (void)it;
    }

    u_vec_reach_if_mut(v, it, *it > 0) {
      (void)it;
    }

    {
      auto it  = u_vec_find_if(v, it > 0);
      auto ref = u_vec_find_if_ref(v, *it > 0);
      auto mut = u_vec_find_if_mut(v, *it > 0);
    }

    {
      auto it  = u_vec_find_nif(v, it > 0);
      auto ref = u_vec_find_nif_ref(v, *it > 0);
      auto mut = u_vec_find_nif_mut(v, *it > 0);
    }

    {
      auto it  = u_vec_rfind_if(v, it > 0);
      auto ref = u_vec_rfind_if_ref(v, *it > 0);
      auto mut = u_vec_rfind_if_mut(v, *it > 0);
    }

    {
      auto it  = u_vec_rfind_nif(v, it > 0);
      auto ref = u_vec_rfind_nif_ref(v, *it > 0);
      auto mut = u_vec_rfind_nif_mut(v, *it > 0);
    }

    {
      auto m = u_vec_map_by(v, { it = 2 * it; });
    }

    {
      u_vec_map_by_mut(v, { it = it * 2; });
    }

    {
      auto m = u_vec_filter_if(v, it > 3);
    }

    {
      u_vec_filter_if_mut(v, it > 3, { (void)it; });
    }

    {
      auto b = u_vec_all_if(v, it > 0);
    }
    {
      auto b = u_vec_all_if_ref(v, *it > 0);
    }

    {
      auto b = u_vec_any_if(v, it > 0);
    }

    {
      auto b = u_vec_any_if_ref(v, *it > 0);
    }

    u_vec_clear(v, { (void)it; });
    u_vec_cleanup(v, { (void)it; });

    typeof_unqual(v->_[0].val) _ = {};
  }

#endif

#if 0
  /* clang-format: on */
  { /* Tree */
    u_tree_t(int, char) v = {};

    auto ok = u_tree_is_valid(v, 0);

    {
      auto val = u_tree_at(v, 0);
    }

    {
      auto val = u_tree_at(v, 0, '3');
    }

    {
      auto ref = u_tree_at_ref(v, 0);
      // *ref     = 1;
      ref      = nullptr;
    }

    {
      auto mut = u_tree_at_mut(v, 0);
      *mut     = 32;
      mut      = nullptr;
    }

    u_tree_try_at(v, 0) {
      (void)it;
    }

    u_tree_try_at(v, 0, it) {
      (void)it;
    }

    u_tree_try_at_ref(v, 0) {
      (void)it;
    }

    u_tree_try_at_ref(v, 0, it) {
      (void)it;
    }

    u_tree_try_at_mut(v, 0) {
      (void)it;
    }

    u_tree_try_at_mut(v, 0, it) {
      (void)it;
    }

    {
      auto it = u_tree_remove(v, 0);
    }

    u_tree_insert(v, 0, 'a');

    {
      u_tree_each (v, it) {
        it.key = 0;
        it.val = '0';
      }
    }

    {
      u_tree_each_if (v, it, it.key % 2 == 0) {
        it.key = 0;
        it.val = '0';
      }
    }

    {
      u_tree_each_ref(v, it) {
        auto s = it->key;
      }
    }

    {
      u_tree_each_if_ref(v, it, it->key % 2 == 0) {
      }
    }

    {
      u_tree_each_mut(v, it) {
        it->val = 0;
        // it->key = 32;
      }
    }

    // #  define cnt const
    // #  define mut

    // {
    //   typeof_unqual(v->_[0].key) key = {};
    //   typeof_unqual(v->_[0].val) val = {};
    //
    //   typeof_unqual(v->_[0].it) it = {};
    //   it.key                       = 0;
    //   it.val                       = 3;
    //
    //   typeof_unqual(v->_[0].ref) ref = {};
    //   ref->key                       = 0;
    //   ref->val                       = 0;
    //
    //   typeof_unqual(v->_[0].mut) mut = {};
    //   mut->key                       = 0;
    //   mut->val                       = 0;
    // }
  }
#endif

  {
    u_dict_t(int, int) d = u_dict_new(int, int);

    u_each (i, 10) {
      u_dict_insert(d, i, i);
    }

    u_inf("len is %d", d->len);

    u_each (i, 10) {
      auto result = u_dict_at(d, i);
      u_inf("d[%d] = %d", i, result);
    }

    u_dict_each(d, it) {
      u_inf("%d, %d", it.key, it.val);
    }

    u_dict_clear(d, u_inf("%d", it.key););
  }

  return EXIT_SUCCESS;

end:
  return EXIT_FAILURE;
}

typedef uint64_t u_ifindex_t;

defstruct(ldp_if) {
  u_ifindex_t ifindex;
};

defstruct(ldp) {
  u_vec_t(ldp_if_t) ifs;
};

void fun() {
  ldp_mut_t ldp = u_talloc(ldp_t);

  ldp->ifs = u_vec_new(ldp_if_ref_t);
  ldp->ifs = nullptr;

  u_vec_each (ldp->ifs, it) {
    auto result = it;
    tb_trace_i("ifindex is %d", result.ifindex);
  }

  u_vec_each_mut(ldp->ifs, it) {
    auto result = it;

    result->ifindex = 0;
  }
}
