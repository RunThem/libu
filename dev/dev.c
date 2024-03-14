/* third libs */
#include <u/u.h>

/* system libs */

#if 0
#  include <arpa/inet.h>
#  include <ctype.h>
#  include <dirent.h>
#  include <fcntl.h>
#  include <math.h>
#  include <net/ethernet.h>
#  include <net/if.h>
#  include <netinet/if_ether.h>
#  include <netinet/in.h>
#  include <netinet/ip.h>
#  include <netinet/ip_icmp.h>
#  include <netpacket/packet.h>
#  include <regex.h>
#  include <sys/ioctl.h>
#  include <sys/resource.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <sys/wait.h>
#  include <ucontext.h>
#  include <unistd.h>
#endif

ret_t code = 0;

/*
 * namespace
 *
 * uv: vector
 * um: hash table
 * ut: avl tree
 * ul: list
 * us: string
 * ub: buffer
 * uf: file & directory
 * ug: log
 * un: network
 * */

int main(int argc, const u_cstr_t argv[]) {
  char str[] = "hello";
  int result = typeeq(char[], str);
  infln("result is %d", result);

  u_str_t s = us_new("");

  println("len is %zu, '%s'", us_len(s), (u_cstr_t)s);

  u_str_t l = us_new("");

  us_put(l, "world");
  println("len is %zu, '%s'", us_len(l), (u_cstr_t)l);

  us_put(s, 'h');
  us_put(s, "ello");
  us_put(s, ' ');
  us_put(s, l);
  us_put(s, '!');

  us_ins(s, 0, '"');
  us_put(s, '"');

  us_ins(s, 6, "<>");

  println("len is %zu, '%s'", us_len(s), (u_cstr_t)s);

  return EXIT_SUCCESS;
err:
  errln("failed.");

  return EXIT_FAILURE;
}
