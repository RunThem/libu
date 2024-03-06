/* third libs */
#include <u/u.h>

/* system libs */
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include <math.h>
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

ret_t code = 0;

/*
 * namespace
 *
 * uv, um, ut, ul, u_thrdpl
 * */

uvec(size_t) prime(size_t N) {
  bool is_prime = false;
  size_t square = 0;

  auto tbl = uv_new(size_t);

  uv_put(tbl, 2);

  for (size_t num = 3; num < N; num++) {
    is_prime = true;
    square   = (size_t)sqrt((f64_t)num);

    uv_for_all(tbl, i, it) {
      if (it >= square) {
        break;
      }

      if (num % it == 0) {
        is_prime = false;
        break;
      }
    }

    if (is_prime) {
      uv_put(tbl, num);
    }
  }

  return tbl;
}

int main(int argc, const str_t argv[]) {
  char str[] = "hello";
  int result = typeeq(char[], str);

  infln("result is %d", result);

  return EXIT_SUCCESS;
err:
  errln("failed.");

  return EXIT_FAILURE;
}
