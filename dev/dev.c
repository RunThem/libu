/* local libs */
#include <u/core/avl.h>
#include <u/core/heap.h>
#include <u/core/list.h>
#include <u/core/map.h>
#include <u/core/queue.h>
#include <u/core/stack.h>
#include <u/core/str.h>
#include <u/core/vec.h>
#include <u/u.h>
#include <u/util/fs.h>
#include <u/util/obj.h>

/* system libs */
#include <arpa/inet.h>
#include <dirent.h>
#include <net/ethernet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netpacket/packet.h>
#include <stdalign.h>
#include <sys/ioctl.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <threads.h>
#include <unistd.h>

/* third libs */
#include <libsock.h>
// #include <backtrace-supported.h>
// #include <backtrace.h>

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

#if 0
  /*
   * ping baidu.com
   * */
  uint8_t msg[] = {
      0x4c, 0x49, 0xe3, 0x6a, 0x91, 0xbb, 0x9c, 0xfc, 0xe8, 0xe7, 0x22, 0xe1, 0x08, 0x00,
      0x45, 0x00, 0x00, 0x54, 0xf9, 0x00, 0x40, 0x00, 0x40, 0x01, 0xec, 0x47, 0xc0, 0xa8,
      0x2b, 0x12, 0x27, 0x9c, 0x42, 0x0a, 0x08, 0x00, 0xad, 0x2e, 0xa1, 0x36, 0x00, 0x07,
      0xbb, 0x4e, 0x25, 0x65, 0x00, 0x00, 0x00, 0x00, 0x09, 0x0d, 0x01, 0x00, 0x00, 0x00,
      0x00, 0x00, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b,
      0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29,
      0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
  };
  size_t msg_len = sizeof(msg);

  int eth_idx                = 0;
  int ipv4_idx               = 14;
  int icmp_idx               = 34;
  size_t size                = 0;
  struct ifreq ifr           = {0};
  struct sockaddr_ll ll_addr = {0};
  struct sockaddr_in in_addr = {0};

  /*
   * eth send ping
   * */
  int eth_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP));
  u_if(eth_fd < 0) {
  }

  strncpy(ifr.ifr_ifrn.ifrn_name, "wlan0", IFNAMSIZ);
  size = ioctl(eth_fd, SIOCGIFINDEX, &ifr);
  u_if(size < 0) {
  }

  ll_addr.sll_ifindex = ifr.ifr_ifindex;
  ll_addr.sll_family  = ETH_P_IP;

  each(i, 1) {
    size = sendto(eth_fd,
                  &msg[eth_idx],
                  msg_len - eth_idx,
                  0,
                  as(&ll_addr, struct sockaddr*),
                  sizeof(struct sockaddr_ll));

    infln("write is %zu", size);

    sleep(3);
  }

  /*
   * ipv4 send ping
   * */
  int ipv4_fd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
  u_if(ipv4_fd < 0) {
  }

  in_addr.sin_family = AF_INET;
  memcpy(&in_addr.sin_addr.s_addr, &msg[30], 4);

  each(i, 1) {
    size = sendto(ipv4_fd,
                  &msg[ipv4_idx],
                  msg_len - ipv4_idx,
                  0,
                  as(&in_addr, struct sockaddr*),
                  sizeof(struct sockaddr_in));

    infln("write is %zu", size);

    sleep(3);
  }

  /*
   * icmp send ping
   * */
  int icmp_fd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
  u_if(icmp_fd < 0) {
  }

  in_addr.sin_family = AF_INET;
  memcpy(&in_addr.sin_addr.s_addr, &msg[30], 4);

  each(i, 1) {
    size = sendto(icmp_fd,
                  &msg[icmp_idx],
                  msg_len - icmp_idx,
                  0,
                  as(&in_addr, struct sockaddr*),
                  sizeof(struct sockaddr_in));

    infln("write is %zu", size);

    sleep(3);
  }

#  if 0
  int arp_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));
  u_if(arp_fd < 0) {
  }

  int igmp_fd = socket(AF_INET, SOCK_RAW, IPPROTO_IGMP);
  u_if(igmp_fd < 0) {
  }
#  endif

#endif

#if 0
#  define u_str_len(str) (*(as(any(str) - sizeof(size_t), size_t*)))
#  define s(_str)                                                                                  \
    (((struct {                                                                                    \
       size_t cap;                                                                                 \
       size_t len;                                                                                 \
       char data[sizeof(_str)];                                                                    \
     }){.len = sizeof(_str) - 1, .data = (_str)})                                                  \
         .data)

  u_str_t s = s("hello");

  infln("%s", s);
  infln("%zu", u_str_len(s));

  s[0] = '4';

  infln("%s", s);
  infln("%zu", u_str_len(s));

  inf_hex(s - 16, (size_t)8 + 8 + 5 + 1);
#endif

#if 0
  u_str_t s = nullptr;

  s = u_str_new(120);
  infln("%zu, %zu, %s", u_str_len(&s), u_str_cap(&s), s);

  s = u_str_new("hello world");
  infln("%zu, %zu, %s", u_str_len(&s), u_str_cap(&s), s);

  s = u_str_new("hello %s!", "world");
  infln("%zu, %zu, %s", u_str_len(&s), u_str_cap(&s), s);

  u_str_append(&s, " I love you!");
  infln("%zu, %zu, %s", u_str_len(&s), u_str_cap(&s), s);

  u_str_erase(&s, 5, 6);
  infln("%zu, %zu, %s", u_str_len(&s), u_str_cap(&s), s);
#endif

#if 0
  sock_conf_t conf = {
      .type = SOCK_TYPE_INET4_TCP,

      .host = "baidu.com",
      .port = 443,
  };

  auto ret = sock_open(&conf);
  infln("ret is %d, fd is %d", ret, conf.fd);
#endif

#if 0
#  define is_cite(var, ...)                                                                        \
    va_elseif(va_has(__VA_ARGS__))(var, va_at(0, __VA_ARGS__))(&(var), sizeof(var))
#endif

  return EXIT_SUCCESS;
}
