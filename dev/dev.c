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

#if 1
typedef any_t u_tack_arg_t;
typedef fnt(u_tack_fn, void, u_tack_arg_t);

typedef struct u_tack_t u_tack_t;
struct u_tack_t {
  u64_t tick;
  u64_t period;
  u_tack_fn fu;
  u_tack_arg_t arg;
};

typedef struct iheap_t iheap_t;
struct iheap_t {
  size_t len;
  size_t cap;

  u_tack_t* items;
};

typedef struct u_timer_t u_timer_t;
struct u_timer_t {
  u64_t tick;

  u_tack_t* root;
};

void fun(u_tack_arg_t arg) {
  printf("%s\n", "hello");
}
#endif

iheap_t* iheap_new(size_t cap) {
  iheap_t* self = nullptr;

  self = u_zalloc(sizeof(iheap_t));
  u_mem_if(self);

  self->items = u_zalloc(sizeof(int) * cap);
  u_mem_if(self->items);

  self->len = 0;
  self->cap = cap;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

void iheap_push(iheap_t* self, u_tack_t it) {
  ssize_t pidx = 0; /* parent index */
  ssize_t idx  = (ssize_t)self->len;

  /* TODO: resize */
  self->items[self->len] = it;

  for (pidx = (idx - 1) / 2; pidx >= 0; idx = pidx, pidx = (idx - 1) / 2) {
    if (self->items[idx].tick >= self->items[pidx].tick) {
      break;
    }

    swap(self->items[idx], self->items[pidx]);
  }

  self->len++;
}

u_tack_t iheap_pop(iheap_t* self) {
  u_tack_t it  = {0};
  ssize_t cidx = 0; /* child index */
  ssize_t idx  = 0;

  it             = self->items[0];
  self->items[0] = self->items[--self->len];

  for (cidx = idx * 2 + 1; cidx < self->len; idx = cidx, cidx = idx * 2 + 1) {
    if ((cidx < self->len) && (self->items[cidx].tick > self->items[cidx + 1].tick)) {
      cidx++;
    }

    if (self->items[idx].tick <= self->items[cidx].tick) {
      break;
    }

    swap(self->items[idx], self->items[cidx]);
  }

  return it;
}

u_tack_t ihead_peek(iheap_t* self) {
  return self->items[0];
}

bool ihead_empty(iheap_t* self) {
  return self->len == 0;
}

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

  iheap_t* h = iheap_new(20);

  struct {
    int a;
    int b;
  } s = {0};

  return EXIT_SUCCESS;
}
