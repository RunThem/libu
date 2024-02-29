/* local libs */
#include <libsock.h>
#include <tbox/tbox.h>
#include <u/u.h>

/* system libs */
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h>
#include <regex.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <ucontext.h>
#include <unistd.h>

/* third libs */

#define __typeof(t)     (__builtin_classify_type(t))
#define typeeq(t1, t2)  (__builtin_types_compatible_p(typeof(t1), typeof(t2)))
#define constexpr(expr) (__builtin_constant_p(expr))

ret_t code = 0;

/*
 * namespace
 *
 * uv, ut, ua, ul, us, uf
 * */

int main(int argc, const char** argv) {

  char str[] = "hello";
  int result = typeeq(char[], str);
  infln("result is %d", result);

#define u_vec(...)                                                                                 \
  ({                                                                                               \
    static_assert(va_size(__VA_ARGS__) != 0);                                                      \
                                                                                                   \
    typeof(va_at(0, __VA_ARGS__)) _arr[]     = {__VA_ARGS__};                                      \
    uvec(typeof(va_at(0, __VA_ARGS__))) _vec = {};                                                 \
    uv_init(_vec);                                                                                 \
                                                                                                   \
    arr_for(_arr, i) {                                                                             \
      uv_put(_vec, -1, _arr[i]);                                                                   \
    }                                                                                              \
                                                                                                   \
    _vec;                                                                                          \
  })

#define u_map()
  auto v = u_vec(1, 2, 3, 4, 5);

  uv_foreach(v, ssize_t, i, int, it, {
    ;
    println("[%zu], %d", i, it);
  });

  return EXIT_SUCCESS;
err:
  errln("failed.");

  return EXIT_FAILURE;
}
