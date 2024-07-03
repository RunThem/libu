// #define NDEBUG

#define u_vec_defs  u_defs(vec, bool, int, u_vec_t(int))
#define u_set_defs  u_defs(set, item)
#define u_map_defs  u_defs(map, (int, bool), (int, char))
#define u_tree_defs u_defs(tree, (int, bool), (int, char), (int, task_t*))
#define u_list_defs u_defs(list, bool, int, task_t*)

#include <u/u.h>

/* system libs */

#include <limits.h>
#include <threads.h>
#include <ucontext.h>

#if 0
#  include <arpa/inet.h>
#  include <dirent.h>
#  include <fcntl.h>
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
#  include <threads.h>
#  include <ucontext.h>
#  include <unistd.h>
#endif

#if 0

ret_t code = 0;

/*
 * namespace
 *
 * ua:
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

/* 全新版本的字符串原始实现 */
typedef char* u_string_t[2]; /* {raw string pointer, string data pointer} */

typedef struct {
  int id;
  int cost;
} side;

typedef struct {
  int id;
  side sub[4];
} node;

typedef struct {
  int id;
  bool isuse;
  int cost;
  int prev;
} item;

node graph[] = {
    {0, {{1, 4}, {7, 8}, {-1, 0}, {-1, 0}} },
    {1, {{0, 4}, {7, 11}, {2, 8}, {-1, 0}} },
    {2, {{1, 8}, {8, 2}, {5, 4}, {3, 7}}   },
    {3, {{2, 7}, {5, 14}, {4, 9}, {-1, 0}} },
    {4, {{3, 9}, {5, 10}, {-1, 0}, {-1, 0}}},
    {5, {{6, 2}, {2, 4}, {3, 14}, {4, 10}} },
    {6, {{7, 1}, {8, 6}, {5, 2}, {-1, 0}}  },
    {7, {{0, 8}, {1, 11}, {8, 7}, {6, 1}}  },
    {8, {{7, 7}, {6, 6}, {2, 2}, {-1, 0}}  },
};

item table[] = {
    {0, false, 0,       -1},
    {1, false, INT_MAX, -1},
    {2, false, INT_MAX, -1},
    {3, false, INT_MAX, -1},
    {4, false, INT_MAX, -1},
    {5, false, INT_MAX, -1},
    {6, false, INT_MAX, -1},
    {7, false, INT_MAX, -1},
    {8, false, INT_MAX, -1},
};

void spf() {
  int idx  = 0;
  int cost = 0;
  u_arr_for (table, i, _) {
    // 找到最优节点, 未被使用的节点中开销最小
    cost = INT_MAX;
    u_arr_for (table, i, it) {
      if (!it->isuse && it->cost < cost) {
        idx  = it->id;
        cost = it->cost;
      }
    }

    // 标记该节点已被使用
    table[idx].isuse = true;

    // 更新所有子节点
    u_arr_for (graph[idx].sub, i, it) {
      if (table[it->id].isuse) {
        continue;
      }

      if (cost + it->cost < table[it->id].cost) {
        table[it->id].cost = cost + it->cost;
        table[it->id].prev = idx;
      }
    }
  }

  u_arr_for (table, i, it) {
    printf("id %d, cost %d, prev %d\n", it->id, it->cost, it->prev);
  }
}
#endif

typedef struct {
  u_cstr_t ptr;
} _u_str_t;

void set(any_t _self) {
  ((any_t*)_self)[0] = nullptr;
}

typedef struct {
  size_t id;
  any_t fun;      /* 协程执行入口 */
  u8_t* stack;    /* 栈帧 */
  size_t stksize; /* 栈帧大小 */
  ucontext_t ctx; /* 上下文 */
} task_t;

size_t id                    = 1;
size_t count                 = 0;
ucontext_t ctx               = {};
task_t* run                  = nullptr;
u_list_t(task_t*) run_queue  = nullptr;
u_tree_t(int, task_t*) rwait = nullptr;
u_tree_t(int, task_t*) wwait = nullptr;

void task_init() {
  run_queue = u_list_new(task_t*);

  rwait = u_tree_new(int, task_t*, fn_cmp(int));
  wwait = u_tree_new(int, task_t*, fn_cmp(int));
}

task_t* task_new(any_t fun) {
  task_t* self = nullptr;

  self = u_talloc(task_t);
  u_nil_if(self);

  self->stksize = 8192 * 4;
  self->stack   = u_zalloc(self->stksize);
  u_nil_if(self->stack);

  self->id  = id++;
  self->fun = fun;

  getcontext(&self->ctx);

  self->ctx.uc_stack.ss_sp   = self->stack;
  self->ctx.uc_stack.ss_size = self->stksize;

  return self;

err:
  u_free_if(self);

  return nullptr;
}

#define u_task_new(fun, ...)                                                                       \
  do {                                                                                             \
    task_t* _t = task_new(fun);                                                                    \
                                                                                                   \
    makecontext(&_t->ctx, any(fun), va_size(__VA_ARGS__) va_list(0, __VA_ARGS__));                 \
                                                                                                   \
    u_list_put(run_queue, _t);                                                                     \
    count++;                                                                                       \
                                                                                                   \
    u_inf("task count is %zd", count);                                                             \
  } while (0)

void task_scheduler() {
  while (true) {
    u_err_if(count == 0);

    run = u_list_pop(run_queue);
    u_die_if(run == nullptr);

    u_inf("run id %zu", run->id);

    swapcontext(&ctx, &run->ctx);
    run = nullptr;
  }

err:
  u_inf("end");
}

void __fun() {
  u_dbg("task");
}

int main(int argc, const u_cstr_t argv[]) {
  u_log_init();

  task_init();

  u_task_new(__fun, 1, 2);
  u_task_new(__fun, 1, 2);
  u_task_new(__fun, 1, 2);
  u_task_new(__fun, 1, 2);

  task_scheduler();

  return EXIT_SUCCESS;

err:
  return EXIT_FAILURE;
}
