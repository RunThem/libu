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

u_struct_def(ApiArgs) {
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
  char auto json = new(json_ref_t, .type = 0, .key = _key, .klen = strlen(_key));                  \
                                                                                                   \
  json;                                                                                            \
  })

#define json_t(_key)                                                                               \
  ({                                                                                               \
    auto json = new(json_ref_t, .type = 1, .key = _key, .klen = strlen(_key));                     \
                                                                                                   \
    json;                                                                                          \
  })

#define json_f(_key)                                                                               \
  ({                                                                                               \
    auto json = new(json_ref_t, .type = 2, .key = _key, .klen = strlen(_key));                     \
                                                                                                   \
    json;                                                                                          \
  })

#define json_s(_key, _str, ...)                                                                    \
  ({                                                                                               \
    auto json = new(json_ref_t, .type = 3, .key = _key, .klen = strlen(_key), );                   \
                                                                                                   \
    json->s    = _str;                                                                             \
    json->vlen = strlen(_str);                                                                     \
                                                                                                   \
    json;                                                                                          \
  })

#define json_a(_key, ...)                                                                          \
  ({                                                                                               \
    auto json = new(json_ref_t, .type = 4, .key = _key, .klen = strlen(_key), );                   \
                                                                                                   \
    json->a    = u_vec_new(json_ref_t);                                                            \
    json->vlen = 0;                                                                                \
                                                                                                   \
    json;                                                                                          \
  })

#define json_o(_key, ...)                                                                          \
  ({                                                                                               \
    auto json = new(json_ref_t, .type = 5, .key = _key, .klen = strlen(_key), );                   \
                                                                                                   \
    json->o    = u_vec_new(json_ref_t);                                                            \
    json->vlen = 0;                                                                                \
                                                                                                   \
    json;                                                                                          \
  })

int main(int argc, const u_cstr_t argv[]) {
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

  auto fn = lambda(int* a, int* b) {
    return *a - *b;
  };

#define labmbda_cmp(type) ^int(type * x, type * y)

  auto k = labmbda_cmp(int) {
    return *x - *y;
  };

  auto m = Block_copy(f);

  u_inf("blocks is %d", __has_extension(blocks));

  tb_trace_i("hello");

  u_dict_t(int, int) d = u_dict_new(d);

  tb_exit();

  return EXIT_SUCCESS;

end:
  return EXIT_FAILURE;
}
