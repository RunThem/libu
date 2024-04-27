#define NDEBUG

#define u_vec_defs  u_defs(vec, bool, int)
#define u_set_defs  u_defs(set, item)
#define u_map_defs  u_defs(map, (int, bool), (int, char))
#define u_tree_defs u_defs(tree, (int, bool), (int, char), (int, u_zero_size_type_t))
#define u_list_defs u_defs(list, bool, int)

#include <u/u.h>

/* system libs */

#if 0
#  include <limits.h>
#  include <stdlib.h>
// #include <task.h>

#  if 0
#    include <arpa/inet.h>
#    include <dirent.h>
#    include <fcntl.h>
#    include <net/ethernet.h>
#    include <net/if.h>
#    include <netinet/if_ether.h>
#    include <netinet/in.h>
#    include <netinet/ip.h>
#    include <netinet/ip_icmp.h>
#    include <netpacket/packet.h>
#    include <regex.h>
#    include <sys/ioctl.h>
#    include <sys/resource.h>
#    include <sys/socket.h>
#    include <sys/time.h>
#    include <sys/wait.h>
#    include <threads.h>
#    include <ucontext.h>
#    include <unistd.h>
#  endif

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

#  define mtx_block(mtx) for (bool _ = true; _ && !mtx_lock(mtx); _ = false, mtx_unlock(mtx))

#  define u_check(expr, ...)                                                                       \
    if (expr)                                                                                      \
    __VA_OPT__({ __VA_ARGS__; })

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

#  define u_arr_for(arr, i, it)                                                                    \
    for (size_t i = 0; i < arrlen(arr); i++)                                                       \
      for (auto it = &arr[i]; it; it = nullptr)

void spf() {
  int idx  = 0;
  int cost = 0;
  u_arr_for(table, i, _) {
    // 找到最优节点, 未被使用的节点中开销最小
    cost = INT_MAX;
    u_arr_for(table, i, it) {
      if (!it->isuse && it->cost < cost) {
        idx  = it->id;
        cost = it->cost;
      }
    }

    // 标记该节点已被使用
    table[idx].isuse = true;

    // 更新所有子节点
    u_arr_for(graph[idx].sub, i, it) {
      if (table[it->id].isuse) {
        continue;
      }

      if (cost + it->cost < table[it->id].cost) {
        table[it->id].cost = cost + it->cost;
        table[it->id].prev = idx;
      }
    }
  }

  u_arr_for(table, i, it) {
    printf("id %d, cost %d, prev %d\n", it->id, it->cost, it->prev);
  }
}
#endif

typedef struct {
  const u_cstr_t ptr;
} _u_str_t;

void set(any_t _self) {
  ((any_t*)_self)[0] = nullptr;
}

#include <u/utils/va.h>

typedef struct {
  int a;
  int b;
} item;

fn_compe_def(item, x.a == y.a, x.a > y.a);
fn_compe_dec(item);

int main(int argc, const u_cstr_t argv[]) {
  infln("%lu", sizeof(enum {T, F}));

  _u_str_t str = {"hello"};

  println("%p", str.ptr);

  set(&str);

  println("%p", str.ptr);

  u_set_t(item) s = u_set_new(item, fn_cmp(item));

  // u_set_init(s, fn_cmp(item));

  println("len is %zu", u_set_len(s));
  println("empty is %d", u_set_is_empty(s));

  u_set_put(s, me(item, .a = 12, .b = 100));
  u_set_put(s, me(item, .a = 13, .b = 200));

  println("len is %zu", u_set_len(s));
  println("empty is %d", u_set_is_empty(s));

  println("%d", u_set_at(s, me(item, .a = 12)).b);
  println("%d", u_set_at(s, me(item, .a = 13)).b);

  u_set_at(s, me(item, .a = 12), me(item, .a = 13, .b = 300));

  println("len is %zu", u_set_len(s));
  println("empty is %d", u_set_is_empty(s));

  println("%d", u_set_at(s, me(item, .a = 12)).b);
  println("%d", u_set_at(s, me(item, .a = 13)).b);

  item sodf = {.a = 12};

  u_set_try(s, sodf) {
    println("%p", it);
    println("%d", it->a);
    println("%d", it->b);
  }

  // println("%d", u_set_pop(s, me(item, .a = 14)).a);

  u_set_for(s, it) {
    println("%d, %d", it.a, it.b);
  }

  u_set_clear(s);

  u_set_cleanup(s);

  return EXIT_SUCCESS;

err:
  errln("failed.");

  return EXIT_FAILURE;
}
