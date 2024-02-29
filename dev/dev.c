/* third libs */
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

ret_t code = 0;

/*
 * namespace
 *
 * uv, ut, ua, ul, us, uf
 * */

int main(int argc, const str_t argv[]) {
  char str[] = "hello";
  int result = typeeq(char[], str);
  infln("result is %d", result);

  return EXIT_SUCCESS;
err:
  errln("failed.");

  return EXIT_FAILURE;
}
