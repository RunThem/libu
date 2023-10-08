#include <u/core/avl.h>
#include <u/core/head.h>
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

  typedef struct {
    int a;
    u_list_node_t lnode;
  } st_t;

  st_t a = {.a = 1};
  st_t b = {.a = 2};
  st_t c = {.a = 3};
  st_t d = {.a = 4};
  st_t e = {.a = 5};

  u_list_t(st_t) l = u_list_new(st_t, lnode);

  u_list_push_back(l, &a);
  u_list_push_back(l, &b);

  u_list_push_front(l, &c);

  u_list_push(l, &c, &d);
  u_list_push(l, &a, &e);

  /*
   * c, d, a, e, b
   * 3, 4, 1, 5, 2
   * */

  infln("%zu", u_list_len(l));

  u_list_for(l) {
    infln("%d", l->item->a);
  }

  infln();

  u_list_rfor(l) {
    infln("%d", l->item->a);
  }

  u_list_pop_back(l);

  infln();

  u_list_for(l) {
    infln("%d", l->item->a);
  }

  u_list_pop_front(l);

  infln();

  u_list_for(l) {
    infln("%d", l->item->a);
  }

  u_list_pop(l, &a);

  infln();

  u_list_for(l) {
    infln("%d", l->item->a);
  }

  u_list_pop(l, &d);

  infln();

  u_list_for(l) {
    infln("%d", l->item->a);
  }

  infln("%zu", u_list_len(l));

  return 0;
}
