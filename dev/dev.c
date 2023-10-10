#include <u/core/avl.h>
#include <u/core/heap.h>
#include <u/core/list.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/core/vec.h>
#include <u/u.h>
#include <u/util/buf.h>
#include <u/util/fs.h>
#include <u/util/obj.h>
#include <u/util/sock.h>

// #include <backtrace-supported.h>
// #include <backtrace.h>
// #include <stdalign.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <libnet.h>
#include <linux/if_ether.h>
#include <net/if.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <threads.h>

#define _typeof(t) __builtin_classify_type(t)

any_t __bt_state = nullptr;
void boo() {
  // backtrace_print((struct backtrace_state*)__bt_state, 0, stderr);
}

#if 0
typedef any_t u_timer_arg_t;
typedef fnt(u_timer_fn, void, u_timer_arg_t);

typedef struct u_timer_node_t u_timer_node_t;
struct u_timer_node_t {
  uint64_t tick;
  uint64_t period;
  u_timer_fn fun;
  u_timer_arg_t arg;

  u_timer_node_t* parent;
  u_timer_node_t* left;
  u_timer_node_t* right;
};

typedef struct u_timer_t u_timer_t;
struct u_timer_t {
  uint64_t tick;

  u_timer_node_t* root;
};

void fun(u_timer_arg_t arg) {
  printf("%s\n", "hello");
}
#endif

int main(int argc, const char** argv) {
  // __bt_state = backtrace_create_state(argv[1], 0, nullptr, nullptr);

  infln("hello libu!");

#if 0
  u_timer_node_t tack = {.tick = 3, .period = 1, .fun = fun, .arg = nullptr};
  u_timer_t tm        = {.tick = 0, .root = &tack};

  while (true) {
    if (tm.tick > tm.root->tick) {
      tm.root->fun(tm.root->arg);
    }

    usleep(1000 * 1000); /* 0.01 */
    tm.tick++;
  }
#endif

#if 0
  struct [[gnu::packed]] st {
    char c;
    int n;
  }* s;

  infln("%lu", offsetof(struct st, n));

  typedef struct {
    uint64_t a;
    uint64_t b;
    uint64_t c;
    uint64_t d;
    uint64_t e;
    uint64_t f;
  } st;

  struct [[gnu::packed, gnu::aligned(16)]] ll {
    char c;
    st s;
  };

  struct ll llll = {0};
  struct ll* s   = &llll;

  infln("%p, %p", &s->c, &s->s);
#endif

#if 0

  int eth_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
  u_if(eth_fd < 0) {
  }

  int arp_fd = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));
  u_if(arp_fd < 0) {
  }

  int ipv4_fd = socket(AF_INET, SOCK_DGRAM, htons(ETH_P_IP));
  u_if(ipv4_fd < 0) {
  }

  int icmp_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  u_if(icmp_fd < 0) {
  }

  int igmp_fd = socket(AF_INET, SOCK_RAW, IPPROTO_IGMP);
  u_if(igmp_fd < 0) {
  }

  libnet_t* n                      = nullptr;
  char err_buf[LIBNET_ERRBUF_SIZE] = {0};

  n = libnet_init(LIBNET_LINK_ADV, nullptr, err_buf);
  u_err_if(n == nullptr);

  uint8_t target_mac[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55};
  uint8_t sender_mac[] = {0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb};
  uint8_t target_ip[]  = {192, 168, 1, 1};
  uint8_t sender_ip[]  = {192, 168, 1, 2};

  libnet_ptag_t ptag;
  ptag = libnet_build_arp(ARPHRD_ETHER,
                          ETHERTYPE_IP,
                          6,
                          4,
                          ARPOP_REQUEST,
                          sender_mac,
                          sender_ip,
                          target_mac,
                          target_ip,
                          NULL,
                          0,
                          n,
                          0);
  u_err_if(ptag == -1);

  uint8_t dst_mac[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

  ptag = libnet_build_ethernet(dst_mac, sender_mac, ETHERTYPE_ARP, NULL, 0, n, 0);
  u_err_if(ptag == -1);

  int bytes_sent = libnet_write(n);
  u_err_if(bytes_sent == -1);

  libnet_destroy(n);

  return EXIT_SUCCESS;

err:
  return EXIT_FAILURE;

#endif

  return EXIT_SUCCESS;
}
